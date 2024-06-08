#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "LineParser.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

typedef struct process{
    cmdLine* cmd;                         /* the parsed command line*/
    pid_t pid; 		                  /* the process id that is running the command*/
    int status;                           /* status of the process: RUNNING/SUSPENDED/TERMINATED */
    struct process *next;	                  /* next process in chain */
} process;

#define TERMINATED  -1
#define RUNNING 1
#define SUSPENDED 0

void addProcess(process** process_list, cmdLine* cmd, pid_t pid);
void printProcessList(process** process_list);
void freeProcessList(process* process_list);
void updateProcessList(process **process_list);
void updateProcessStatus(process* process_list, int pid, int status);
void execute(cmdLine *pCmdLine);
int debug;
process* p_list;

int main(int argc, char*argv[]){
    if(argc>1 && strncmp("-d",argv[1],2)==0){
        debug = 1;
    }
    while(1){
        char dir[PATH_MAX];
        getcwd(dir,PATH_MAX);
        printf("~%s$ > ",dir);
        char line[2048];
        fgets(line,2048,stdin);
        cmdLine* cmdL = parseCmdLines(line);
        if(strncmp("quit",cmdL->arguments[0],4) == 0){
            freeCmdLines(cmdL);
            break;
        }
        if(strncmp("cd",cmdL->arguments[0],2) == 0){
            char newDir[PATH_MAX];
            memset(newDir,0,PATH_MAX);
            strcat(newDir,dir);
            strcat(newDir,"/");
            strcat(newDir,cmdL->arguments[1]);
            if(chdir(newDir) == -1){
                perror("ERROR");
            }
            freeCmdLines(cmdL);
        }
        else if(strncmp("procs",cmdL->arguments[0],5) == 0){
            printProcessList(&p_list);
            freeCmdLines(cmdL);
        }
        else
        {
            execute(cmdL);
        }
        //sleep(1); //for good printing only
    }
    freeProcessList(p_list);
    return 0;
}


void execute(cmdLine *pCmdLine){
    if(strncmp("suspend",pCmdLine->arguments[0],7)==0){
        int p_id = atoi(pCmdLine->arguments[1]);
        int time = atoi(pCmdLine->arguments[2]);
        if(fork()==0){
            kill(p_id,SIGTSTP);
            sleep(time);
            kill(p_id,SIGCONT);
            freeCmdLines(pCmdLine);
            _exit(1);
        }
        freeCmdLines(pCmdLine);
    }else if(strncmp("kill",pCmdLine->arguments[0],4) == 0){
        int p_id = atoi(pCmdLine->arguments[1]);
        kill(p_id,SIGINT);
        updateProcessStatus(p_list,p_id,TERMINATED);
        freeCmdLines(pCmdLine);
    }else{
        int pid;
        if((pid = fork()) <0){
            freeCmdLines(pCmdLine);
            _exit(1);
        }
        else if(pid ==0){  //child
            if(debug ==1)
                fprintf(stderr,"Child PID: [%d]\n",getpid());
            else if(execvp(pCmdLine->arguments[0],pCmdLine->arguments) < 0){
                perror("ERROR");
                freeCmdLines(pCmdLine);
                _exit(1);
            }
        }
        else  //parent
        {
            if(debug == 1){
                fprintf(stderr,"Parent PID: [%d]\n",getpid());
                fprintf(stderr,"Command: [%s]\n",pCmdLine->arguments[0]);           
            }
            addProcess(&p_list,pCmdLine,pid);
            if(pCmdLine-> blocking){
                waitpid(pid,NULL,0);
            }
        }
    }
}

void addProcess(process** process_list, cmdLine* cmd, pid_t pid){
    process* new_proccess = malloc(sizeof(struct process));
    new_proccess->cmd = cmd;
    new_proccess->pid = pid;
    new_proccess->status = RUNNING;
    if(*process_list == NULL){
        new_proccess->next = NULL;
        *process_list = new_proccess;
    } else{
    new_proccess->next = *process_list;
    *process_list = new_proccess;
    }
}

void printProcessList(process** process_list){
    printf("PID\t\tCommand\t\tSTATUS\n");
    process* p = *process_list;
    updateProcessList(process_list);
    process *last = NULL;
    process *head = *process_list;
    while (p!=NULL)
    {
        char* status = (p)->status == TERMINATED? "TERMINATED":
                       (p)->status == RUNNING?"RUNNING":
                       "SUSPENDED";
        printf("%d\t\t%s\t\t%s\n",(p)->pid,(p)->cmd->arguments[0],status);
        process* next = p->next;
        if(p->status == TERMINATED){
            if(last!=NULL){
                last->next = next;
            }
            else
                head = next;
            freeCmdLines(p->cmd);
            free(p);
        }else
            last = p;
        p = (p)->next;        
    }
    p_list = head;
}

void updateProcessStatus(process* process_list, int pid, int status){
    while(process_list!=NULL){
        if(process_list->pid == pid){
            process_list->status = status;
            break;
        }
        process_list=process_list->next;
    }
}

void freeProcessList(process* process_list){
    while(process_list != NULL){
        process* next = process_list->next;
        freeCmdLines(process_list->cmd);
        free(process_list);
        process_list = next;
    }
}

void updateProcessList(process **process_list){
    process* p = *process_list;
    int status;
    pid_t child_pid;
    while(p!=NULL){
        child_pid = waitpid(p->pid, &status,WNOHANG | WUNTRACED | WCONTINUED);
        if(child_pid == TERMINATED){
            updateProcessStatus(*process_list,p->pid,TERMINATED);
        }
        if(child_pid >0){
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                updateProcessStatus(*process_list, p->pid, TERMINATED);
            }
            else if(WIFCONTINUED(status)){
                updateProcessStatus(*process_list, p->pid, RUNNING);
            }
            else if(WIFSTOPPED(status)){
                updateProcessStatus(*process_list, p->pid, SUSPENDED);
            }
        }
        p = p->next;
    }
}
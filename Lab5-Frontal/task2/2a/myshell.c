#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "LineParser.h"
#include <sys/types.h>
#include <sys/wait.h>

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
        }
        else if(strncmp("procs",cmdL->arguments[0],5) == 0){
            printProcessList(&p_list);
        }
        else
        {
            execute(cmdL);
        }
        //sleep(1); //for good printing only
    }
    return 0;
}


void execute(cmdLine *pCmdLine){
    int pid;
    if((pid = fork()) <0){
        _exit(1);
    }
    else if(pid ==0){
        if(debug ==1)
            fprintf(stderr,"Child PID: [%d]\n",getpid());
        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments) < 0){
            perror("ERROR");
            freeCmdLines(pCmdLine);
            exit(1);
        }
    }
    else
    {
        addProcess(&p_list,pCmdLine,pid);
        if(pCmdLine-> blocking){
            waitpid(pid,NULL,0);
        }
        if(debug == 1){
            fprintf(stderr,"Parent PID: [%d]\n",getpid());
            fprintf(stderr,"Command: [%s]\n",pCmdLine->arguments[0]);           
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
    while (p!=NULL)
    {
        printf("%d\t\t%s\t\t%s\n",(p)->pid,(p)->cmd->arguments[0],
               (p)->status == 0? "SUSPENDED":
               (p)->status == 1?"RUNNING":
               "TERMINATED");
        p = (p)->next;        
    }
}
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "LineParser.h"
#include <sys/types.h>
#include <sys/wait.h>

void execute(cmdLine *pCmdLine);
int debug;

int main(int argc, char*argv[]){
    if(argc>1 && strncmp("-d",argv[1],2)==0){
        debug = 1;
    }
    while(1){
        char dir[PATH_MAX];
        getcwd(dir,PATH_MAX);
        printf("\n~%s$ > ",dir);
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
        else
        {
            execute(cmdL);
            freeCmdLines(cmdL);
        }
        //sleep(1); //for good printing only
    }
    return 0;
}


void execute(cmdLine *pCmdLine){
    int pid;
    int mypipe[2];
    char buffer[1024];
    if((pid = fork()) <0){
        _exit(1);
    }
    else if(pid ==0){ //child
        if(debug ==1)
            fprintf(stderr,"Child PID: [%d]\n",getpid());
        if(pipe(mypipe) == -1){
            perror ("Cannot pipe");
            _exit(1);     
        }
        if(pCmdLine->inputRedirect !=NULL){
            FILE* fd = fopen(pCmdLine->inputRedirect,"r");
            int read = fread(buffer,1,sizeof(buffer),fd);
            write(mypipe[1],buffer,read);
            fclose(fd);
        }
        if(pCmdLine->outputRedirect !=NULL){
            char buf[1024];
            FILE* fd = fopen(pCmdLine->outputRedirect,"w");
            int write = read(mypipe[0],buf,sizeof(buf));
            fwrite(buf,1,write,fd);
            fclose(fd);
        }
        close(mypipe[0]);
        close(mypipe[1]);
        if(pCmdLine->inputRedirect !=NULL || pCmdLine->outputRedirect !=NULL)
            _exit(0);

        if(execvp(pCmdLine->arguments[0],pCmdLine->arguments) < 0){
            perror("ERROR");
            freeCmdLines(pCmdLine);
            exit(1);
        }
    }
    else //parent
    {
        if(pCmdLine-> blocking){
            waitpid(pid,NULL,0);
        }
        if(debug == 1){
            fprintf(stderr,"Parent PID: [%d]\n",getpid());
            fprintf(stderr,"Command: [%s]\n",pCmdLine->arguments[0]);           
       }
    }
}
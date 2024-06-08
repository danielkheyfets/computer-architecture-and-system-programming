#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "LineParser.h"

void execute(cmdLine *pCmdLine);
int debug;

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
        execute(cmdL);
        freeCmdLines(cmdL);
        sleep(1); //for good printing only
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
            perror("error.");
            freeCmdLines(pCmdLine);
            exit(1);
        }
    }
    else
    {
       if(debug == 1){
        fprintf(stderr,"Parent PID: [%d]\n",getpid());
        fprintf(stderr,"Command: [%s]\n",pCmdLine->arguments[0]);           
       }
    }
    
}
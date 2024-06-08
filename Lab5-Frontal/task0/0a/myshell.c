#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "LineParser.h"

void execute(cmdLine *pCmdLine);

int main(int argc, char*argv[]){
    while(1){
        char dir[PATH_MAX];
        getcwd(dir,PATH_MAX);
        printf("%s\n",dir);
        char line[2048];
        fgets(line,2048,stdin);
        cmdLine* cmdL = parseCmdLines(line);
        if(strncmp("quit",cmdL->arguments[0],4) == 0){
            freeCmdLines(cmdL);
            break;
        }
        execute(cmdL);
        freeCmdLines(cmdL);
    }
    return 0;
}

void execute(cmdLine *pCmdLine){
    int res = execvp(pCmdLine->arguments[0],pCmdLine->arguments);
    if(res == -1){
        perror("error.");
        freeCmdLines(pCmdLine);
        exit(-1);
    }
}
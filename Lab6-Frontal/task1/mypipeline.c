#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int debug;

int main(int argc, char*argv[]){
    int mypipe[2];
    char* ls[3];
    ls[0] = "ls";
    ls[1] = "-l";
    ls[2] = NULL;
    char* tail[4];
    tail[0] = "tail";
    tail[1] = "-n";
    tail[2] = "2";
    tail[3] = NULL;
    if(argc>1 && strncmp("-d",argv[1],2)==0){
        debug = 1;
    }
    int successPipe = pipe(mypipe);
    if(successPipe == -1){
        perror("Cannot pipe");
        exit(1);
    }
    int id_fork_child1 = fork();
    if(id_fork_child1 == -1){
        _exit(1);
    }
    if(id_fork_child1 == 0){ //child 1
        if(debug){
            fprintf(stderr,"(child1>redirecting stdout to the write end of the pipe…)\n");
        }
        fclose(stdout);
        if(dup(mypipe[1]) == -1){
            perror("error ducplicating in child 1 process");
            _exit(1);
        }
        close(mypipe[1]);
        if(debug){
            fprintf(stderr,"(child1>going to execute cmd: …)\n");
        }
        execvp(ls[0],ls);
        _exit(1);
    }
    else{ //parent
        close(mypipe[1]);
        if(debug){
            fprintf(stderr,"(parent_process>closed the write end of the pipe)\n");
        }
        int id_fork_child2 = fork();
        if(debug){
            fprintf(stderr,"(parent_process> created process with id: %d)\n",id_fork_child2);
        }
        if(id_fork_child2 == -1){
            _exit(1);
        } 
        if(id_fork_child2 == 0){ //child 2
            if(debug){
                fprintf(stderr,"(child2>redirecting stdin to the read end of the pipe…)\n");
            }
            fclose(stdin);
            if(dup(mypipe[0]) == -1){
                perror("error ducplicating in child 2 process");
                _exit(1);
            }
            close(mypipe[0]);
            if(debug){
                fprintf(stderr,"(child2>going to execute cmd: …)\n");
            }
            execvp(tail[0],tail);
            _exit(1);
        }
        else{
            if(debug){
                fprintf(stderr,"(parent_process> closing the read end of the pipe...)\n");
            }
           close(mypipe[0]); 
        }
        if(debug){
            fprintf(stderr,"(parent_process>waiting for child processes to terminate…)\n");
        }
        waitpid(id_fork_child1,0,0);
        waitpid(id_fork_child2,0,0);
        if(debug){
            fprintf(stderr,"(parent_process>exiting…)\n");
        }
        exit(0);
    }
    return 0;
}

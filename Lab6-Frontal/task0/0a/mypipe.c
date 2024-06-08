#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>



int main (int argc, char **argv)
{
    char* msg = "hello";
    int mypipe[2];
    int j = pipe (mypipe);
    if(j == -1){
        perror ("Cannot pipe");
        exit (1); 
    }
    int value = fork();
    if (value == -1)
        {
        perror ("Cannot fork");
        exit (0);
        }
    else if (value == 0) //child proccess
        {
        close(mypipe[0]);
        write(mypipe[1], msg, 6);
        close(mypipe[1]);
        _exit (0);
        }
    else //parent process
        {
        char buf[256];
        close(mypipe[1]);
        read (mypipe[0], buf,256);
        printf ("Parent proccess: %s\n", buf);
        close(mypipe[0]);
        _exit (0);
        }
}
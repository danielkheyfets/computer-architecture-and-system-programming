#include "util.h"
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_READ 3
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern int system_call();
/* Function Declaration*/
void debug(int input, int output);
int rangeCheck(int key);
int changeChar(char myChar,const char* encrypted);

int main(int argc, char* argv[]){
    int input = STDIN;
    int output = STDOUT;
    int iFileModeIn = -1;
    int iFileModeOut = -1;
    int i;
    for(i=1; i<argc;i++){
        if(strncmp("-i",argv[i],2)==0){
            iFileModeIn = i;
            char *fileName = argv[i] +2;
            input = system_call(SYS_OPEN, fileName, 0, (0777 | 0644));
        }
        if(strncmp("-o",argv[i],2)==0){
            iFileModeOut = i;
            char *fileName = argv[i] +2; 
            output = system_call(SYS_OPEN, fileName, 1 | 64, (0777 | 0644));
        }
    }
    int c;
    while (system_call(SYS_READ, input,&c,1) != 0)
    {
        if(c>='A' && c<='Z')
            c+=32;
        system_call(SYS_WRITE, output,&c,1);  
    }
    if(iFileModeIn!=-1){
        system_call(SYS_CLOSE,input);
    }
    if(iFileModeOut!=-1){
        system_call(SYS_CLOSE,output);
    }
    return 0;
}
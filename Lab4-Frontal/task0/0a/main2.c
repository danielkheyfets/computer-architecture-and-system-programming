#include "util.h"

#define SYS_WRITE 4
#define STDOUT 1

extern int system_call();
int main (int argc , char* argv[], char* envp[])
{
    char* str = "hello world\n";
    system_call(SYS_WRITE,STDOUT, str,strlen(str));
    return 0;
}
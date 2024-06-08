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


int main (int argc , char* argv[], char* envp[])
{
  int input = STDIN;
  int output = STDOUT;
  int c;
  while(system_call(SYS_READ,input,&c,1)!=0)
  {
    if(c>='A' && c<='Z')
      c+=32;
    system_call(SYS_WRITE,output,&c,1);
  }
  return 0;
}

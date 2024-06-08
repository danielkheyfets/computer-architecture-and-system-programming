#include "util.h"

#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_LSEEK 19

extern int system_call();
int main (int argc , char* argv[], char* envp[])
{
  char* fileName=argv[1];
  char* newName = "Mira.\n\0";
  int fd=system_call(SYS_OPEN, fileName, 1, 0777);
  system_call(SYS_LSEEK,fd,657,0);
  system_call(SYS_WRITE,fd,newName,strlen(newName));
  system_call(SYS_CLOSE,fd);
  return 0;
}
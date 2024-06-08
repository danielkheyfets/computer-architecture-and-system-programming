#include "util.h"
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_READ 3
#define SYS_CLOSE 6
#define SYS_LSEEK 19
#define SYS_GETENDS 141
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define O_RDONLY 0
extern int system_call();

#define BUF_SIZE 8192

typedef struct ent{
    int inode;
    int offset;
    unsigned short len;
    char name[1];
} ent;

int main(int argc, char* argv[]){
    int fd;
    long nread;
    char buf[BUF_SIZE];
    struct ent *d;
    fd = system_call(SYS_OPEN,"./", O_RDONLY, (0777 | 0644));
    if(fd == -1){
        system_call(SYS_WRITE,STDOUT,"error from file read path.\n",27);
        system_call(1,0x55);
    }
    while(1) {
        nread = system_call(SYS_GETENDS,fd,buf,BUF_SIZE);
        if(nread == -1){
            char* str = "error read getends.\n";
            system_call(SYS_WRITE,STDOUT,str,strlen(str));
            system_call(1,0x55);
        }
        if(nread == 0) break;
        for(long bpos = 0 ; bpos<nread;){
            d = (struct ent*) (buf+bpos);
            system_call(SYS_WRITE,STDOUT,d->name,strlen(d->name));
            system_call(SYS_WRITE,STDOUT,"\n",1);
            bpos += d->len;
        }
    }
    system_call(SYS_CLOSE,fd);
}
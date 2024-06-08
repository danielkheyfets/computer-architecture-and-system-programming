#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



int main(int argc, char **argv){
    unsigned char virus[617];
    FILE* elfexec = fopen("ELFexec2long","r");
    fseek(elfexec,4080,SEEK_SET);
    unsigned char symbol;
    fread(&virus,1,617,elfexec);
     fclose(elfexec); 
     int vSize = 617;
     char signature[18] = {0x69, 0x02,0x4d,0x79, 0x20, 0x76, 0x69 ,0x72 ,0x75 ,0x73 ,0x20 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00 ,0x00};
     FILE* fd = fopen("signatures-L","r+");
     fseek(fd,0,SEEK_END);
     fwrite(signature,1,18,fd);
     fseek(fd,0,SEEK_END);
     fwrite(&virus,1,vSize,fd);
     fclose(fd);
    return 0;
}

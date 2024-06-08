#include <stdio.h>
#include <stdlib.h>


void PrintHex(unsigned char *buffer, int length){
    for (size_t i = 0; i < length-1; i++)
        printf("%02X ", buffer[i]);
    printf("%02X\n",buffer[length-1]);
}

int main(int argc, char **argv) {
    FILE* fp = fopen(argv[1],"r");
    if(fp == NULL){
         printf("Error opening file\n");
        exit(1);
    }
    unsigned char buf[1024];
    int count = fread(&buf,sizeof(unsigned char),1024,fp);
    fclose(fp);
    PrintHex(buf,count);
    return 0;
}
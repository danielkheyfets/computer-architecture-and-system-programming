#include <stdio.h>
#include <stdlib.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

void PrintHex(unsigned char *buffer, int length){
    for (size_t i = 0; i < length-1; i++)
        printf("%02X ", buffer[i]);
    printf("%02X\n",buffer[length-1]);
}

virus* readVirus(FILE* fp){
    virus* v = malloc(sizeof(struct virus));
    if(fread(v,1,18,fp)==0){
        free(v);
        return NULL;
    }
    v->sig = malloc(sizeof(unsigned char)*v->SigSize);
    fread(v->sig,sizeof(unsigned char),v->SigSize,fp);
    return v;
}

void printVirus(virus* virus, FILE* output){
    fprintf(output,"Virus Name: %s\n",virus->virusName);
    fprintf(output,"Virus Size: %d\n",virus->SigSize);
    fprintf(output,"signature:\n");
    PrintHex(virus->sig,virus->SigSize);
}

int main(int argc, char** argv){
    FILE* fp = fopen(argv[1],"r");
    if(fp == NULL){
        printf("Error opening file\n");
        exit(1);
    }
    fseek(fp,4,SEEK_SET);
    while (1)
    {
        virus* v = readVirus(fp);
        if(v==NULL) break;
        printVirus(v,stdout);
        free(v->sig);
        free(v);
        printf("\n");
    }
    fclose(fp);
    return 0;
}
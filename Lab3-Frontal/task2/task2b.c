#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** g_argv;

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;


typedef struct link link;
 
struct link {
    link *nextVirus;
    virus *vir;
};

struct fun_desc {
    char *name;
    void (*fun)(link**);
};

/*FUNCTION DEC*/

virus* readVirus(FILE*);
void printVirus(virus* virus, FILE* output);

 /* Print the data of every link in list to the given stream. Each item followed by a newline character. */
void list_print(link *virus_list, FILE*stream){
    while(virus_list!= NULL){
        printVirus(virus_list->vir,stream);
        fprintf(stream,"\n");
        virus_list = virus_list->nextVirus;
    }
}
    
/* Add a new link with the given data to the list 
(either at the end or the beginning, depending on what your TA tells you),
and return a pointer to the list (i.e., the first link in the list).
If the list is null - create a new entry and return a pointer to the entry. */
link* list_append(link* virus_list, virus* data){
    link* newLink = malloc(sizeof(struct link));
    newLink->nextVirus = NULL;
    newLink->vir = data;
    if(virus_list==NULL) return newLink;
    link* beginOfList = virus_list;
    link* last_in_list;
    while(virus_list!=NULL){
        last_in_list= virus_list;
        virus_list=virus_list->nextVirus;
    }
    last_in_list->nextVirus=newLink;
    return beginOfList;
}

 /* Free the memory allocated by the list. */
void list_free(link *virus_list){
    link* tmp;
    while (virus_list != NULL)
    {
    tmp = virus_list;
    virus_list = virus_list->nextVirus;
    free(tmp->vir->sig); //free signature unsigned char allocated
    free(tmp->vir); // free the virus allocated
    free(tmp); // free the link allocated
    }
}

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

void load(link** list){
    char filename[1024];
    printf("Enter file name:\n");
    fgets(filename,sizeof(filename),stdin);
    filename[strcspn(filename, "\n")] = 0; //remove the \n char at the end for reading the file name
    FILE* fp = fopen(filename,"r");
    if(fp == NULL){
        return;
    }
    fseek(fp,4,SEEK_SET);
    while (1){
        virus* v = readVirus(fp);
        if(v == NULL) break;
        *list = list_append(*list,v);
    }
    fclose(fp);
}


void print(link** list){
    if(*list !=NULL){
        list_print(*list,stdout);
    }
}
void detect_virus(char *buffer, unsigned int size, link *virus_list, FILE* output){
    while (virus_list!=NULL){
        virus* v = virus_list->vir;
        for(int i=0; i<size; i++){
            char* suspected = &buffer[i];
            int res = memcmp(suspected,v->sig,v->SigSize);
            if(res==0){
                fprintf(output,"starting byte location: %d\n", i);
                fprintf(output,"Virus Name: %s\n", v->virusName);
                fprintf(output,"Size of virus sig: %d\n", v->SigSize);
                fprintf(output,"\n");
                break;
            }
        }
        virus_list = virus_list->nextVirus;
    }    
}

void detect_v(link** respected_list){
    FILE* fp = fopen(g_argv[1],"r"); //g_argv global varible
    if(fp==NULL){
        return;
    }
    char suspected[10240];
    int size_b = fread(&suspected,sizeof(char),10240,fp);
    int size_S = sizeof(suspected);
    int size = (size_S < size_b)? size_S : size_b;
    detect_virus(suspected,size,*respected_list,stdout);
}

void kill_virus(char *fileName, int signitureOffset, int signitureSize){
    FILE* fp = fopen(g_argv[1],"r+");
    if(fp == NULL){
        return;
    }
    unsigned char fix[1] = {0x90};
    unsigned char s[signitureSize];
    for (int i = 0; i <signitureSize ; ++i) {
        strncat(s,fix,1);
    }
    fseek(fp,signitureOffset,SEEK_SET);
    fwrite(s,1,signitureSize,fp);
    fclose(fp);
}

void fix_f(link** v_list){
    char signitureOffset[256];
    int index;
    printf("Offset:\n");
    fgets(signitureOffset,sizeof(signitureOffset),stdin);
    sscanf(signitureOffset,"%d",&index);
    char signitureSize[256]; 
    int sigSize;
    printf("Signatures size:\n");
    fgets(signitureSize,sizeof(signitureSize),stdin);
    sscanf(signitureSize,"%d",&sigSize);
    kill_virus(g_argv[1],index,sigSize);
}

int main(int argc, char** argv){
    g_argv = argv;
    struct fun_desc menu[] = {{ "Load signatures", load }, { "Print signatures", print },{ "Detect viruses", detect_v },{"Fix file", fix_f}, {NULL,NULL}};
    int numOfFunctions = ((sizeof(menu)/sizeof(struct fun_desc))-1); //(48/8 = number of elements with {Null,NUll})
    int option;
    link* list = NULL;
    while (1){
        printf("Please choose an option\n");
        for(int i=0; i<numOfFunctions;i++)
            printf("%d) %s\n",i+1,menu[i].name);
        //read from user the option
        char buffer[1024]; //check what size to put in here
        fgets(buffer,numOfFunctions,stdin);
        int success = sscanf(buffer,"%d",&option);
        printf("Option: %d\n",option);
        if(success<=0 || (option<1 || option >numOfFunctions)){
            printf("Not within bounds\n");
            if(list != NULL){
                list_free(list);
            }
            exit(0);
        }
        menu[option-1].fun(&list);
        printf("DONE.\n\n");
    }
}
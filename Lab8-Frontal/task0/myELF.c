#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define  NAME_LEN  128
#define  BUF_SZ    10000

/* FUNCTION DECLORATION*/
typedef struct {
    char debug_mode;
    char file_name[NAME_LEN];
    int currFD;
    void *map_start; /* will point to the start of the memory mapped file */
    struct stat fd_stat; /* this is needed to  the size of the file */
    Elf32_Ehdr *header; /* this will point to the header structure */
    int num_of_section_headers;
} state;

struct fun_desc {
  char *name;
  void (*fun)(state* s);
};
void Toggle_Debug_Mode(state* s);
void Examine_ELF_File(state* s);
void Print_Section_Names(state* s);
void Print_Symbols(state* s);
void Relocation_Tables(state* s);
void printDebugState (state* s);
void Set_File_Name(state* s);
void Quit(state* s);
 /*
Choose action:
0-Toggle Debug Mode
1-Examine ELF File
2-Print Section Names
3-Print Symbols
4-Relocation Tables
5-Quit
*/
int main(int argc, char **argv){
   struct fun_desc menu[] = { { "Toggle Debug Mode", Toggle_Debug_Mode }, { "Examine ELF File",  Examine_ELF_File},{"Print Section Names" , Print_Section_Names},
                                {"Print Symbols",Print_Symbols}, {"Relocation Tables",Relocation_Tables},{"Quit",Quit }, { NULL, NULL } };
   int numOfFunctions = ((sizeof(menu)/sizeof(struct fun_desc))-1); //(48/8 = number of elements with {Null,NUll})
   int option;
   state* s = malloc(sizeof(state));
   while (1)
   {
       if(s->debug_mode){
           printDebugState(s);
       }
        printf("Choose action:\n");
        for(int i=0; i<numOfFunctions;i++)
            printf("%d-%s\n",i,menu[i].name);

        
        char buffer[1024]; //check what size to put in here
        fgets(buffer,numOfFunctions-1,stdin);
        int success = sscanf(buffer,"%d",&option);
        printf("Option: %d\n",option);
        if(success<=0 || (option<0 || option >numOfFunctions-1)){
            printf("Not within bounds\n");
            exit(0);
        }
        printf("Within bounds\n");
        menu[option].fun(s);
        printf("DONE.\n\n");      
   }
   
}


void Toggle_Debug_Mode(state* s){
    if(!s->debug_mode){ //if its off then turn on
        s->debug_mode = 1;
        printf("Debug flag now on\n");
    }
    else{ // its on then turn off
        s->debug_mode = 0;
        printf("Debug flag now off\n");
    }
}

void printDebugState(state* s){
    // printf("-----debug info----\n");
    // printf("unit_size: %d\n",s->unit_size);
    // printf("file_name: %s\n",s->file_name);
    // printf("mem_count: %d\n",s->mem_count);
    // printf("-------------------\n");
}

void Quit(state* s){
    if(s->debug_mode)
        printf("quitting\n");
    munmap(s->map_start,s->fd_stat.st_size);
    close(s->currFD);
    free(s);
    exit(0);
}


void Examine_ELF_File(state* s){
     Set_File_Name(s);
     s->currFD = open(s->file_name,O_RDONLY);
     if(s->currFD == -1){
        perror("ERROR: ");
        return;
    }
    if(fstat(s->currFD,&s->fd_stat)!=0){
        perror("error in open");
        munmap(s->map_start,s->fd_stat.st_size);
        close(s->currFD);
        s->currFD == -1;
        return;
    }
    if((s->map_start = mmap( NULL, s->fd_stat.st_size,PROT_READ , MAP_SHARED, s->currFD, 0))==MAP_FAILED){
        perror("ERROR: ");
        close(s->currFD);
        s->currFD == -1;
        return;
    }
    s->header = (Elf32_Ehdr*) s->map_start;
    s->num_of_section_headers = s->header->e_shnum;
    printf("Magic Numbers (1,2,3):\t\t %02X , %02X , %02X\n",s->header->e_ident[0],s->header->e_ident[1],s->header->e_ident[2]);
    printf("Data encoding scheme:\t\t %02X\n",s->header->e_ident[EI_DATA]);
    printf("Entry point address:\t\t %#02x\n",s->header->e_entry);
    printf("Start of section headers:\t %d\n",s->header->e_shoff);
    printf("Number of section headers:\t %d\n",s->num_of_section_headers);
    printf("Size of section headers:\t %d\n",s->header->e_shentsize);
    printf("Start of program headers:\t %d\n",s->header->e_phoff);
    printf("Number of program headers:\t %d\n",s->header->e_phnum);
    printf("Size of program headers:\t %d\n",s->header->e_phentsize);
    

    
}
void Print_Section_Names(state* s){
     printf("not implemented yet\n"); 
}
void Print_Symbols(state* s){
     printf("not implemented yet\n"); 
}
void Relocation_Tables(state* s){
     printf("not implemented yet\n"); 
}

void Set_File_Name(state* s){
    printf("file name: ");
    fgets(s->file_name,NAME_LEN,stdin);
    s->file_name[strcspn(s->file_name, "\n")] = 0;
    if(s->debug_mode)
        printf("Debug: file name set to %s\n",s->file_name);   
}


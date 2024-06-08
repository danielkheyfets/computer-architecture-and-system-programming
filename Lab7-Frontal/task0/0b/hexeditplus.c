#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define  NAME_LEN  128
#define  BUF_SZ    10000

/* FUNCTION DECLORATION*/
typedef struct {
  char debug_mode;
  char file_name[NAME_LEN];
  int unit_size;
  unsigned char mem_buf[BUF_SZ];
  size_t mem_count;
  /*
   .
   .
   Any additional fields you deem necessary
  */
} state;

struct fun_desc {
  char *name;
  void (*fun)(state* s);
};
void Toggle_Debug_Mode(state* s);
void Set_File_Name(state* s);
void Set_Unit_Size(state* s);
void Load_Into_Memory(state* s);
void Memory_Display(state* s);
void Save_Into_File(state* s);
void Memory_Modify(state* s);
void printDebugState(state* s);
void Quit(state* s);
 /*
Choose action:
0-Toggle Debug Mode
1-Set File Name
2-Set Unit Size
3-Load Into Memory
4-Memory Display
5-Save Into File
6-Memory Modify
7-Quit
*/
int main(int argc, char **argv){
   struct fun_desc menu[] = { { "Toggle Debug Mode", Toggle_Debug_Mode }, { "Set File Name",  Set_File_Name},{"Set Unit Size" , Set_Unit_Size},
                                {"Load Into Memory",Load_Into_Memory }, {"Memory Display",Memory_Display },{"Save Into File", Save_Into_File},
                                {"Memory Modify",Memory_Modify },{"Quit",Quit }, { NULL, NULL } };
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
void Set_File_Name(state* s){
    printf("name: ");
    fgets(s->file_name,NAME_LEN,stdin);
    s->file_name[strcspn(s->file_name, "\n")] = 0;
    if(s->debug_mode)
        printf("Debug: file name set to %s\n",s->file_name);   
}

void Set_Unit_Size(state* s){
    int size;
    char in[128]; //check what size to put in here
    fgets(in,128,stdin);
    sscanf(in,"%d",&size);
    if(size == 1 || size == 2 || size == 4){
        s->unit_size = size;
        if(s->debug_mode){
            printf("Debug: set size to %d\n",s->unit_size);   
        }
    }
    else{
        printf("Invalid unit_size inserted - not 1,2,4\n");  
    }
}
void printDebugState(state* s){
    printf("-----debug info----\n");
    printf("unit_size: %d\n",s->unit_size);
    printf("file_name: %s\n",s->file_name);
    printf("mem_count: %d\n",s->mem_count);
    printf("-------------------\n");
}

void Quit(state* s){
    if(s->debug_mode)
        printf("quitting\n");
    free(s);
    exit(0);
}


void Load_Into_Memory(state* s){
     printf("not implemented yet\n"); 
}
void Memory_Display(state* s){
     printf("not implemented yet\n"); 
}
void Save_Into_File(state* s){
     printf("not implemented yet\n"); 
}
void Memory_Modify(state* s){
     printf("not implemented yet\n"); 
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define  NAME_LEN  128
#define  BUF_SZ    10000
#define KB(i) ((i)*1<<10)
#define INT sizeof(int)
#define SHORT sizeof(short)
#define BYTE sizeof(char)

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
int getIntFromUser();
int read_units_to_memory(FILE* input, char* buffer,int unit_size, int count);
void print_units(FILE* output, unsigned char* buffer,int unit_size, int count);
char* unit_to_format(int unit,int hex);
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
        printf("\n\n");      
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
    int size = getIntFromUser();
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
    if(strcmp(s->file_name,"") == 0){
        printf("file name is empty.\n");
        return;
    }
    FILE* file = fopen(s->file_name,"r");
    if(file == NULL){
        printf("error open file.\n");
        return;
    }
    printf("Please enter <location> <length>\n");
    int location;
    int length;
    char in[128]; //check what size to put in here
    fgets(in,128,stdin);
    sscanf(strtok(in," "),"%X",&location);
    sscanf(strtok(NULL," "),"%d",&length);
    if(s->debug_mode){
      printf("file name: %s\n",s->file_name);
      printf("location: %X\n",location);
      printf("length: %d\n",length);  
    }
    fseek(file,location,SEEK_SET);
    int read= read_units_to_memory(file,s->mem_buf,s->unit_size,(s->unit_size*length));
    s->mem_count =read;
    fclose(file);
    printf("Loaded %d units into memory\n",read); 
}

void Memory_Display(state* s){
    printf("Enter <address> <length>\n");
    int address;
    int length;
    char in[128]; //check what size to put in here
    fgets(in,128,stdin);
    sscanf(strtok(in," "),"%X",&address);
    sscanf(strtok(NULL," "),"%d",&length);
    if(address == 0){
        print_units(stdout,s->mem_buf,s->unit_size,length);
    }
    else{
        print_units(stdout,s->mem_buf+address,s->unit_size,length);      
    }
}
void Save_Into_File(state* s){
    printf("Please enter <source-address> <target-location> <length>\n");
    long source_address;
    int target_location;
    int length;
    char in[128]; //check what size to put in here
    fgets(in,128,stdin);
    sscanf(in,"%lx %x %d",&source_address,&target_location,&length);
    FILE* file = fopen(s->file_name,"r+");
    if(file == NULL){
        if(s->debug_mode)
            printf("error open file.\n");
        return;
    }
    fseek(file, 0L, SEEK_END);
    int file_size = ftell(file);
    if(target_location+length > file_size){
        printf("offset of target is greater then file size.\n");
        fclose(file);
        return;
    }
    fseek(file,0,SEEK_SET);
    fseek(file,target_location,SEEK_SET);
    unsigned char* tmp = source_address == 0 ? s->mem_buf : (unsigned char*)source_address;
    fwrite(tmp,s->unit_size,length,file);
    fclose(file);
}
void Memory_Modify(state* s){
    printf("Please enter <location> <val>\n");
    int location;
    int val;
    char in[128]; //check what size to put in here
    fgets(in,128,stdin);
    sscanf(in,"%x %x",&location,&val);
    if(s->debug_mode){
        printf("location: %X\n",location);
        printf("val: %X\n",val);
    }
    FILE* file = fopen(s->file_name,"r+");
    if(file == NULL){
        if(s->debug_mode)
            printf("error open file.\n");
        return;
    }
    fseek(file,location,SEEK_SET);
    fwrite(&val,s->unit_size,1,file);
    fclose(file);
}


int getIntFromUser(){
    int num;
    char in[128]; //check what size to put in here
    fgets(in,128,stdin);
    sscanf(in,"%d",&num);
    return num;
}

/* Reads units from file */
int read_units_to_memory(FILE* input, char* buffer,int unit_size, int count) {
    return fread(buffer, unit_size, count, input); 
}

/* Prints the buffer to screen by converting it to text with printf */
void print_units(FILE* output, unsigned char* buffer,int unit_size, int count) {
    unsigned char* end = buffer + unit_size*count;
    unsigned char* start = buffer;
    fprintf(output,"Hexadecimal\n===========\n");
    while (buffer < end) {
        //print Hexadecimal value 
        int var = *((int*)(buffer));
        fprintf(output, unit_to_format(unit_size,1), var);
        buffer += unit_size;
    }
    end = start + unit_size*count;
    fprintf(output,"\nDecimal\n===========\n");
    while (start < end) {
        //print Deciaml value
        int var = *((int*)(start));
        fprintf(output, unit_to_format(unit_size,0), var);
        start += unit_size;
    }
}

char* unit_to_format(int unit,int hex) {
    char* hexa[] = {"%hhX\n", "%hX\n", "No such unit", "%X\n"};
    char* decimal[] = {"%#hhd\n", "%#hd\n", "No such unit", "%#d\n"};
    return hex == 1? hexa[unit-1] : decimal[unit-1];
}  

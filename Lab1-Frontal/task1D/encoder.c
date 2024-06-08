#include <stdio.h>
#include <string.h>

/* Function Declaration*/
void debug(FILE* input, FILE* printOut, const char* task, const char* encrypted);
int rangeCheck(int key);
void encrypt(FILE* input, FILE* printOut, const char* encrypted);
int changeChar(char myChar,const char* encrypted);


int main(int argc, char* argv[]){
    FILE* input = stdin;
    FILE* printOut = stdout;
    int iFileModeIn = -1;
    int encryptedMode = -1;
    int debugMode = -1;
    //check for what the arguments are
    for(int i=1; i<argc;i++){
        if(strncmp("-D",argv[i],2)==0)
            debugMode = i;
        if((strncmp("+e",argv[i],2)==0) || (strncmp("-e",argv[i],2)==0))
            encryptedMode = i;
        if(strncmp("-i",argv[i],2)==0){
            iFileModeIn = i;
            char *fileName = argv[i] +2; //-iinput
            input = fopen(fileName,"r");
            if(input == NULL){
            fprintf(stderr, "error, file does not exist in directory\n");
            return-1;     
            }
        }
    }
    //case where +e1 & -D
    if(encryptedMode!= -1 && debugMode!=-1){
        char encrypted[3];
        strcpy(encrypted,argv[encryptedMode]);
        if(rangeCheck(encrypted[2]) == -1){} // will print error
        else
            debug(input,printOut,"ed", encrypted);
    }
    //case of only +e1
    else if(encryptedMode!=-1){
        char encrypted[3];
         strcpy(encrypted,argv[encryptedMode]);
         if(rangeCheck(encrypted[2]) == -1){}
         else
            encrypt(input,printOut,encrypted);
    }

    else if(debugMode!=-1){ //-D only
        debug(input,printOut,"d","");
    }
    //no arguments therefor only lowercase
    else 
    {
        int c = fgetc(input); //task1a
        do{
            if(c>='A' && c<='Z')
                c+=32;
            fputc(c,printOut);
            c = fgetc(input);
        }while(c!=EOF);
    }
    //close the file i\o
    if(iFileModeIn!=-1){
        fputc('\n',printOut);
        fclose(input);
    }
    return 0;
}





/*Imp Of Function*/

void debug(FILE* input, FILE* printOut, const char* task, const char* encrypted){
    FILE* err = stderr;
    fputs("-D\n",err);
    int c;
    int counter = 0;
    do{
        c = fgetc(input);
        if(c==-1) // case ^D to stop the program
            break;
        int before = c;
        int after = c;
        if(task[0] == 'e' && task[1] == 'd'){
            after = changeChar(after,encrypted);
            if(c!='\n' && encrypted[2]!= '0') //check if needed--------------
                counter++;
        }
        else if(c>='A' && c<='Z'){
            after= c+32;
            counter++;
        }
        //case where needed to print error debug and reset for next command
        if(c==10){ //  '\n' -> end of reading char
            fputs("\nthe number of letters: ", err);
            fprintf(err, "%d\n\n", counter);
            counter=0;
            fputc('\n',printOut);
            continue;
        }
        fputc(after,printOut);
        fprintf(err,"%d\t%d\n", before, after);
    }while(c!=EOF);
}


int rangeCheck(int key){
    if(!((key>='0' && key<='9')||(key>='A' && key<='F'))){
        printf("%s", "outside the permissible range");
        return -1;
    }
    return 1;
}


void encrypt(FILE* input, FILE* printOut, const char* encrypted){
    int myChar = 0;
    while(myChar!=EOF){
        myChar = fgetc(input);
        if(myChar == -1) //case of ^D
            break;
        if(myChar == '\n'){ //case end
            fputc('\n',printOut);
            continue;
        }
        //the rangeCheck was already checked in main
        myChar = changeChar(myChar,encrypted);
        fputc(myChar,printOut);       
    }
}

int changeChar(char myChar, const char* encrypted){
    int c = myChar;
    int key = encrypted[2];
    char operator = encrypted[0];
    if(key>='A' && key<='F')
        key = (key-65)+10;
    if(key>='0' && key<='9')
        key = (key-48);
    if(operator == '+')
        c+=key;
    else
        c-=key;
    return c;
}
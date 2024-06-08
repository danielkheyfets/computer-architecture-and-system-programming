#include <stdio.h>


/* Function Declaration*/
void debug(FILE* input, FILE* printOut);


int main(int argc, char* argv[]){
    FILE* input = stdin;
    FILE* printOut = stdout;
    if((argc-1)>=1){
        debug(input,printOut);
    }
    int c = fgetc(input); //task1a
    do{
        if(c>='A' && c<='Z')
            c+=32;
        fputc(c,printOut);
        c = fgetc(input);
    }while(c!=EOF);
    return 0;
}





/*Imp Of Function*/

void debug(FILE* input, FILE* printOut){
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
        if(c>='A' && c<='Z'){
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
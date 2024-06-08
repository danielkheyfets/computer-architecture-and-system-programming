#include <stdio.h>

int main(){
    FILE* input = stdin;
    FILE* printOut = stdout;
    int c = fgetc(input);
    do{
        if(c>='A' && c<='Z')
            c+=32;
        fputc(c,printOut);
        c = fgetc(input);
    }while(c!=EOF);
    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* FUNCTION DECLORATION*/
char encrypt(char c); /* Gets a char c and returns its encrypted form by adding 3 to its value. 
          If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c); /* Gets a char c and returns its decrypted form by reducing 3 to its value. 
            If c is not between 0x20 and 0x7E it is returned unchanged */
char cprt(char c); /* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
                    by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
                    the value of c unchanged. */
char my_get(char c); /* Ignores c, reads and returns a character from stdin using fgetc. */

struct fun_desc {
  char *name;
  char (*fun)(char);
};


/* IMP */
char encrypt(char c){
    if(c >=0x20 && c<=0x7E)
        c+=3;
    return c;
}

char decrypt(char c){
    if(c >=0x20 && c<=0x7E)
        c-=3;
    return c;
}

char cprt(char c){
    if(c >=0x20 && c<=0x7E)
       printf("%c\n",c);
    else
        printf("%c\n",'.');
    return c;
}

char my_get(char c){
    return fgetc(stdin);
}

char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for (int i = 0; i < array_length; i++)
  {
      mapped_array[i]=f(array[i]);
  }  
  return mapped_array;
}
 /*
0) Censor
1) Encrypt
2) Decrypt
3) Print string
4) Get string
*/
int main(int argc, char **argv){
   char* carray = (char*) malloc(5);
   struct fun_desc menu[] = { { "Censor", censor }, { "Encrypt", encrypt },{"Decrypt" , decrypt},{"Print string", cprt}, {"Get string", my_get}, { NULL, NULL } };
   int numOfFunctions = ((sizeof(menu)/sizeof(struct fun_desc))-1); //(48/8 = number of elements with {Null,NUll})
   int option;
   while (1)
   {
        printf("Please choose a function:\n");
        for(int i=0; i<numOfFunctions;i++)
            printf("%d) %s\n",i,menu[i].name);
        char buffer[1024]; //check what size to put in here
        fgets(buffer,numOfFunctions-1,stdin);
        int success = sscanf(buffer,"%d",&option);
        printf("Option: %d\n",option);
        if(success<=0 || (option<0 || option >numOfFunctions-1)){
            printf("Not within bounds\n");
            free(carray);
            exit(0);
        }
        printf("Within bounds\n");
        carray = map(carray,5,menu[option].fun);
        printf("DONE.\n\n");      
   }
   
}
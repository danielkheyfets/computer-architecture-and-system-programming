#include <stdio.h>
int digit_cnt(char* str){
    int count=0;
    int i=0;
    while(str[i]!=0){
        if(str[i] <= 57 && str[i] >= 48)
            count++;
        i++;
    }
    return count;
}

int main(int argc, char **argv){
    if(argc >1)
        printf("The number of digits in the string is: %d\n",digit_cnt(argv[1])); 
   return 0;
}



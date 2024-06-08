#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int addr5; 
int addr6; 
//numerical order of int -> address increments by 4

int foo(); 
void point_at(void *p);
void foo1(); 
void foo2();

int main (int argc, char** argv){
    int addr2; 
    int addr3; 
    char* yos="ree"; 
    int * addr4 = (int*)(malloc(50)); 
    printf("- &addr2: %p [STACK]\n",&addr2);
    printf("- &addr3: %p [STACK]\n",&addr3);
    printf("- foo: %p [TEXT]\n",foo);
    printf("- &addr5:%p[BSS]\n",&addr5); 
    
	point_at(&addr5);
	
    printf("- &addr6: %p[BSS]\n",&addr6); 
    printf("- yos: %p[POINTER IN STACK AND 'yossi' IN TEXT]\n",yos);  
    printf("- addr4: %p [HEAP]\n",addr4); // ->address in heap allocation
    printf("- &addr4: %p[STACK]\n",&addr4); // -> address of pointer in stack
    
    printf("- &foo1: %p[TEXT]\n" ,&foo1);
    printf("- &foo2: %p[TEXT]\n" ,&foo2);
    printf("- &foo2 - &foo1: %d\n" ,&foo2 - &foo1); //43 



    

    printf("---------task 1c---------\n");
    int iarray[3];
    double darray[3];
    char carray[3];
    int* iptrarray[3];
    printf("- &iarray: %p\n",iarray); //arr is a pointer to the first element of the array.So, if we move arr by 1 position it will point the second element.
    printf("- &iarray+1: %p\n",iarray+1);
    //&arr is a pointer to an entire array. So, if we move &arr by 1 position it will point the next block of [x = the size of array init] elements.


    printf("- &darray: %p\n",darray); //size of double is 8 bytes
    printf("- &darray+1: %p\n",darray+1);

    printf("- &carray: %p\n",carray); //size of char is 1 therefore carray is in address 1 higher than carray+1 (carray[1])
    printf("- &carray+1: %p\n",carray+1);
    printf("sizeof char - > %d\n", sizeof(char)); //size of char is 1 byte
    printf("- &iptrarray: %p\n",iptrarray); //size of pointer is 4 bytes
    printf("- &iptrarray+1: %p\n",iptrarray+1);

    printf("---------task 1d---------\n");
    int iarray1[] = {1,2,3};
    char carray1[] = {'a','b','c'};
    int* iarrayPtr = iarray1;
    char* carrayPtr = carray1;
    printf("iarray1 : {%d,%d,%d}\n",iarrayPtr[0],iarrayPtr[1],iarrayPtr[2]);
    printf("carray1 : {%c,%c,%c}\n",carrayPtr[0],carrayPtr[1],carrayPtr[2]);
    return 0;
}

int foo(){
    return -1;
}

void point_at(void *p){
    int local; 
	static int addr0 = 2;
    static int addr1;


    long dist1 = (size_t)&addr6 - (size_t)p; // -4 -> [BSS-BSS]memory allocates from high to low therefore the distance (addr5-addr6) is negative
    long dist2 = (size_t)&local - (size_t)p; // [DATA[S]-BSS]
    long dist3 = (size_t)&foo - (size_t)p;// [TEXT-BSS]global address higher than (text segment) address

    printf("- &local: %p [STACK]\n" ,&local);
    printf("- &p->addr5: %p [STACK]\n" ,&p);
    printf("sizeof -> long : %d\n", sizeof(long)); // size 4 bytes
    printf("sizeof -> int : %d\n", sizeof(int)); // size 4 bytes
    printf("sizeof -> double : %d\n", sizeof(double)); // size 8 bytes
    
    printf("dist1: (size_t)&addr6 - (size_t)p: %ld\n",dist1);
    printf("dist2: (size_t)&local - (size_t)p: %ld\n",dist2);
    printf("dist3: (size_t)&foo - (size_t)p:  %ld\n",dist3);
	
	printf("- addr0: %p [BSS]\n", & addr0);
    printf("- addr1: %p [BSS]\n",&addr1);
    

}

void foo1 (){    
    printf("foo1\n"); 
}

void foo2 (){    
    printf("foo2\n");    
}

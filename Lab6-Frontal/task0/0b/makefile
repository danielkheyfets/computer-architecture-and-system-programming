all: exec

#create executable file named 'myshell'
exec: mypipe lineP
	gcc -g -Wall -m32 mypipe.o LineParser.o -o mypipe

#create .o file compilation
mypipe: mypipe.c
	gcc -g -Wall -m32 -c  mypipe.c -o mypipe.o 

#create .o file compilation
lineP: LineParser.c
	gcc -g -Wall -m32 -c  LineParser.c -o LineParser.o

.PHONY: clean
clean:
	rm -rf ./*.o mypipe
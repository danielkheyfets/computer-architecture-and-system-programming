all: exec

#create executable file named 'myshell'
exec: myshell lineP
	gcc -g -Wall -m32 myshell.o LineParser.o -o myshell

#create .o file compilation
myshell: myshell.c
	gcc -g -Wall -m32 -c  myshell.c -o myshell.o 

#create .o file compilation
lineP: LineParser.c
	gcc -g -Wall -m32 -c  LineParser.c -o LineParser.o

.PHONY: clean
clean:
	rm -rf ./*.o myshell
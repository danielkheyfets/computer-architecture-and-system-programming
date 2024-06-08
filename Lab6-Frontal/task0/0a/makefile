all: exec

#create executable file named 'mypipe'
exec: mypipe
	gcc -g -Wall -m32 mypipe.o -o mypipe

#create .o file compilation
mypipe: mypipe.c
	gcc -g -Wall -m32 -c  mypipe.c -o mypipe.o 

.PHONY: clean
clean:
	rm -rf ./*.o mypipe

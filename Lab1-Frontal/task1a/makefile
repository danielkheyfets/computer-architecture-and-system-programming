all: encoder

#create executable file named 'encoder'
encoder: task1a
	gcc -g -m32 task1a.o -o encoder
	rm task1a.o

#create .o file compilation
task1a: task1a.c
	gcc -g -m32 -c -o task1a.o task1a.c

.PHONY: clean
clean:
	rm -rf encoder
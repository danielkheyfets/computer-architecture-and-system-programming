all: encoder

#create executable file named 'encoder'
encoder: task1b
	gcc -g -m32 task1b.o -o encoder
	rm task1b.o

#create .o file compilation
task1b: task1b.c
	gcc -g -m32 -c -o task1b.o task1b.c

.PHONY: clean
clean:
	rm -rf encoder
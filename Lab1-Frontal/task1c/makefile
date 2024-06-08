all: encoder

#create executable file named 'encoder'
encoder: task1c
	gcc -g -m32 task1c.o -o encoder
	rm task1c.o

#create .o file compilation
task1c: task1c.c
	gcc -g -m32 -c -o task1c.o task1c.c

.PHONY: clean
clean:
	rm -rf encoder
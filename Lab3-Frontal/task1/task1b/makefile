all: t1

#create executable file named 'encoder'
t1: t1b
	gcc -g -m32 task1b.o -o task1b
	rm task1b.o

#create .o file compilation
t1b: task1b.c
	gcc -g -m32 -c -o task1b.o task1b.c

.PHONY: clean
clean:
	rm -rf task1b
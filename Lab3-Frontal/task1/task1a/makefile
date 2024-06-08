all: t1

#create executable file named 'encoder'
t1: t1a
	gcc -g -m32 task1a.o -o task1a
	rm task1a.o

#create .o file compilation
t1a: task1a.c
	gcc -g -m32 -c -o task1a.o task1a.c

.PHONY: clean
clean:
	rm -rf task1a
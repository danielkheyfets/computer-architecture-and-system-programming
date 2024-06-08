all: t1

#create executable file named 'encoder'
t1: t1c
	gcc -g -m32 task1c.o -o task1c
	rm task1c.o

#create .o file compilation
t1c: task1c.c
	gcc -g -m32 -c -o task1c.o task1c.c

.PHONY: clean
clean:
	rm -rf task1c
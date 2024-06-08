all: t2

#create executable file named 'encoder'
t2: t2b
	gcc -g -m32 task2b.o -o task2b
	rm task2b.o

#create .o file compilation
t2b: task2b.c
	gcc -g -m32 -c -o task2b.o task2b.c

.PHONY: clean
clean:
	rm -rf task2b
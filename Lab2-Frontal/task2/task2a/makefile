all: exec

t2a: task2a.c
	gcc -g -m32 -c -o task2a.o task2a.c

exec: t2a
	gcc -g -m32  task2a.o -o task2a
	
.PHONY: clean
clean:
	rm -rf ./*.o task2a

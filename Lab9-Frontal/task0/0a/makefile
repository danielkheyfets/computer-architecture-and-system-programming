all: exec

exec: x
	gcc -g -m32 0a.o -o task0a
	rm 0a.o

x: 0a.c
	gcc -g -m32 -c -o 0a.o 0a.c

.PHONY: clean
clean:
	rm -rf task0a
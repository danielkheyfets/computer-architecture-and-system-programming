all: t3f

t3f: t3
	gcc -g -m32 task3.o -o task3
	rm task3.o


t3: task3.c
	gcc -g -m32 -c -o task3.o task3.c

.PHONY: clean
clean:
	rm -rf task3
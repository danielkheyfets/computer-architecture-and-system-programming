all: exec

exec: myELF
	gcc -g -m32 myELF.o -o myELF
	rm myELF.o

myELF: myELF.c
	gcc -g -m32 -c -o myELF.o myELF.c

.PHONY: clean
clean:
	rm -rf myELF
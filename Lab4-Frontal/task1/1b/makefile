all: exec

start: start.s
	nasm -f elf32 start.s -o start.o
encoder: encoder.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector encoder.c -o encoder.o
util: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

exec: start encoder util
	ld -m elf_i386 start.o encoder.o util.o -o encoder

.PHONY: clean
clean:
	rm -rf ./*.o encoder
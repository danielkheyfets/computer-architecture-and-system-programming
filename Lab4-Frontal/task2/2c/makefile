all: exec

start: start.s
	nasm -f elf32 start.s -o start.o

task2c: task2c.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2c.c -o task2c.o
util: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

exec: start task2c util 
	ld -m elf_i386 start.o task2c.o util.o -o flame2

.PHONY: clean
clean:
	rm -rf ./*.o flame2
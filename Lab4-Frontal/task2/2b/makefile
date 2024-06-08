all: exec

start: start.s
	nasm -f elf32 start.s -o start.o
task2b: task2b.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector task2b.c -o task2b.o
util: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

exec: start task2b util
	ld -m elf_i386 start.o task2b.o util.o -o flame2

.PHONY: clean
clean:
	rm -rf ./*.o flame2
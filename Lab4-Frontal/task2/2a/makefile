all: exec

start: start.s
	nasm -f elf32 start.s -o start.o
task2a: task2a.c
	gcc -m32 -Wall -c -nostdlib -fno-stack-protector task2a.c -o task2a.o
util: util.c
	gcc -m32 -Wall -c -nostdlib -fno-stack-protector util.c -o util.o

exec: start task2a util
	ld -m elf_i386 start.o task2a.o util.o -o flame2

.PHONY: clean
clean:
	rm -rf ./*.o flame2
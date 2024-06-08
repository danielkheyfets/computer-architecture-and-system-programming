all: exec

start: start.s
	nasm -f elf32 start.s -o start.o

main: main2.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main2.c -o main.o

# main: main.c
# 	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector main.c -o main.o
util: util.c
	gcc -m32 -Wall -ansi -c -nostdlib -fno-stack-protector util.c -o util.o

exec: start main util
	ld -m elf_i386 start.o main.o util.o -o task0a

.PHONY: clean
clean:
	rm -rf ./*.o task0a
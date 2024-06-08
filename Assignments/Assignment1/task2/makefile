all: exec

libs: asm-lib

asm-lib: asm_task2Assignment1.s
	nasm -g -f elf  -o asm_task2Assignment1.o asm_task2Assignment1.s

exec: main_task2Assignment1.c libs
	gcc -g -m32 -c -Wall -o main_task2Assignment1.o main_task2Assignment1.c
	gcc -g -m32  main_task2Assignment1.o asm_task2Assignment1.o -o task2Assignment1
	rm main_task2Assignment1.o asm_task2Assignment1.o

.PHONY: clean
clean:
	rm -rf ./*.o main
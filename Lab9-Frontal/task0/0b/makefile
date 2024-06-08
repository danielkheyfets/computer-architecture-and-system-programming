all: exec

x: skeleton.s
	nasm -f elf32 skeleton.s -o skeleton.o

exec: x  
	ld -m elf_i386 skeleton.o -o skeleton

.PHONY: clean
clean:
	rm -rf ./*.o skeleton
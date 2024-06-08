all: exec

x: skeleton.s
	nasm -f elf32 skeleton.s -o skeleton.o

exec: x  
	ld -m elf_i386 skeleton.o -o skeleton

.PHONY: clean
clean:
	rm -rf ./*.o skeleton


# all: exec

# x: task1.s
# 	nasm -f elf32 task1.s -o task1.o

# exec: x  
# 	ld -m elf_i386 task1.o -o task1

# .PHONY: clean
# clean:
# 	rm -rf ./*.o task1


# all: exec

# exec: x
# 	gcc -g -m32 virus.o -o virus
# 	rm virus.o

# x: virus.c
# 	gcc -g -m32 -c -o virus.o virus.c

# .PHONY: clean
# clean:
# 	rm -rf virus
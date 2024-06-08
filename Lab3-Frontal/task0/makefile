all: hp

#create executable file named 'encoder'
hp: hex
	gcc -g -m32 hexaPrint.o -o hexaPrint
	rm hexaPrint.o

#create .o file compilation
hex: hexaPrint.c
	gcc -g -m32 -c -o hexaPrint.o hexaPrint.c

.PHONY: clean
clean:
	rm -rf hexaPrint
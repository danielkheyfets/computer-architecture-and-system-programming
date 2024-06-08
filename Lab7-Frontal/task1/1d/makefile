all: exec

exec: hex
	gcc -g -m32 hexeditplus.o -o hexeditplus
	rm hexeditplus.o

hex: hexeditplus.c
	gcc -g -m32 -c -o hexeditplus.o hexeditplus.c

.PHONY: clean
clean:
	rm -rf hexeditplus
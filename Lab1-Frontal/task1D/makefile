all: encoder

#create executable file named 'encoder'
encoder: encoder.o
	gcc -g -m32 encoder.o -o encoder
	rm encoder.o

#create .o file compilation
encoder.o: encoder.c
	gcc -g -m32 -c -o encoder.o encoder.c

.PHONY: clean
clean:
	rm -rf encoder
all: exec

#create executable file named 'myshell'
exec: looper
	gcc -g -Wall -m32 looper.o -o looper

#create .o file compilation
looper: looper.c
	gcc -g -Wall -m32 -c  looper.c -o looper.o

.PHONY: clean
clean:
	rm -rf ./*.o myshell

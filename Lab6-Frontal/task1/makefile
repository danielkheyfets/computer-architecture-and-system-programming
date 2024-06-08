all: exec

#create executable file named 'myshell'
exec: mypipeline
	gcc -g -Wall -m32 mypipeline.o -o mypipeline

#create .o file compilation
mypipeline: mypipeline.c
	gcc -g -Wall -m32 -c  mypipeline.c -o mypipeline.o 

.PHONY: clean
clean:
	rm -rf ./*.o mypipeline
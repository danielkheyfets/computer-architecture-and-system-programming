all: exec

countW: count-words.c
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -o numbers.o = output compiled file to numbers.o
	gcc -g -m32 -c -o count-words.o count-words.c

exec: countW
	gcc -g -m32  count-words.o -o count-words
	rm count-words.o
	
.PHONY: clean
clean:
	rm -rf ./*.o count-words

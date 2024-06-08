all: exec

ad: addresses.c
	# flags meanings:
	# -g = debug symbols
	# -m32 = compile to x86_32 architecture (32bit)
	# -c = Don't link (i.e., create an object file, not an executable)
	# -o numbers.o = output compiled file to numbers.o
	gcc -g -m32 -c -o addresses.o addresses.c

exec: ad
	gcc -g -m32  addresses.o -o addresses
	rm addresses.o
	
.PHONY: clean
clean:
	rm -rf ./*.o addresses

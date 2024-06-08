all: exec

exec: t4
	gcc -g -Wall -m32 task4.o -o task4
	rm task4.o

t4: task4.c
	gcc -g -Wall -m32 -fno-pie -c -o task4.o task4.c

.PHONY: clean
clean:
	rm -rf task4
 



# all: exec

# exec: hex
# 	gcc -g -m32 hexeditplus.o -o hexeditplus
# 	rm hexeditplus.o

# hex: hexeditplus.c
# 	gcc -g -m32 -c -o hexeditplus.o hexeditplus.c

# .PHONY: clean
# clean:
# 	rm -rf hexeditplus
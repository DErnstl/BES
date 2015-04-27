CC = gcc -Wall -Werror -Wextra -Wstrict-prototypes -pedantic -fno-common -g -O3

all:
	${CC} -c myopen.c -o myopen.o
	gcc -o popentest myopen.o -lpopentest
doc:
	doxygen doxygen.dcf	
clean:
	rm -f popentest
	rm -f myopen.o
	rm -r -f doc
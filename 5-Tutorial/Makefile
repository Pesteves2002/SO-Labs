# Makefile
#
# It uses implicit rules to compile .c files using the
# values assigned to macros CC and CFLAGS
#

CC = gcc
CFLAGS = -g -Wall -std=c11


contapartilhada: contapartilhada.o
	gcc -o contapartilhada contapartilhada.o -lpthread

clean:
	rm -f *.o contapartilhada 
	

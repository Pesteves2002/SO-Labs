# Makefile
#
# It uses implicit rules to compile .c files using the
# values assigned to macros CC and CFLAGS
#

CC = gcc
CFLAGS = -g -Wall -std=c11


thread: thread.o
	gcc -o thread thread.o -lpthread

alt: alt.o
	gcc -o alt alt.o -lpthread

clean:
	rm -f *.o thread 
	rm -f *.o alt

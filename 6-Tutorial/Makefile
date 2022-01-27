# Makefile
#
# It uses implicit rules to compile .c files using the
# values assigned to macros CC and CFLAGS
#

CC = gcc
CFLAGS = -g -Wall -std=c11


coordination: coordination.o
	gcc -o coordination coordination.o -lpthread

coordination_condvar: coordination_condvar.o
	gcc -o coordination_condvar coordination_condvar.o -lpthread
clean:
	rm -f *.o coordination 

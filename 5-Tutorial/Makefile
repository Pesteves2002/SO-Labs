# Makefile
#
# It uses implicit rules to compile .c files using the
# values assigned to macros CC and CFLAGS
#

CC = gcc
CFLAGS = -g -Wall -std=gnu11 -lpthread -fsanitize=undefined -fsanitize=thread
OBJECTS = contapartilhada

all: $(OBJECTS)

clean:
	rm -f *.o $(OBJECTS)
	

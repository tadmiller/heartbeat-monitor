# Ensure that these targest always run
.PHONY: clean

# DO NOT CHANGE THESE FLAGS
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Werror

all:
	$(CC) $(CFLAGS) commands.c -o cmds.o

clean:
	rm -f *.o

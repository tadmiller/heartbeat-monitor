# Ensure that these targest always run
.PHONY: clean

# DO NOT CHANGE THESE FLAGS
CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Werror

all:
	$(CC) $(CFLAGS) -c commands.c -o cmd.o

clean:
	rm -f *.o

.PHONY: all

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -g

all: shell

shell: shell.c shell.h builtins.c builtins.h
	${CC} ${CFLAGS} shell.c builtins.c -o shell.o

clean:
	@rm -f build/*
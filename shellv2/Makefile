.PHONY: all

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Werror -lsqlite3

all: compile

compile:
	${CC} ${CFLAGS} cli.c shell.c ardlib.c utils.c db.c -o cli.o

clean:
	@rm -rf *.o
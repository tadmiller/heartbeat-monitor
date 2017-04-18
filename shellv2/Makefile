.PHONY: all

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Werror

all: compile

compile:
	${CC} ${CFLAGS} cli.c shell.c ardlib.c utils.c -o cli.o

clean:
	@rm -rf *.o
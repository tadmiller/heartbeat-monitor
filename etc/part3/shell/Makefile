.PHONY: all

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Werror

all: compile

compile:
	${CC} ${CFLAGS} shell.c -o shell.o

clean:
	@rm -rf *.o
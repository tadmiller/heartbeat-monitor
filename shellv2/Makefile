.PHONY: all

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Werror
CLINK=-lsqlite3

all: compile

compile:
	${CC} ${CFLAGS} cli.c shell.c ardlib.c utils.c db.c -o cli.o ${CLINK}

clean:
	@rm -rf *.o

install:
	sudo apt-get update -y && sudo apt-get install libsqlite3-dev -y && sudo apt-get install sqlite3 -y
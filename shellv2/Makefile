.PHONY: all

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Werror
CLINK=-lsqlite3
APT=sudo apt-get -y

all: clean compile

compile:
	${CC} ${CFLAGS} cli.c shell.c ardlib.c utils.c db.c -o cli.o ${CLINK}

clean:
	@rm -f *.o

install: sqlite compile

sqlite:
	${APT} update && ${APT} install libsqlite3-dev && ${APT} install sqlite3
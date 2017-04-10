.PHONY: all

CC=gcc
CFLAGS=-std=c99 -Wall -pedantic -Werror

all: visualizer

visualizer: visualizer.c
	${CC} ${CFLAGS} visualizer.c -o visualizer.o

clean:
	@rm -rf *.o
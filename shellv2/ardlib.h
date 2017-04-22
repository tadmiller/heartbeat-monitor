#ifndef _ARDLIB_H
#define _ARDLIB_H

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#ifndef CRTSCTS
#define CRTSCTS 020000000000
#endif

#define BUFFER_SIZE 32

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <time.h>

#include "utils.h"

void sys_exit();

void arduino_env();

void arduino_heartrate();

void arduino_close();

int arduino_connect(char **args);

void process_rate();

void print_histogram(int bpms[10], int hours[10], int mins[10], int secs[10]);

void arduino_show();

void arduino_pause();

void arduino_resume();

char *arduino_rate(bool keep);

//void arduino_clock_sync();

void arduino_clock(char **args);

void arduino_clock_sync();

int send_bytes(char byte);

int init_tty(int fd);

void fork_heartrate();

#endif
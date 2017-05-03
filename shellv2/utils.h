#ifndef _UTILS_H
#define _UTILS_H

#define GROUP_SIZE 96

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <stdint.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <math.h>

#include "db.h"

void clear_mmap();

void print_histogram(int bpms[10], int hours[10], int mins[10], int secs[10]);

char *mmap_read(char *file);

void process_hist(char *map);

int mmap_write(char *data, char *file, char mode);

int inverse_bucket_m(int bucket);

int inverse_bucket_h(int bucket);

void process_hist_v2(char **args);

void calc_stat(char *time);

#endif
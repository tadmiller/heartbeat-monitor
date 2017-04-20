#ifndef _UTILS_H
#define _UTILS_H

void clear_mmap();

void print_histogram(int bpms[10], int hours[10], int mins[10], int secs[10]);

char *mmap_read(char *file);

void process_hist(char *map);

int mmap_write(int bpms[10], int hours[10], int mins[10], int secs[10]);

void process_hist_v2(char **args);

#endif
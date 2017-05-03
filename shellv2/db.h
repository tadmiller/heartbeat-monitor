#ifndef _DB_H
#define _DB_H

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#ifndef CRTSCTS
#define CRTSCTS 020000000000
#endif

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <stdbool.h>

void db_init();

void db_handler(char **args);

void db_insert(char *time, int rate, char *env);

void db_exec(char *sql, char type);

void db_select();

char ***db_calc_data(int *s);

int db_size();

#endif
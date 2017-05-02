#ifndef _DB_H
#define _DB_H

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h> 

void db_init();

void db_handler(char **args);

void db_insert(int rate, char *env);

#endif
#ifndef _SHELL_H
#define _SHELL_H

#define TOK_BUFSIZE 16
/* Characters to stop tokenizing on */
#define TOK_DELIM " \t\r\n\a"
/* Fallback prompt to show */
#define DEFAULT_PROMPT "> "

#define _GNU_SOURCE
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "ardlib.h"
#include "utils.h"
#include "db.h"

void print_prompt();

size_t read_line(char **);

char **tok_line(char *line);

int shell_exec(char **args);

int shell_run(char **args);

int shell_init();

#endif
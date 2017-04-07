/************************************************
 *              CSCI 3410, Week 10
 *                  Mini Shell
 *************************************************/

#ifndef _SHELL_H
#define _SHELL_H

#include "builtins.h"

/* Size to string to tokenize at a time */
#define TOK_BUFSIZE 16

/* Characters to stop tokenizing on */
#define TOK_DELIM " \t\r\n\a"

/* Fallback prompt to show */
#define DEFAULT_PROMPT "> "

static char *builtins[] = {
        "print",
        "exit"
};

static int(*builtin_funcs[]) (char **) = {
        &builtin_print,
        &builtin_exit
};

void
print_prompt(void);

size_t
read_line(char **);

char **
tok_line(char *line);

int
shell_exec(char **args);

int
shell_run(char **args);

#endif

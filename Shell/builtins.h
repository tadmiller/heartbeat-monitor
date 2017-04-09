/************************************************
 *              CSCI 3410, Week 10
 *                  Mini Shell
 *************************************************/

#ifndef _SHELL_BUILTINS_H
#define _SHELL_BUILTINS_H

#define BUILTIN_COUNT 2

int
shell_builtin_count(void);

int
builtin_cd(char **args);

int
builtin_exit(char **args);


#endif

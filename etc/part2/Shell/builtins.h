/************************************************
 *              CSCI 3410, Week 10
 *                  Mini Shell
 *************************************************/

#ifndef _SHELL_BUILTINS_H
#define _SHELL_BUILTINS_H

int shell_builtin_count(void);

//int builtin_cd(char **args);

void builtin_exit();
void resume();
void pauseProg();
void show();
int connectArduino(int argc, char **argv, char *device);

#endif

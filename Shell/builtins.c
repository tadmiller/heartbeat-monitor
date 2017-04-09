/************************************************
 *              CSCI 3410, Week 10
 *                  Mini Shell
 *************************************************/

#define _GNU_SOURCE

#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "builtins.h"

#define HOME_DIR_ENV "HOME"
//static const char *__get_home_dir(void);

// int shell_builtin_count() {
//     return BUILTIN_COUNT;
// }

/* Leave. */
void builtin_exit()
{
    exit(0);
}
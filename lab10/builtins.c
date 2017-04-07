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
static const char *__get_home_dir(void);

int shell_builtin_count() {
    return BUILTIN_COUNT;
}

/* cd implementation */
int builtin_print(char **args)
{
    char buf[4096];

    printf("\n%s\n", *(args + 1));

    // FILE *fp = fopen(*args + 1, "r");

    // while(fgets(buf, 4096, fp) != 0) {
    //     // do stuff
    // }
    // fclose(fp);

    return 0;
}

/* Leave. */
void builtin_exit()
{
    exit(0);
}
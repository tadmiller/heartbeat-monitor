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

int shell_builtin_count()
{
    return BUILTIN_COUNT;
}

/* Leave. */
int builtin_exit()
{
    //exit(0);

    return -1;
}

/* Get the current user's home directory
 * First check HOME environment variable
 * and try /etc/passwd if not
 */
static const char *__get_home_dir(void)
{
    char *dir;
    struct passwd *pw;

    /* Switch to the user's home directory */
    dir = getenv(HOME_DIR_ENV);
    if (!dir) {
            /* Environment variable not set, read from /etc/passwd */
            pw = getpwuid(getuid());
            if (!pw) {
                    return NULL;
            }
            dir = pw->pw_dir;
            free(pw);
    }
    return dir;
}

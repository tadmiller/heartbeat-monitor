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

int
shell_builtin_count() {
        return BUILTIN_COUNT;
}

/* cd implementation */
int
builtin_cd(char **args)
{
        /* Change the working directory of the shell's current process
         * Think: why must this be implemented as a builtin function and
         * not forked and run in a subprocess?
         *
         * If run with no arguments, change to the user's home directory.
         * Otherwise, change to the directory passed as an argument.
         */
        printf("\nCalling cd\n");
        char pathname[32]; 
        strcpy(pathname, *args + 1);//= *args + 1;
        int i = 0;
        chdir(*args + 1);
        // TODO

        struct direct **files;

        if (getwd(pathname) == NULL )
                        { printf("Error getting path\n");
                                        exit(0);
                        }
        printf("Current Working Directory = %s\n",pathname);
        int count = scandir(pathname, &files);

        /* If no files found, make a non-selectable menu item */
        if (count <= 0)
        {
                printf("No files in this directory\n");
                return -1;
        }

        printf("Number of files = %d\n",count);

        for (i = 1; i < count + 1; ++i)
                printf("\n%s", (char *)(files[i-1] -> d_name));
        printf("\n"); /* flush buffer */

        return 0;
}

int pwd()
{

}

/* Leave. */
int
builtin_exit(char **args) {
        return -1;
}

/* Get the current user's home directory
 * First check HOME environment variable
 * and try /etc/passwd if not
 */
static const char *
__get_home_dir(void)
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

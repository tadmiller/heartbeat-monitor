/************************************************
 *              CSCI 3410, Week 10
 *                  Mini Shell
 *************************************************/

#define _GNU_SOURCE

#include <libgen.h>
#include <limits.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "shell.h"
#include "builtins.h"


int main()
{
	char *line;
	char **args;
	size_t line_len;
	int status;

	do {
			/* Give the user a prompt */
			print_prompt();

			/* Read and parse the input */
			line_len = read_line(&line);
			if (line_len == 0) continue;
			args = tok_line(line);

			/* Run the command */
			status = shell_exec(args);

			/* Cleanup memory */
			free(line);
			free(args);
	} while (status >= 0);

	return EXIT_SUCCESS;
}

void print_prompt(void)
{
	struct passwd *pw;
	char *host;
	char *dir = NULL;
	char *base;
	size_t size = 0;

	/* Get user name from /etc/passwd file */
	pw = getpwuid(getuid());
	if (!pw) {
			printf("%s", DEFAULT_PROMPT);
			return;
	}
	printf("%s@", pw->pw_name);

	/* Get hostname */
	host = malloc(HOST_NAME_MAX + 1);
	if (gethostname(host, HOST_NAME_MAX + 1)) {
			perror("psh");
			printf("%s", DEFAULT_PROMPT);
			return;
	}
	printf("%s ", host);
	free(host);

	/* Get current working directory */
	dir = getcwd(dir, size);
	if (!dir) {
			perror("psh");
			printf("%s", DEFAULT_PROMPT);
			return;
	}
	base = basename(dir);
	printf("%s $ ", base);
	free(dir);
}

size_t read_line(char **line)
{
	size_t size = 0;

	/* Read a line from the user */
	getline(line, &size, stdin);

	return size;
}

char **tok_line(char *line)
{
	int buffer = TOK_BUFSIZE;
	int position = 0;
	char *token;
	char **tokens;
	char **tokens_backup;

	tokens = malloc(buffer * sizeof(char *));

	if (!tokens)
	{
			fprintf(stderr, "psh: Allocation failure :(\n");
			exit(EXIT_FAILURE);
	}

	/* Tokenize the input string
	 * Can't handle spaces inside quoted strings :(  */
	token = strtok(line, TOK_DELIM);
	while (token)
	{
			tokens[position++] = token;

			if (position >= buffer) {
					/* If the new string is greater than our buffer, increase the size
					 * Make sure to backup the old version, in case realloc fails */
					buffer += TOK_BUFSIZE;
					tokens_backup = tokens;
					tokens = realloc(tokens, buffer * sizeof(char *));
					if (!tokens) {
							/* Free the pre-realloc array and exit with failure */
							free(tokens_backup);
							fprintf(stderr, "psh: Allocation failure :(\n");
							exit(EXIT_FAILURE);
					}
			}
			token = strtok(NULL, TOK_DELIM);
	}

	tokens[position] = NULL;
	return tokens;
}

/*
 * Used to run a command, first checking against the list of builtin commands
 * This should return a positive number, as long as the shell should keep running
 * In practice, only the `exit` builtin should return something negative
 */
int shell_exec(char **args)
{
	/* Check for empty command */
	if (args[0] == NULL)
		return 0;

    if(strcmp(*args, "resume") == 0)   //if input is resume then start resume method
        resume();
    else if(strcmp(*args, "pause") == 0) //if input is pause then start pause method
        pauseProg();
    else if(strcmp(*args, "show") == 0) //if input is show X then start show method
        show();
	else if (strcmp(*args, "exit") == 0 || strcmp(*args, "quit") == 0 || strcmp(*args, "q") == 0)
		builtin_exit();
    else
        printf("Invalid input!\n"); //anything else is read as invalid 
		
	/**
	 * Below, implement the part where we run a builtin shell function, if
	 * feeded. Refer to builtins.c  and the declarations in shell.h to
	 * understand how the builtin functions work. If the first command
	 * matches any of them, run the function pointer at the proper
	 * index of the builtins array and return from this function.
	 */

	/* Check the list of builtin functions first */
	// TODO

	/* Simple fork/exec */
	return shell_run(args);
}

/*
 * Forks this process and runs what it has been passed
 */
int shell_run(char **args)
{
	// pid_t pid;
	// int status;
	// int i = 0;
	/* // Check if the last argument is "&"
	int concurrent = 0;
	while (args[i]) i++;
		if (strcmp("&", args[i - 1]) == 0)
		{
			concurrent = 1;
			args[i - 1] = NULL;
		}
	*/

	/* Fork the process and execute stuff!
	 * In the child process, run the command the user desires
	 * Be sure to implement robust error checking.
	 * If the concurrent variable is true, then we should not wait
	 * for the subprocess to finish, but instead print out its PID
	 */
	// TODO

	printf("\nCMD IS: %s\n", *args + 0);

	return 0;
}






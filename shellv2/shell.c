#define _GNU_SOURCE
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sqlite3.h> 

#include "shell.h"
#include "ardlib.h"
#include "utils.h"

int shell_init()
{
	char *line;
	char **args;
	size_t line_len;
	int status;

	do
	{   /* Give the user a prompt */
		print_prompt();

		/* Read and parse the input */
		line_len = read_line(&line);

		if (line_len == 0)
			continue;

		args = tok_line(line);

		/* Run the command */
		status = shell_exec(args);

		/* Cleanup memory */
		free(line);
		free(args);
	}
	while (status >= 0);

	return EXIT_SUCCESS;
}

void print_prompt()
{
	struct passwd *pw;
	char *dir = NULL;
	char *base;
	size_t size = 0;

	/* Get user name from /etc/passwd file */
	pw = getpwuid(getuid());
	if (!pw)
	{
		printf("%s", DEFAULT_PROMPT);
		return;
	}

	printf("%s@processor/", pw->pw_name);

	/* Get current working directory */
	dir = getcwd(dir, size);
	if (!dir)
	{
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

		if (position >= buffer)
		{	/* If the new string is greater than our buffer, increase the size
			* Make sure to backup the old version, in case realloc fails */
			buffer += TOK_BUFSIZE;
			tokens_backup = tokens;
			tokens = realloc(tokens, buffer * sizeof(char *));

			if (!tokens)
			{	// Free the pre-realloc array and exit with failure
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

	// Required commands
	if (strcmp(*args, "date") == 0)   //if input is resume then start resume method
		arduino_clock(args);
	else if (strcmp(*args, "regression") == 0)   //if input is resume then start resume method
		printf("regression");
	// else if (strcmp(*args, "stat") == 0)
	else if (strcmp(*args, "env") == 0)
		arduino_env(0);
	else if (strcmp(*args, "exit") == 0 || strcmp(*args, "quit") == 0 || strcmp(*args, "q") == 0)
		sys_exit();
	else if (strcmp(*args, "hist") == 0)
		process_hist_v2(args);
	else if (strcmp(*args, "pause") == 0)
		arduino_pause();
	else if (strcmp(*args, "rate") == 0)
		arduino_rate(0);
	// else if (strcmp(*args, "reset") == 0)
	else if (strcmp(*args, "resume") == 0)
		arduino_resume();
	else if (strcmp(*args, "show") == 0)
		arduino_show(args);
	else if (strcmp(*args, "connect") == 0 || strcmp(*args, "c") == 0)
		arduino_connect(args);
	// Additional commands
	else if (strcmp(*args, "close") == 0)
		arduino_close();
	// else if (strcmp(*args, "flush") == 0)
	// else if (strcmp(*args, "set") == 0)
	else if (strcmp(*args, "collect") == 0)
		fork_heartrate();


	//     connectArduino();
	// else if (strcmp(*args, "pause") == 0) //if input is pause then start pause method
	//     pauseProg();
	// else if (strcmp(*args, "show") == 0) //if input is show X then start show method
	//     show();
	else
		printf("\t\t\tcmd error: %s not found\n", *args);
		
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
	return 0; // shell_run(args);
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

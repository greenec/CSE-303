/*
 * Connor Greene (crg222)
 * CSE 303 - Spring 2020
 * Program 2 - Shell
 *
 * This program should work on any UNIX system.
 *
 * To compile and run:
 *    >>(g)cc shellStart.c -o ashell
 *    >>ashell 
 *      OR
 *    >>(g)cc shellStart.c
 *    >>a.out
 */

#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

enum {
	MAXLINE = 200
};

/*
 * Prints the ASCII clear command to reset the terminal.
 */
void clearScreen()
{
	printf("\e[1;1H\e[2J");
}

/*
 * Prints the contents of a given directory.
 */
void listDirectory(const char* const s)
{
	// pointer for directory 
	struct dirent *de;

	char s2[MAXLINE];
	strcpy(s2, s);
	
	// use strtok to get rid of line feed then find the first token
	strtok(s2, "\n");
	strtok(&s2[2], " ");
	char* dir = strtok(NULL, " ");
	
	// if no directory is specified, read the current directory
	if (dir == NULL)
	{
		dir = s2;
		strcpy(dir, ".");
	}

	// opendir() returns a pointer of DIR type
	DIR *dr = opendir(dir);
	
	// opendir returns NULL if couldn't open directory
	if (dr == NULL)
	{
		fprintf(stderr, "Could not open the directory '%s'\n", dir);
		return;
	}

	while ((de = readdir(dr)) != NULL)
	{	
		printf("%s ", de->d_name);
	}
	
	printf("\n");

	closedir(dr); 
}

void runCommand(const char* const s)
{
	// make a copy of the input string
	char cmd[MAXLINE];
	strcpy(cmd, s);
	
	// use strtok to get rid of line feed then find the first token
	strtok(cmd, "\n");
	char* ptr = strtok(&cmd[2], " ");

	// get the rest of the tokens and call exec
	int i = 0;
	char* argvt[MAXLINE];
	while (ptr) 
	{
		// passing a null pointer will cause cause it to continue where it left off
		ptr = strtok(NULL, " ");
		argvt[i++] = ptr;
	}
	
	int pid = fork();
	if (pid == 0)
	{
		// this is the child process, exec the command
		execvp(argvt[0], argvt);
		fprintf(stderr, "EXEC FAILED\n");
	}
	else
	{
		// parent waits for the child to finish
		wait(NULL);
	}
}

/*
 * This function processes the command.
 */
void processLine(const char* const s)
{
    // output the command entered by the user
	printf("Command received = %s\n", s);
	
	/*
	 * Command: Quit
	 * Usage: "quit"
	 * Description: Exits the shell program.
	 */
	const char* const stopStr = "quit";
	if (strncmp(s, stopStr, strlen(stopStr)) == 0)
	{
		exit(0);
	}

	/*
	 * Command: Clear
	 * Usage: "clr"
	 * Description: Clears the screen. 
	 */
	const char* const clearStr = "clr";
	if (strncmp(s, clearStr, strlen(clearStr)) == 0)
	{
		clearScreen();
		return;
	}
	
	/*
	 * Command: Directory
	 * Usage: "dir <directory>"
	 * Description: List the contents of <directory>
	 */
	const char* const directoryStr = "dir";
	if (strncmp(s, directoryStr, strlen(directoryStr)) == 0)
	{
		listDirectory(s);
		return;
	}
	
	/*
	 * Command: Run
	 * Usage: "run <command> <arg1> <arg2> <...>"
	 * Description: Runs the specified command with its arguments.
	 */
	const char* const runStr = "run";
	if(strncmp(s, runStr, strlen(runStr)) == 0)
	{
		runCommand(s);
		return;
	}
}

int main()
{
    char line[MAXLINE];

    /*
     * Get a command line, parse it and process it.
     * This program exits via an exit(0) in the processLine() function.
     */
    while(1)
    {
		printf("crg222 > ");
		fgets(line, MAXLINE, stdin);
		processLine(line);
    }
	
    return 0;
}


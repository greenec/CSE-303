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

const int MAXLINE = 200;

/* 
 * Reverses a given string without modifying the original.
 */
void strrev(const char* const str, char* out)
{
	char *p1, *p2;

	if (! str || ! *str)
	{
		return;
	}
	
	// copy the input to the output string so it can be reversed
	strcpy(out, str);
	
	// use two pointers and XOR to reverse the output string
	for (p1 = out, p2 = out + strlen(out) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
}

/*
 * Prints the ASCII clear command to reset the terminal.
 */
void clearCmd()
{
	printf("\e[1;1H\e[2J");
}

/*
 * Prints the contents of a given directory.
 */
void directoryCmd(const char* const s)
{
	char cmd[strlen(s)];
	strcpy(cmd, s);
	
	// use strtok to get rid of line feed then find the first token
	strtok(cmd, "\n");
	strtok(&cmd[2], " ");
	char* dir = strtok(NULL, " ");
	
	// if no directory is specified, read the current directory
	if (dir == NULL)
	{
		dir = cmd;
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

	// pointer for directory 
	struct dirent *de;
	
	while ((de = readdir(dr)) != NULL)
	{
		if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
		{
			continue;
		}
		
		printf("%s ", de->d_name);
	}
	
	printf("\n");

	closedir(dr); 
}

/* 
 * Runs the given command with its arguments.
 */
void runCmd(const char* const s)
{
	// make a copy of the input string
	char cmd[strlen(s)];
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
		
		// this will only run if execvp fails
		fprintf(stderr, "EXEC FAILED\n");
		exit(1);
	}
	else
	{
		// parent waits for the child to finish
		wait(NULL);
	}
}

/*
 * Puts the process to sleep for the given duration.
 */
void sleepCmd(const char* const s)
{
	// make a copy of the input string
	char cmd[strlen(s)];
	strcpy(cmd, s);
	
	// user strtok to get rid of line feed then find the first token
	strtok(cmd, "\n");
	strtok(&cmd[4], " ");
	char* ptr = strtok(NULL, " ");
	
	int duration = atoi(ptr);
	
	sleep(duration);
}

/*
 * Prints all the environment strings.
 */
void environmentCommand(char** envp)
{
	int i = 0;
	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i++]);
	}
}

/*
 * Determines if the input string is blank.
 */
int isBlankInput(const char* const s)
{
	char str[strlen(s)];
	strcpy(str, s);
	
	// remove leading spaces and newline
	char* ptr = strtok(str, " \n");
	
	// return true if it's a null pointer
	return ptr == NULL;
}

/*
 * Determines whether the given command ends with an ampersand (&)
 */
int isBackgroundCmd(const char* const s)
{
	// make a reversed copy of the input command
	char reversed[strlen(s)];
	strrev(s, reversed);
	
	// remove any trailing spaces and newline
	char* ptr = strtok(reversed, " \n");
	
	if (ptr != NULL)
	{
		return ptr[0] == '&';
	}
	
	return 0;
}

/*
 * This function processes the command.
 */
void processLine(char** envp, const char* const s)
{
	if (isBlankInput(s))
	{
		return;
	}

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

	// if there is a trailing '&', fork and return the parent immediately
	int bBackgroundCmd = isBackgroundCmd(s);
	if (bBackgroundCmd)
	{
		int pid = fork();
		if (pid != 0)
		{
			// parent returns immediately to run the next command
			return;
		}
	}

	/*
	 * Command: Clear
	 * Usage: "clr"
	 * Description: Clears the screen. 
	 */
	if (strncmp(s, "clr", 3) == 0)
	{
		clearCmd();
	}
	
	/*
	 * Command: Directory
	 * Usage: "dir <directory>"
	 * Description: List the contents of <directory>
	 */
	else if (strncmp(s, "dir", 3) == 0)
	{
		directoryCmd(s);
	}
	
	/*
	 * Command: Run
	 * Usage: "run <command> <arg1> <arg2> <...>"
	 * Description: Runs the specified command with its arguments.
	 */
	else if (strncmp(s, "run", 3) == 0)
	{
		runCmd(s);
	}
	
	/*
	 * Command: Sleep
	 * Usage "sleep <duration>"
	 * Description: Sleeps for <duration> seconds.
	 */
	else if (strncmp(s, "sleep", 5) == 0)
	{
		sleepCmd(s);
	}

	/*
	 * Command: Environment
	 * Usage "environ"
	 * Description: List all the environment strings.
	 */
	else if (strncmp(s, "environ", 7) == 0)
	{
		environmentCommand(envp);
	}
	
	else 
	{
		// the command is not defined above, run it with the UNIX system() function
		system(s);
	}

	if (bBackgroundCmd)
	{
		exit(0);
	}
}

int main(int argc, char** argv, char** envp)
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
		processLine(envp, line);
    }
	
    return 0;
}

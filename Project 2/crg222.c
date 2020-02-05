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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

enum {
	MAXLINE = 200
};

/*
 * This function processes the command.
 */
void processLine(const char* const s)
{
    /*
	 * Do not hesitate to declare lots of local variables.
	 * The optimizer removes them but can make the program more readable.
     */
	const char* const stopStr = "quit";
	const char* const newline = "\n";
	const int sLen = strlen(stopStr);
	const int sVal = strncmp(s, stopStr, sLen);

    // output the command entered by the user
	printf("Command received = %s\n", s);

    // check if the user entered the stop string, "quit"
	if(!sVal)
	{
		exit(0);
	}
	else
    {
		const char* const runStr = "run";
		const int rLen = strlen(runStr);
		const int rVal = strncmp(s, runStr, rLen);
	   
		if(!rVal)
		{
			char s2[MAXLINE];
			char* argvt[MAXLINE];
			int i = 0;
			char* cx = strncpy(s2, s, MAXLINE);

			// use strtok to get rid of line feed then find the first token
			char* c = strtok(&s2[rLen - 1], newline);
				  c = strtok(&s2[rLen - 1], " ");

			// get the rest of the tokens and call exec
			while (c) 
			{
				c = strtok(0," ");
				argvt[i++]= c; 
			}
			
			execvp(argvt[0],argvt);
			printf("EXEC FAILED\n");
		}
    }
}

int main()
{
    char line[MAXLINE];

    /*
     * When a const is needed declare it as such.
     */
    const char* const prompt = "crg222 > ";

    /*
     * Get a command line, parse it and process it.
     * This program exits via an exit(0) in the processLine() function.
     */
    while(1)
    {
		const int j = fputs(prompt, stdout);
		const char* const c = fgets(line, MAXLINE, stdin);
		processLine(line);
    }
    return 0;

}


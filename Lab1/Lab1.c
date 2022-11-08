 // ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed:____Gregory _Adams_____ Date:___10/5/22______

// 3460:426 Lab 1 - Basic C shell rev. 9/10/2020

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_ARGS		64
#define MAX_ARG_LEN		16
#define MAX_LINE_LEN	80
#define WHITESPACE		" ,\t\n"

struct command_t {
   char *name;
   int argc;
   char *argv[MAX_ARGS];
};

/* Function prototypes */
int parseCommand(char *, struct command_t *);
void printPrompt();
void readCommand(char *);
void help(void);

int main(int argc, char *argv[]) {
   int pid;
   int status;
   char cmdLine[MAX_LINE_LEN];
   struct command_t command;
   char args[1][1];
   args[0][0] = '\0';
   int pseudoBool = 1;

   while (pseudoBool == 1) {
      printPrompt();
      /* Read the command line and parse it */
      readCommand(cmdLine);
      parseCommand(cmdLine, &command);
      command.argv[command.argc] = NULL;
	
	  /*
	     TODO: if the command is one of the shortcuts you're testing for
		 either execute it directly or build a new command structure to
		 execute next
	  */
	  
		  if (strcmp(command.name, "C") == 0)
		  {
			  command.name = "cp"; // set command.name to the existing command
			
		  }
		  else if (strcmp(command.name, "D") == 0)
		  {
			command.name = "rm";  // set command.name to the existing command
			
		  }
		  else if (strcmp(command.name, "E") == 0)
		  {
			command.name = "echo"; // set command.name to the existing command
									// seems to be working without setting command.argv
		  }
		  else if (strcmp(command.name, "H") == 0)
		  {
			help(); // calls the help function which just prints
			command.argv[2] = NULL;		// my "manual" to console
			// set command.argv[0] aswell for all ? 
		  }
		  else if (strcmp(command.name, "L") == 0)
		  {	
			// fork here
			  command.name = "pwd"; // We want to first print out the current directory
			  if ((pid=fork()) == 0) // we will need to use execvp twice
			  {							// thus we fork here
				execvp(command.name, command.argv); // <- this executes pwd
			  }
			  wait(&status); // we wait for the child to finish
			  char* args[3]; // we will need 3 arguments seen below
			  command.name = "ls";
			  args[0] = "ls";
			  args[1] = "-l";
			  args[2] = '/0';
			  command.argv[3] = args;
		  }
		  else if (strcmp(command.name, "M") == 0)
		  {
			command.name = "nano"; // set command.name to the existing command
			
		  }
		  else if (strcmp(command.name, "P") == 0)
		  {
			command.name = "more"; // set command.name to the existing command
						
		  }
		  else if (strcmp(command.name, "Q") == 0)
		  {
			pseudoBool = 0; // this will turn the bool value to 0 so the loop wont run again
			break;		// the break should leave the loop immediately aswell, this is just for extra protection.
		  }
		/*  else if (strcmp(command.name, "S") == 0)
		  {
			command.name = "firefox";  // NO LONGER NEED THIS FUNCTION
		  }
		*/
		  else if (strcmp(command.name, "W") == 0)
		  {
			command.name = "clear"; // set command.name to the existing command
			command.argv[2] = NULL;						// seems to be working without setting command.argv
		  }
		  else if (strcmp(command.name, "X") == 0)
		  {
			command.name = command.argv[1]; // first set command.name to the actual name wanting to be executed
			int comSize = strlen(command.name); // get the size of said command
			char newCommand[comSize + 3];  // create new C_String with 3 extra slots to incluide : null terminator and "./".
			
			
			for (auto i =0 ; i < comSize; ++i) // fills this new array with the command that we want to execute
			{									// but it is scewed by 2 to account for the "./" command.
				newCommand[i+2] = command.name[i];
			}
			newCommand[0] = '.'; // adding in the "./" command here.
			newCommand[1] = '/';
			newCommand[comSize + 2] = NULL; // add NULL here so that we don't have memory leaks
			for (auto i = 0; i < comSize + 2; ++i) // we then will fill the command.name from the structure with the new command
			{
				command.name[i] = newCommand[i];
			}
			command.name[comSize+2] = '\0'; // add null terminator back again
			command.argv[0] = newCommand;
			command.argv[1] = NULL; // arv[1] = NULL so we know that there are no more arguments.
		}
		
		  
	  
      /* Create a child process to execute the command */
      if ((pid = fork()) == 0) {
         /* Child executing command */
		execvp(command.name, command.argv);
		
	 /* TODO: what happens if you enter an incorrect command? */
      return -1; // if we get passed execvp: the program did not execute, -1 will return and thread will term
      }
      /* Wait for the child to terminate */
      wait(&status); /* EDIT THIS LINE */ // This line is already correct.
   }

   /* Shell termination */
   printf("\n\n shell: Terminating successfully\n");
   return 0;
}

/* End basic shell */

/* Parse Command function */

/* Determine command name and construct the parameter list.
 * This function will build argv[] and set the argc value.
 * argc is the number of "tokens" or words on the command line
 * argv[] is an array of strings (pointers to char *). The last
 * element in argv[] must be NULL. As we scan the command line
 * from the left, the first token goes in argv[0], the second in
 * argv[1], and so on. Each time we add a token to argv[],
 * we increment argc.
 */
int parseCommand(char *cLine, struct command_t *cmd) {
   int argc;
   char **clPtr;
   /* Initialization */
   clPtr = &cLine;	/* cLine is the command line */
   argc = 0;
   cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
   /* Fill argv[] */
   while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
      cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
   }

   /* Set the command name and argc */
   cmd->argc = argc-1;
   cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
   strcpy(cmd->name, cmd->argv[0]);
   return 1;
}

/* End parseCommand function */

/* Print prompt and read command functions - Nutt pp. 79-80 */

void printPrompt() {
   /* Build the prompt string to have the machine name,
    * current directory, or other desired information
    */
   char *promptString = "linux~gna5$";
   // promptString = "linux gna5"; /* EDIT THIS LINE */
   printf("%s ", promptString);
}

void readCommand(char *buffer) {
   /* This code uses any set of I/O functions, such as those in
    * the stdio library to read the entire command line into
    * the buffer. This implementation is greatly simplified,
    * but it does the job.
    */
   fgets(buffer, 80, stdin);
}

/* End printPrompt and readCommand */

/* simply prints out my "manual" to the terminal. */
void help(void)
{
printf("|linux~gna5 - man page| \n \n");
printf("*Description: \n ");

printf("linux~gna5 is a simple stripped down linux shell to execute a few basic commands. ");
printf("Some of these commands could be called where as others required a little bit more work. \n \n");
printf("*Commands: \n \n");
printf("C file1 file2		-Copies file, file1 is the file to be copied; file2 is the destination. \n \n");

printf("D file			-Deletes the named file\n \n");
printf("E comment		-Will display the comment on the screen, followed by a new line. \n \n");
printf("H			-Will the display the user manual.\n \n");
printf("L 			-Lists the current directory\n \n");
printf("M file			-will create the named text file via nano.\n \n");
printf("P file			-will print the contents of a text file to the screen.\n \n");
printf("Q			-will quit out of the shell.\n \n");
printf("W			-wipes the screen.\n \n");
printf("X program		-will execute the named program.\n \n");
}



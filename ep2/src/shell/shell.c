#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "command.h"

#define CWD_BUFFER 1024

int main(void) {

	char prompt[SHELL_BUFFER];
	Command *command = NULL;
	char *cwd;
	char *rl;

	cwd = getcwd(NULL, CWD_BUFFER);
	snprintf(prompt, CWD_BUFFER, "[%s] ", cwd);

	while((rl = readline(prompt)) != NULL) {
		
		/* Encerra o shell */
		if(strcmp(rl, "exit") == 0) break;

		/* Nao houve comando */
		if(strcmp(rl, "") == 0) continue; 
		
		command = command_read(rl);
			
		if(strcmp(command->argv[0], "cd") == 0) {
			chdir(command->argv[1]);

		} else if(strcmp(command->argv[0], "pwd") == 0) {
			printf("%s\n", cwd);

		} else	{
		
			if(fork() == 0) {
				
				int r = execve(command->argv[0], command->argv, NULL); 
				
				/* Trata o erro do execve */					
				if(r == -1) { 
					perror("Comando nao encontrado");
					exit(1);
				}

			} else {
				wait(NULL);
			}
		}

		add_history(rl);
		free(rl);

		free(cwd);
		cwd = getcwd(NULL, CWD_BUFFER);
		
		snprintf(prompt, CWD_BUFFER, "[%s] ", cwd);
		command_destroy(command);

	}

	free(rl);
	return 0;
}

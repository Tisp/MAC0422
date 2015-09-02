#include <stdlib.h>
#include <string.h>

#include "command.h"
#include "util.h"

int command_total_args(char *str_command) {

	int total = 0;
	char *token;
	char *aux;

	aux = strdup(str_command);
	token = strtok(aux, " ");

	while(token != NULL) {
		total++;
		token = strtok(NULL, " ");
	}

	free(aux);
	return total;
}

Command *command_read(char *str_command) {

	Command *command;
	char *token;
	char *aux;
	int total_args;
	int i = 0;


	total_args = command_total_args(str_command);
	aux = strdup(str_command);

	command = (Command *) fmalloc(sizeof(Command));
	command->argc = total_args;
	command->argv = (char **) fmalloc((total_args + 1) * sizeof(char **));

	token = strtok(aux, " ");

	while(token != NULL) {
		command->argv[i] = strdup(token);
		token = strtok(NULL, " ");
		i++;
	}

	free(aux);

	command->argv[i] = NULL;
	return command;
}

void command_destroy(Command *command) {

	int i = 0;

	for(i = 0; i < command->argc; i++)
		free(command->argv[i]);

	free(command->argv);
	free(command);

}


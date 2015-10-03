#ifndef _COMMAND_H
#define _COMMAND_H

#define SHELL_BUFFER 1024
/* 
	Struct que armazena os numero total
	e argumentos passados no console
*/
typedef struct {
	int argc;
	char **argv;
} Command;

/* Funcao que conta o numero de argumentos */
int command_total_args(char *str_command);

/* Funcao que separa os argumentos e retorna 
	um array com os argumentos. Ao final é
	acrescentado um valor nulo(NULL) que auxilia
	a ultilizacao da funcao execlp
*/
Command *command_read(char *str_command);

void command_destroy(Command *command);

#endif

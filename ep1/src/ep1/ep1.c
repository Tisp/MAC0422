#include <stdlib.h>
#include <stdio.h>
#include "threadlist.h"
#include "scheduler.h"
#include "util.h"



int main (int argc, char** argv)
{
	//redirecionamento de entrada e saida, trocar os parametros de entrada
	/*char* argv2[] = {"./"};
	int argc2 = sizeof(argv2)/sizeof(*argv2);
	argc = argc2;
	argv = argv2;*/
	//freopen("./testdata/input0", "r", stdin);
	//freopen("./testdata/out", "w", stdout);


	/*int t0;
	char name[80];
	int dt;
	int deadline;
	int p;

	while(scanf("%d %s %d %d %d\n", &t0, name, &dt, &deadline, &p) == 5)
	{
		printf("%d	%s	%d	%d	%d\n", t0, name, dt, deadline, p);
	}*/


	//cria algumas threads
	threadlist_init();

	for(int i=0; i<6; i++)
		threadlist_create();

	//loop principal, roda enquando houverem threads que ainda não terminaram
	for(int i=0; !threadlist_empty(); i++)
	{
		printf("start run %d\n", i);

		scheduler_update(); //atualiza a lista de threads que devem ser executadas de acordo com a lógica do escalonador
		threadlist_signalrun(); //roda as threads que devem ser executadas
		scheduler_wait(); //espera algo de acordo com a lógica do escalonador
		threadlist_clear(); //remove as threads que já terminaram da lista

		printf("end run %d\n\n", i);
	}

	//destruimos a estrutura para liberar a memoria
	threadlist_destroy();


	return EXIT_SUCCESS;
}

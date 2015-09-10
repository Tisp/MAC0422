#include <stdlib.h>
#include <stdio.h>
#include "threadlist.h"
#include "linkedlist.h"
#include "schedulers/test.h"
#include "util.h"


typedef struct input_item input_item;


struct input_item
{
	int t0;
	char name[100];
	int dt;
};



int main (int argc, char** argv)
{
	//redirecionamento de entrada e saida, trocar os parametros de entrada
	/*char* argv2[] = {"./"};
	int argc2 = sizeof(argv2)/sizeof(*argv2);
	argc = argc2;
	argv = argv2;*/
	freopen("./testdata/input0", "r", stdin);
	//freopen("./testdata/out", "w", stdout);


	//inicializa as estruturas e le a entrada para um lista ligada
	///@todo comentar
	void(*scheduler_update)(void) = test_update;
	void(*scheduler_wait)(void) = test_wait;
	timer cl;
	threadlist_init();
	linkedlist input = linkedlist_new();
	int t0;
	char name[100];
	int dt;

	while(scanf("%d %s %d %*d %*d\n", &t0, name, &dt) == 3)
	{
		input_item* item = fmalloc(sizeof(input_item));
		item->t0 = t0;
		item->dt = dt;
		for(int i=0; i<100; i++)
			item->name[i] = name[i];
		linkedlist_add(input, item);
	}



	//loop principal, roda enquando houverem threads que ainda não terminaram ou processos para chegar
	cl = timer_start();
	for(int i=0; !threadlist_empty() || linkedlist_size(input)>0; i++)
	{
		//cria os processos que precisar
		int time = timer_gets(cl);
		int size = linkedlist_size(input);

		for(int i=0; i<size; i++)
		{
			input_item* item = linkedlist_get(input, i);
			if(time >= item->t0)
			{
				threadlist_create(item->dt);
				linkedlist_delete(input, i);
				size--;
				i--;
			}
		}

		printf("start run %d at %ds\n", i, time);

		scheduler_update(); //atualiza a lista de threads que devem ser executadas de acordo com a lógica do escalonador
		threadlist_signalrun(); //roda as threads que devem ser executadas
		scheduler_wait(); //espera algo de acordo com a lógica do escalonador
		threadlist_clear(); //remove as threads que já terminaram da lista

		printf("end run %d\n\n", i);
	}


	//destruimos a estrutura para liberar a memoria e saimos
	threadlist_destroy();
	linkedlist_destroy(input);

	return EXIT_SUCCESS;
}

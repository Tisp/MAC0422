#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "util.h"
#include "threadlist.h"
#include "linkedlist.h"
#include "schedulers/fcfs.h"
#include "schedulers/sjf.h"
#include "schedulers/srtn.h"


//tamanho máximo do nome de processo
#define NAME_SIZE 100

///@todo pegar iss do sistema
#define CORES 2


//estrutura que guarda os dados de cada processo que vai ser criado
typedef struct input_item input_item;


struct input_item
{
	char name[NAME_SIZE];
	int t0;
	int dt;
};


//relogio global de referencia para todo o programa
timer global_clock;



int main (int argc, char** argv)
{
	freopen("./testdata/input0", "r", stdin); //redirecionamento de entrada
	setbuf(stdout, NULL); //por seguranca desligamos o buffer do stout


	///@todo processamento da linha de comando (escalonador, aquivos de entrada e saida, flag de debug)
	//ponteiros para as funcoes reais do escalonador, atualizadas de acordo com o escalonador que vai ser usado
	void(*scheduler_update)(void) = srtn_update;
	void(*scheduler_wait)(void) = srtn_wait;


	//inicializa as estruturas
	threadlist_init(CORES);
	linkedlist input = linkedlist_new(); //lista ligada que vai guardar os processos lidos do arquivo de trace


	//lemos o arquivo de entrada e adicionamos cada linha a lista ligada criada anteriormente
	int t0;
	char name[NAME_SIZE];
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


	//íniciamos o timer global imediatamente antes de começar a simulaçao
	global_clock = timer_start();

	//loop principal, roda enquando houverem threads que ainda não terminaram ou processos para chegar
	for(int i=0; !(threadlist_empty() && linkedlist_empty(input)); i++)
	{
		//varre a entrada procurando quais processos devem ser criados
		int time = timer_gets(global_clock);
		int size = linkedlist_size(input);
		for(int i=0; i<size; i++)
		{
			input_item* item = linkedlist_get(input, i);
			if(time >= item->t0)
			{
				int id = threadlist_create(item->dt);
				printf("chegou thread %d [dt=%d] at %ds\n", id, item->dt, time); ///@todo remover
				free(item);
				linkedlist_delete(input, i);
				size--;
				i--;
			}
		}

		//chama as funções do escalonador
		threadlist_lockall(); //não queremos que as threads mudem de estado enquanto o escalonador decide quais devem rodar
		scheduler_update(); //atualiza a lista de threads que devem ser executadas de acordo com a lógica do escalonador
		threadlist_unlockall();
		threadlist_signalrun(); //roda as threads que devem ser executadas
		scheduler_wait(); //espera algo de acordo com a lógica do escalonador
		usleep(1000*10); //esperamos um pouco antes de rodar outra iteração para não gastar CPU demais
	}


	//destruimos as estruturas para liberar memoria e saimos
	///@todo verificar se as estruturas estao mesmo vazias
	threadlist_destroy();
	linkedlist_destroy(input);

	return EXIT_SUCCESS;
}

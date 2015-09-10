#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "util.h"
#include "threadlist.h"
#include "linkedlist.h"
#include "schedulers/fcfs.h"
#include "schedulers/sjf.h"
#include "schedulers/srtn.h"


///@todo pegar isso do sistema
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
	fclose(stdout); ///@todo tirar

	///@todo processamento da linha de comando (escalonador, aquivos de entrada e saida, flag de debug)
	char* filename_out = "./testdata/output";
	char* filename_in = "./testdata/input0";
	int scheduler = 3; ///@todo entrada de numeros reais
	bool debug = true; ///@todo imprimir mudancas de contexto no arquivo final


	//ponteiros para as funcoes reais do escalonador, atualizadas de acordo com o escalonador que vai ser usado
	void(*scheduler_update)(void);
	void(*scheduler_wait)(void);

	switch(scheduler)
	{
		case 1:
			scheduler_update = fcfs_update;
			scheduler_wait = fcfs_wait;
			break;

		case 2:
			scheduler_update = sjf_update;
			scheduler_wait = sjf_wait;
			break;

		case 3:
			scheduler_update = srtn_update;
			scheduler_wait = srtn_wait;
			break;

		case 4:
			break;

		case 5:
			break;

		case 6:
			break;

		default:
			scheduler_update = fcfs_update;
			scheduler_wait = fcfs_wait;
			break;
	}



	//inicializa as estruturas
	FILE* stream_out = fopen(filename_out, "w");
	FILE* stream_in = fopen(filename_in, "r");
	threadlist_init(CORES, stream_out, debug);
	linkedlist input = linkedlist_new(); //lista ligada que vai guardar os processos lidos do arquivo de trace


	//lemos o arquivo de entrada e adicionamos cada linha a lista ligada criada anteriormente
	int t0;
	char name[NAME_SIZE];
	int dt;
	while(fscanf(stream_in, "%d %s %d %*d %*d\n", &t0, name, &dt) == 3)
	{
		input_item* item = fmalloc(sizeof(input_item));
		item->t0 = t0;
		item->dt = dt;
		for(int i=0; i<NAME_SIZE; i++)
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
				threadlist_create(item->name, item->dt);
				if (debug)
					fprintf(stderr, "%ds: Chegou '%s' [dt=%d]\n", time, item->name, item->dt);
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
	fclose(stream_out);
	fclose(stream_in);

	return EXIT_SUCCESS;
}

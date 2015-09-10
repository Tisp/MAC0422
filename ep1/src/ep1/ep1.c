#if __STDC_VERSION__ >= 199901L
#define _XOPEN_SOURCE 600
#else
#define _XOPEN_SOURCE 500
#endif


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "util.h"
#include "threadlist.h"
#include "linkedlist.h"
#include "schedulers/fcfs.h"
#include "schedulers/sjf.h"
#include "schedulers/srtn.h"
#include "schedulers/roundrobin.h"
#include "schedulers/realtime.h"


//estrutura que guarda os dados de cada processo que vai ser criado
typedef struct input_item input_item;


struct input_item
{
	char name[NAME_SIZE];
	int t0;
	int dt;
	int deadline;
	int priority;
};


//relogio global de referencia para todo o programa
timer global_clock;



int main (int argc, char** argv)
{
	//processamos a linha de comando
	if(argc!=4 && argc!=5)
		ERROR("Parametros de linha de comando invalidos");
	char* filename_in = argv[2];
	char* filename_out = argv[3];
	int scheduler = atoi(argv[1]);
	bool debug = false;
	if(argc == 5)
		if(argv[4][0] == 'd')
			debug = true;


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
			scheduler_update = roundrobin_update;
			scheduler_wait = roundrobin_wait;
			break;

		case 6:
			scheduler_update = realtime_update;
			scheduler_wait = realtime_wait;
			break;

		default:
			scheduler_update = fcfs_update;
			scheduler_wait = fcfs_wait;
			break;
	}


	//inicializa as estruturas
	FILE* stream_out = fopen(filename_out, "w");
	FILE* stream_in = fopen(filename_in, "r");
	threadlist_init(sysconf(_SC_NPROCESSORS_CONF), stream_out, debug);
	linkedlist input = linkedlist_new(); //lista ligada que vai guardar os processos lidos do arquivo de trace
	roundrobin_init();


	//lemos o arquivo de entrada e adicionamos cada linha a lista ligada criada anteriormente
	float t0;
	char name[NAME_SIZE];
	float dt;
	float deadline;
	int priority;
	while(fscanf(stream_in, "%f %s %f %f %d\n", &t0, name, &dt, &deadline, &priority) == 5)
	{
		input_item* item = fmalloc(sizeof(input_item));
		item->t0 = 1000*t0;
		item->dt = 1000*dt;
		item->deadline = 1000*deadline;
		item->priority = priority;
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
		int time = timer_getms(global_clock);
		int size = linkedlist_size(input);
		for(int i=0; i<size; i++)
		{
			input_item* item = linkedlist_get(input, i);
			if(time >= item->t0)
			{
				threadlist_create(item->name, item->dt, item->deadline);
				if (debug)
					fprintf(stderr, "%.1fs: Chegou '%s' [t0=%.1f dt=%.1f deadline=%.1f priority=%d]\n", time/1000.0, item->name, time/1000.0, item->dt/1000.0, item->deadline/1000.0, item->priority);
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
	}


	//destruimos as estruturas para liberar memoria e saimos
	threadlist_destroy();
	linkedlist_destroy(input);
	fclose(stream_out);
	fclose(stream_in);
	roundrobin_destroy();

	return EXIT_SUCCESS;
}

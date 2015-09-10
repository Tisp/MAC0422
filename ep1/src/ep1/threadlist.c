#include <pthread.h>
#include <stdio.h>
#include "threadlist.h"
#include "linkedlist.h"
#include "util.h"


//guarda uma thread e seus parametros
typedef struct thread thread;


struct thread
{
	pthread_t thread; //thread em si
	char name[NAME_SIZE];
	int id; //id da thread
	bool finished; //indica se a thread ja terminou
	bool torun; //indica se a thread deve executar
	int dt; //tempo total que a thread deve executar
	long int runtimems; //tempo que a thread ja rodou em ms
};


static linkedlist threadlist; //lista ligada que guarda as threads
static int cores; //numero de cores simulados
static FILE* out_stream; //stream onde vai ser impressa a saida
static bool debug; //flag que indica se as menssagens de debug devem ser impressas

//variaveis de condição (e mutexes associados) que sinalizam quando as threads devem ver se vão executar e quando alguma thread terminou
static pthread_mutex_t torun_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t torun_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t finished_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t finished_cond = PTHREAD_COND_INITIALIZER;



//funcao que verifica se a thread deve ser executada, mede o tempo de execução, executa uma operação dummy para gastar CPU e sinaliza quando a thread terminou. Recebe como parametro a thread correspondente a instancia que está sendo executada
static void thread_func (thread* th)
{
	while(th->runtimems < th->dt) //continua rodando até dar o tempo
	{
		//verifica se essa thread deve executar, caso não espera um sinal para verificar de novo
		///@todo tirar informacoes extras dos prints
		pthread_mutex_lock(&torun_mutex);
		bool parou = false;
		if(th->torun == false)
		{
			parou = true;
			if(th->runtimems>0)
			{
				context_changes++;
				if(debug)
				{
					fprintf(stderr, "%.1fs: Pausando '%s' [dt=%.f2] [runtime=%.1f]\n", timer_getms(global_clock)/1000.0, th->name, th->dt/1000.0, th->runtimems/1000.0);
					fprintf(stderr, "Mudancas de contexto: %d\n", context_changes);
				}
			}
			while(th->torun == false)
				pthread_cond_wait(&torun_cond, &torun_mutex);
		}
		pthread_mutex_unlock(&torun_mutex);

		timer t = timer_start(); //comecamos a contar o tempo de execucao da thread

		//executa alguma coisa
		if(debug && parou)
			fprintf(stderr, "%.1fs: Rodando '%s' [dt=%.1f] [runtime=%.1f]\n", timer_getms(global_clock)/1000.0, th->name, th->dt/1000.0, th->runtimems/1000.0);
		usleep(1000*100); ///@todo gastar cpu de verdade

		th->runtimems += timer_getms(t); //somamamos o tempo dessa iteração ao tempo total
	}

	//marca que a thread terminou e sinaliza isso
	pthread_mutex_lock(&finished_mutex);
	if(debug)
	{
		fprintf(stderr, "%.1fs: Terminando '%s' [dt=%.1f] [runtime=%.1f]\n", timer_getms(global_clock)/1000.0, th->name, th->dt/1000.0, th->runtimems/1000.0);
		fprintf(stderr, "Linha de saida: %s %.1f %.1f\n", th->name, timer_getms(global_clock)/1000.0, th->runtimems/1000.0);
	}
	fprintf(out_stream, "%s %.1f %.1f\n", th->name, timer_getms(global_clock)/1000.0, th->runtimems/1000.0);
	th->finished = true;
	pthread_cond_signal(&finished_cond);
	pthread_mutex_unlock(&finished_mutex);
}



//varre a lista de threads procurando a thread com o ID especificado
static thread* get_thread (int id)
{
	thread* th;
	int i = 0;

	do
	{
		th = (thread*)linkedlist_get(threadlist, i);
		i++;
	} while(th->id != id);

	return th;
}



//remove uma thread que já terminou da lista de threads
static void remove_thread (thread* th, int i)
{
	pthread_join(th->thread, NULL);
	free(th);
	linkedlist_delete(threadlist, i);
}



void threadlist_init (int ncores, FILE* output, bool dbg)
{
	//simplesmente iniciamos a lista ligada e copiamos os parametros
	threadlist = linkedlist_new();
	cores = ncores;
	out_stream = output;
	debug = dbg;
	context_changes = 0;
}



void threadlist_destroy ()
{
	//guardamos o numero de trocas de contexto
	fprintf(out_stream, "%d\n", context_changes);
	//excluimos todas as threads que restam na lista e depois destruimos a lista em sí
	int size = linkedlist_size(threadlist);
	for(int i=0; i<size; i++)
	{
		thread* th = (thread*)linkedlist_get(threadlist, 0);
		pthread_join(th->thread, NULL);
		free(th);
		linkedlist_delete(threadlist, 0);
	}

	linkedlist_destroy(threadlist);
}



int threadlist_create (char* name, int dt)
{
	//simplesmente alocamos uma nova thread, copiamos os parametros, iniciamos ela e a adicionamos na lista ligada
	static int id = 0; //jeito simples de lembrar qual foi o ultimo ID usado
	thread* add = fmalloc(sizeof(thread));
	linkedlist_add(threadlist, add);

	add->id = id;
	add->finished = false;
	add->torun = false;
	add->dt = dt;
	add->runtimems = 0;
	for(int i=0; i<NAME_SIZE; i++)
		add->name[i] = name[i];
	pthread_create(&add->thread, NULL, (void*(*)(void*))thread_func, (void*)add);

	id++;
	return id-1;
}



void threadlist_remove (int id)
{
	//varremos a lista de threads
	int size = linkedlist_size(threadlist);
	for(int i=0; i<size; i++)
	{
		thread* th = (thread*)linkedlist_get(threadlist, i);
		if(th->id == id) //achamos a que queriamos
		{
			remove_thread(th, i);
			return;
		}
	}
}



int threadlist_clear ()
{
	//travamos o mutex pois se alguma thread terminar enquando varremos a lista poderiam haver problemas
	pthread_mutex_lock(&finished_mutex);

	int removed = 0;

	//varremos a lista de threads
	int size = linkedlist_size(threadlist);
	for(int i=0; i<size; i++)
	{
		thread* th = (thread*)linkedlist_get(threadlist, i);
		if(th->finished == true) //achamos uma que terminou
		{
			remove_thread(th, i);
			removed++;
			i--;
			size--;
		}
	}

	pthread_mutex_unlock(&finished_mutex);

	return removed;
}



void threadlist_marktorun (int id)
{
	//simplesmente mudamos a flag dela, quando sinalizarmos ela vai rodar
	get_thread(id)->torun = true;
}



void threadlist_markstop (int id)
{
	//simplesmente mudamos a flag dela, quando terminar a iteração atual ela vai parar
	get_thread(id)->torun = false;
}



void threadlist_signalrun ()
{
	//mandamos o sinal para todas as threads verificarem se devem rodar
	pthread_mutex_lock(&torun_mutex);
	pthread_cond_broadcast(&torun_cond);
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_wait (int id)
{
	///@todo remover?
	pthread_mutex_lock(&finished_mutex);

	thread* th = get_thread(id);

	//se a thread que estamos esperando nao terminou, esperamos alguma terminar e vemos se é a que nos interessa que terminou
	while(th->finished == false)
		pthread_cond_wait(&finished_cond, &finished_mutex);

	th->torun = false;

	pthread_mutex_unlock(&finished_mutex);
}



void threadlist_waitany ()
{
	///@todo remover?
	pthread_mutex_lock(&finished_mutex);

	bool any_finished;
	do
	{
		any_finished = false;

		int size = linkedlist_size(threadlist);
		for(int i=0; i<size; i++)
			any_finished = any_finished || ((thread*)linkedlist_get(threadlist, i))->finished;

		if(!any_finished)
			pthread_cond_wait(&finished_cond, &finished_mutex);
	} while(!any_finished);

	pthread_mutex_unlock(&finished_mutex);
}



int threadlist_getid (int index)
{
	return ((thread*)linkedlist_get(threadlist, index))->id;
}



/*int threadlist_getid_stopped (int index)
{
	ATENCAO, IMPLEMENTACAO MUITO PROVAVELMENTE BUGADA
	@todo comentar e adicionar vierificacoes
	int curr = -1;
	thread* th;
	int size = linkedlist_size(threadlist);
	for(int i=0; i<size && curr!=index; i++)
	{
		th = (thread*)linkedlist_get(threadlist, i);
		if(th->torun==false && th->finished==false)
			curr++;
	}

	return th->id;
}*/



bool threadlist_running (int id)
{
	return get_thread(id)->torun;
}



int threadlist_get_dt (int id)
{
	return get_thread(id)->dt;
}



int threadlist_get_remainingms (int id)
{
	return (get_thread(id)->dt*1000) - get_thread(id)->runtimems;
}



int threadlist_size ()
{
	return linkedlist_size(threadlist);
}



bool threadlist_empty ()
{
	return linkedlist_size(threadlist) == 0;
}



int threadlist_ncores ()
{
	return cores;
}



void threadlist_lockall ()
{
	pthread_mutex_lock(&torun_mutex);
	pthread_mutex_lock(&finished_mutex);
}



void threadlist_unlockall ()
{
	pthread_mutex_unlock(&torun_mutex);
	pthread_mutex_unlock(&finished_mutex);
}

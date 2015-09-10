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
	int id; //id da thread
	bool finished; //indica se a thread ja terminou
	bool torun; //indica se a thread deve executar

	int dt; //tempo total que a thread deve executar
};


static linkedlist threadlist; //lista ligada que guarda as threads
static int cores; //numero de cores simulados

//variaveis de condição (e mutexes associados) que sinalizam quando as threads devem ver se vão executar e quando alguma thread terminou
static pthread_mutex_t torun_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t torun_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t finished_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t finished_cond = PTHREAD_COND_INITIALIZER;



//funcao que verifica se a thread deve ser executada, mede o tempo de execução, executa uma operação dummy para gastar CPU e sinaliza quando a thread terminou. Recebe como parametro a thread correspondente a instancia que está sendo executada
static void thread_func (thread* curr_thread)
{
	long int total_timems = 0; ///@todo colocar isso dentro do struct de thread
	int i = 0; ///@todo remover

	while(total_timems < 1000*(curr_thread->dt)) //continua rodando até dar o tempo
	{
		//verifica se essa thread deve executar, caso não espera um sinal para verificar de novo
		pthread_mutex_lock(&torun_mutex);
		while(curr_thread->torun == false)
			pthread_cond_wait(&torun_cond, &torun_mutex);
		pthread_mutex_unlock(&torun_mutex);
		timer t = timer_start(); //comecamos a contar o tempo de execucao da thread

		//executa alguma coisa
		if(i == 0) ///@todo remover
			printf("start thread %d at %ds\n", curr_thread->id, timer_gets(global_clock));
		usleep(1000*100); ///@todo gastar cpu de verdade

		total_timems += timer_getms(t); //somamamos o tempo dessa iteração ao tempo total
		i++; ///@todo remover
	}

	//marca que a thread terminou e sinaliza isso
	pthread_mutex_lock(&finished_mutex);
	printf("end thread %d at %ds\n", curr_thread->id, timer_gets(global_clock)); ///@todo remover
	///@todo gravar no struct qual o tempo que terminou a thread de acordo com o relógio global
	curr_thread->finished = true;
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
		///@todo verificar se ja passou ou chegou no fim
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



void threadlist_init (int ncores)
{
	//simplesmente iniciamos a lista ligada e copiamos os parametros
	threadlist = linkedlist_new();
	cores = ncores;
}



void threadlist_destroy ()
{
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



int threadlist_create (int dt)
{
	//simplesmente alocamos uma nova thread, copiamos os parametros, iniciamos ela e a adicionamos na lista ligada
	static int id = 0; //jeito simples de lembrar qual foi o ultimo ID usado
	thread* add = fmalloc(sizeof(thread));
	linkedlist_add(threadlist, add);

	add->id = id;
	add->finished = false;
	add->torun = false;
	add->dt = dt;
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
	pthread_mutex_lock(&torun_mutex);
	///@todo verificar se o numero de cores é respeitado sempre que for colocar uma thread pra rodar
	get_thread(id)->torun = true;
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_markstop (int id)
{
	//simplesmente mudamos a flag dela, quando terminar a iteração atual ela vai parar
	pthread_mutex_lock(&torun_mutex);
	get_thread(id)->torun = false;
	pthread_mutex_unlock(&torun_mutex);
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
	///@todo comentar
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



int threadlist_dt (int id)
{
	return get_thread(id)->dt;
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

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "threadlist.h"
#include "linkedlist.h"


typedef struct thread thread;


//dados de cada thread
struct thread
{
	pthread_t thread; //thread em si
	int id; //id da thread
	bool finished; //indica se a thread ja terminou
	bool torun; //indica se a thread deve executar

	int dt; //tempo que a thread deve executar
};


linkedlist threadlist; ///@todo static //lista ligada que guarda as threads

//variaveis de condição (e mutexes associados) que sinalizam quando as threads devem ver se vão executar e quando alguma thread terminou
static pthread_mutex_t torun_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t torun_cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t finished_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t finished_cond = PTHREAD_COND_INITIALIZER;



//funcao que verifica se a thread deve ser executada, mede o tempo de execução, executa uma operação dummy para gastar CPU e sinaliza quando a thread terminou
static void thread_func (thread* curr_thread)
{
	long int total_timems = 0;
	static int i = 0; ///@todo remover

	while(total_timems < 1000*curr_thread->dt) //continua rodando até dar o tempo
	{
		//verifica se essa thread deve executar, caso não espera um sinal para verificar de novo
		pthread_mutex_lock(&torun_mutex);
		while(curr_thread->torun == false)
			pthread_cond_wait(&torun_cond, &torun_mutex);
		pthread_mutex_unlock(&torun_mutex);
		timer t = timer_start(); //comecamos a contar o tempo de execucao da thread

		//executa alguma coisa
		printf("running thread %d iter %d\n", curr_thread->id, i);
		usleep(1000*1000*0.5);

		total_timems += timer_getms(t); //somamamos o tempo dessa iteração ao tempo total
		i++; ///@todo remover
	}

	//marca que a thread terminou e sinaliza isso
	pthread_mutex_lock(&finished_mutex);
	printf("finished thread %d\n", curr_thread->id); ///@todo remover
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
		th = (thread*)linkedlist_get(threadlist, i);
		i++;
	} while(th->id != id);

	return th;
}



void threadlist_init ()
{
	srand(109283); ///@todo remover
	threadlist = linkedlist_new();
}



void threadlist_destroy ()
{
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
	static int id = 0;
	thread* add = fmalloc(sizeof(thread));
	linkedlist_add(threadlist, add);

	add->id = id;
	add->finished = false;
	add->torun = false;
	add->dt = dt;
	pthread_create(&add->thread, NULL, (void*(*)(void*))thread_func, (void*)add);

	id++;
	return id;
}



void threadlist_remove (int id)
{
	int size = linkedlist_size(threadlist);
	for(int i=0; i<size; i++)
	{
		thread* th = (thread*)linkedlist_get(threadlist, i);
		if(th->id == id)
		{
			pthread_join(th->thread, NULL);
			free(th);
			linkedlist_delete(threadlist, i);
			return;
		}
	}
}



void threadlist_clear ()
{
	pthread_mutex_lock(&finished_mutex);

	int size = linkedlist_size(threadlist);
	for(int i=0; i<size; i++)
	{
		thread* th = (thread*)linkedlist_get(threadlist, i);
		if(th->finished == true)
		{
			pthread_join(th->thread, NULL);
			free(th);
			linkedlist_delete(threadlist, i);
			i--;
			size--;
		}
	}

	pthread_mutex_unlock(&finished_mutex);
}



void threadlist_marktorun (int id)
{
	pthread_mutex_lock(&torun_mutex);
	get_thread(id)->torun = true;
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_markstop (int id)
{
	pthread_mutex_lock(&torun_mutex);
	get_thread(id)->torun = false;
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_signalrun ()
{
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



bool threadlist_empty ()
{
	return linkedlist_size(threadlist) == 0;
}

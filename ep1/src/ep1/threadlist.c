#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "threadlist.h"



thread threadlist[THREADS];

pthread_mutex_t torun_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t torun_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t finished_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t finished_cond = PTHREAD_COND_INITIALIZER;



static void thread_func (uintptr_t threadnum)
{
	for(int i=0; i<THREAD_MESSAGES; i++)
	{
		pthread_mutex_lock(&torun_mutex);
		while(threadlist[threadnum].torun == false)
			pthread_cond_wait(&torun_cond, &torun_mutex);
		threadlist[threadnum].torun = false;
		pthread_mutex_unlock(&torun_mutex);

		printf("start thread %" PRIxPTR " msg %d\n", threadnum, i);
		RAND_WAIT();
		printf("end thread %" PRIxPTR " msg %d\n", threadnum, i);

		pthread_mutex_lock(&finished_mutex);
		threadlist[threadnum].finished = true;
		pthread_cond_signal(&finished_cond);
		pthread_mutex_unlock(&finished_mutex);
	}
}



void threadlist_init (int size)
{
	srand(109283);

	for(int i=0; i<THREADS; i++)
	{
		threadlist[i].finished = true;
		threadlist[i].torun = false;
		pthread_create(&threadlist[i].thread, NULL, (void*(*)(void*))thread_func, (void*)(uintptr_t)i);
	}
}



void threadlist_destroy (void)
{
	for(int i=0; i<THREADS; i++)
		pthread_join(threadlist[i].thread, NULL);
}



void threadlist_marktorun (int index)
{
	pthread_mutex_lock(&torun_mutex);
	pthread_mutex_lock(&finished_mutex);
	if(threadlist[index].finished==false || threadlist[index].torun==true)
		ERROR("tentou marcar para rodar thread que ja esta rodando");

	threadlist[index].finished = false;
	threadlist[index].torun = true;
	pthread_mutex_unlock(&finished_mutex);
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_run (void)
{
	pthread_mutex_lock(&torun_mutex);
	pthread_cond_broadcast(&torun_cond);
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_waitall (void)
{
	pthread_mutex_lock(&finished_mutex);

	bool all_finished = false;
	while(!all_finished)
	{
		all_finished = true;
		for(int i=0; i<THREADS; i++)
		{
			if(threadlist[i].finished==true && threadlist[i].torun==true)
				ERROR("thread acabou antes de comecar");

			all_finished = all_finished && threadlist[i].finished;
		}

		if(!all_finished)
			pthread_cond_wait(&finished_cond, &finished_mutex);
	}

	pthread_mutex_unlock(&finished_mutex);
}

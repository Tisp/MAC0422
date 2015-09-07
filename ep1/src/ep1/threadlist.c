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
	int i = 0;

	while(i < THREAD_MESSAGES)
	{
		pthread_mutex_lock(&torun_mutex);
		while(threadlist[threadnum].torun == false)
			pthread_cond_wait(&torun_cond, &torun_mutex);
		pthread_mutex_unlock(&torun_mutex);

		printf("start thread %" PRIxPTR " msg %d\n", threadnum, i);
		RAND_WAIT();
		printf("end thread %" PRIxPTR " msg %d\n", threadnum, i);
		i++;
	}

	pthread_mutex_lock(&finished_mutex);
	threadlist[threadnum].finished = true;
	pthread_cond_signal(&finished_cond);
	pthread_mutex_unlock(&finished_mutex);
}



void threadlist_init (int size)
{
	srand(109283);

	for(int i=0; i<THREADS; i++)
	{
		threadlist[i].finished = false;
		threadlist[i].torun = false;
		pthread_create(&threadlist[i].thread, NULL, (void*(*)(void*))thread_func, (void*)(uintptr_t)i);
	}
}



void threadlist_destroy ()
{
	for(int i=0; i<THREADS; i++)
		pthread_join(threadlist[i].thread, NULL);
}



void threadlist_marktorun (int index)
{
	pthread_mutex_lock(&torun_mutex);
	pthread_mutex_lock(&finished_mutex);

	threadlist[index].torun = true;

	pthread_mutex_unlock(&finished_mutex);
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_stop (int index)
{
	pthread_mutex_lock(&torun_mutex);
	pthread_mutex_lock(&finished_mutex);

	threadlist[index].torun = false;

	pthread_mutex_unlock(&finished_mutex);
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_signalrun ()
{
	pthread_mutex_lock(&torun_mutex);
	pthread_cond_broadcast(&torun_cond);
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_wait (int index)
{
	pthread_mutex_lock(&finished_mutex);

	while(threadlist[index].finished == false)
		pthread_cond_wait(&finished_cond, &finished_mutex);

	threadlist[index].torun = false;

	pthread_mutex_unlock(&finished_mutex);
}



void threadlist_waitany ()
{
	pthread_mutex_lock(&finished_mutex);

	bool any_finished = false;
	while(!any_finished)
	{
		any_finished = false;
		for(int i=0; i<THREADS; i++)
		{
			if(threadlist[i].finished==true && threadlist[i].torun==true)
			{
				threadlist[i].torun = false;
				any_finished = true;
			}
		}

		if(!any_finished)
			pthread_cond_wait(&finished_cond, &finished_mutex);
	}

	pthread_mutex_unlock(&finished_mutex);
}



bool threadlist_empty ()
{
	pthread_mutex_lock(&finished_mutex);

	bool all_finished = true;

	for(int i=0; i<THREADS; i++)
		all_finished = all_finished && threadlist[i].finished;

	pthread_mutex_unlock(&finished_mutex);

	return all_finished;
}

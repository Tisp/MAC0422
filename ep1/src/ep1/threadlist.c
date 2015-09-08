#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "threadlist.h"
#include "linkedlist.h"


pthread_mutex_t torun_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t torun_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t finished_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t finished_cond = PTHREAD_COND_INITIALIZER;



static void thread_func (uintptr_t threadnum)
{
	int i = 0;
	thread* curr_thread = linkedlist_get(threadnum);

	while(i < THREAD_MESSAGES)
	{
		pthread_mutex_lock(&torun_mutex);
		while(curr_thread->torun == false)
			pthread_cond_wait(&torun_cond, &torun_mutex);
		pthread_mutex_unlock(&torun_mutex);

		printf("start thread %" PRIxPTR " msg %d\n", threadnum, i);
		RAND_WAIT();
		printf("end thread %" PRIxPTR " msg %d\n", threadnum, i);
		i++;
	}

	pthread_mutex_lock(&finished_mutex);
	curr_thread->finished = true;
	pthread_cond_signal(&finished_cond);
	pthread_mutex_unlock(&finished_mutex);
}



void threadlist_init ()
{
	srand(109283);
	linkedlist_init();

	for(int i=0; i<THREADS; i++)
	{
		thread* add = fmalloc(sizeof(thread));

		add->finished = false;
		add->torun = false;
		pthread_create(&add->thread, NULL, (void*(*)(void*))thread_func, (void*)(uintptr_t)i);

		linkedlist_add(add);
	}
}



void threadlist_destroy ()
{
	for(int i=0; i<THREADS; i++)
		pthread_join(linkedlist_get(i)->thread, NULL);

	linkedlist_destroy();
}



void threadlist_marktorun (int index)
{
	pthread_mutex_lock(&torun_mutex);
	pthread_mutex_lock(&finished_mutex);

	linkedlist_get(index)->torun = true;

	pthread_mutex_unlock(&finished_mutex);
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_markstop (int index)
{
	pthread_mutex_lock(&torun_mutex);
	pthread_mutex_lock(&finished_mutex);

	linkedlist_get(index)->torun = false;

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

	thread* th = linkedlist_get(index);

	while(th->finished == false)
		pthread_cond_wait(&finished_cond, &finished_mutex);

	th->torun = false;

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
			thread* tr = linkedlist_get(i);

			if(tr->finished==true && tr->torun==true)
			{
				tr->torun = false;
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
		all_finished = all_finished && linkedlist_get(i)->finished;

	pthread_mutex_unlock(&finished_mutex);

	return all_finished;
}

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "threadlist.h"
#include "linkedlist.h"


typedef struct thread thread;


struct thread
{
	pthread_t thread;
	int id;
	bool finished;
	bool torun;
};


pthread_mutex_t torun_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t torun_cond = PTHREAD_COND_INITIALIZER;

pthread_mutex_t finished_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t finished_cond = PTHREAD_COND_INITIALIZER;



static thread* get_thread (int id)
{
	thread* th;
	int i = 0;

	do
	{
		th = linkedlist_get(i);
		i++;
	} while(th->id != id);

	return th;
}



static void thread_func (uintptr_t id)
{
	int i = 0;
	thread* curr_thread = get_thread((int)id);

	while(i < 6)
	{
		pthread_mutex_lock(&torun_mutex);
		while(curr_thread->torun == false)
			pthread_cond_wait(&torun_cond, &torun_mutex);
		pthread_mutex_unlock(&torun_mutex);

		printf("start thread %d msg %d\n", curr_thread->id, i);
		RAND_WAIT();
		printf("end thread %d msg %d\n", curr_thread->id, i);
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
}



void threadlist_destroy ()
{
	int size = linkedlist_size();
	for(int i=0; i<size; i++)
		pthread_join(linkedlist_get(i)->thread, NULL);

	linkedlist_destroy();
}



int threadlist_create ()
{
	static int id = 0;
	thread* add = fmalloc(sizeof(thread));

	add->id = id;
	add->finished = false;
	add->torun = false;
	pthread_create(&add->thread, NULL, (void*(*)(void*))thread_func, (void*)(uintptr_t)id);

	id++;
	linkedlist_add(add);

	return id;
}



void threadlist_remove (int id)
{
	int size = linkedlist_size();
	for(int i=0; i<size; i++)
	{
		thread* th = linkedlist_get(i);
		if(th->id == id)
		{
			pthread_join(th->thread, NULL);
			linkedlist_delete(i);
			return;
		}
	}
}



void threadlist_clear ()
{
	pthread_mutex_lock(&finished_mutex);

	int size = linkedlist_size();
	for(int i=0; i<size; i++)
	{
		thread* th = linkedlist_get(i);
		if(th->finished == true)
		{
			pthread_join(th->thread, NULL);
			linkedlist_delete(i);
			i--;
			size--;
		}
	}

	pthread_mutex_unlock(&finished_mutex);
}



void threadlist_marktorun (int id)
{
	pthread_mutex_lock(&torun_mutex);
	pthread_mutex_lock(&finished_mutex);

	get_thread(id)->torun = true;

	pthread_mutex_unlock(&finished_mutex);
	pthread_mutex_unlock(&torun_mutex);
}



void threadlist_markstop (int id)
{
	pthread_mutex_lock(&torun_mutex);
	pthread_mutex_lock(&finished_mutex);

	get_thread(id)->torun = false;

	pthread_mutex_unlock(&finished_mutex);
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

		int size = linkedlist_size();
		for(int i=0; i<size; i++)
			any_finished = any_finished || linkedlist_get(i)->finished;

		if(!any_finished)
			pthread_cond_wait(&finished_cond, &finished_mutex);
	} while(!any_finished);

	pthread_mutex_unlock(&finished_mutex);
}



bool threadlist_empty ()
{
	return linkedlist_size() == 0;
}

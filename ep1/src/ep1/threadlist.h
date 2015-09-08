#ifndef THREADLIST_H
#define THREADLIST_H


#include <unistd.h>
#include <pthread.h>
#include "util.h"


#define THREADS 6
#define PARALLEL 2
#define THREAD_MESSAGES 6


#define RAND_WAIT() usleep(10000+rand()%10000)


typedef struct thread thread;


struct thread
{
	pthread_t thread;
	bool finished;
	bool torun;
};


void threadlist_init (int size);

void threadlist_marktorun (int index);

void threadlist_stop (int index);

void threadlist_signalrun ();

void threadlist_wait (int index);

void threadlist_waitany ();

void threadlist_destroy ();

bool threadlist_empty ();
#endif

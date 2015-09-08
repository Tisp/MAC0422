#ifndef THREADLIST_H
#define THREADLIST_H


#include <unistd.h>
#include <pthread.h>
#include "util.h"


#define THREADS 6
#define PARALLEL 2
#define THREAD_MESSAGES 6


#define RAND_WAIT() usleep(rand()%10000)


typedef struct thread thread;


struct thread
{
	pthread_t thread;
	bool finished;
	bool torun;
};


void threadlist_init ();

void threadlist_marktorun (int index);

void threadlist_markstop (int index);

void threadlist_signalrun ();

void threadlist_wait (int index);

void threadlist_waitany ();

void threadlist_destroy ();

bool threadlist_empty ();
#endif

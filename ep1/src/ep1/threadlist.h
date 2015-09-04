#ifndef THREAD_LIST_H
#define THREAD_LIST_H


#include <unistd.h>
#include <pthread.h>
#include "util.h"


#define THREADS 4
#define PARALLEL 3
#define THREAD_MESSAGES 6


#define RAND_WAIT() usleep(10000+rand()%10000)


typedef struct thread thread;


struct thread
{
    pthread_t thread;
    bool finished;
    bool torun;
};


//init, mark-torun, wait
void threadlist_init (int size);

void threadlist_marktorun (int index);

void threadlist_run (void);

void threadlist_waitall (void);

void threadlist_destroy (void);
#endif

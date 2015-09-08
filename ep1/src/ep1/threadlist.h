#ifndef THREADLIST_H
#define THREADLIST_H


#include <unistd.h>
#include <pthread.h>
#include "util.h"


#define RAND_WAIT() usleep(rand()%10000)


void threadlist_init ();

void threadlist_destroy ();

int threadlist_create ();

void threadlist_remove (int id);

void threadlist_clear ();

void threadlist_marktorun (int id);

void threadlist_markstop (int id);

void threadlist_signalrun ();

void threadlist_wait (int id);

void threadlist_waitany ();

bool threadlist_empty ();
#endif

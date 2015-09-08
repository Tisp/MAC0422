#include "scheduler.h"
#include "threadlist.h"



void scheduler_update ()
{
	static int i = 0;

	threadlist_marktorun(i);
	threadlist_marktorun(i+1);

	i += 2;
}



void scheduler_wait ()
{
	static int i = 0;

	threadlist_wait(i);
	threadlist_remove(i);
	threadlist_waitany();

	i += 2;
}

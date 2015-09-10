#include "scheduler.h"
#include "threadlist.h"
#include "linkedlist.h"


extern linkedlist threadlist;



void scheduler_update ()
{
	static int i = 0;

	if(linkedlist_size(threadlist) < 2)
		return;

	threadlist_marktorun(i);
	threadlist_marktorun(i+1);

	i += 2;
}



void scheduler_wait ()
{
	static int i = 0;

	if(linkedlist_size(threadlist) < 2)
	{
		usleep(1000*1000*0.5);
		return;
	}

	threadlist_wait(i);
	threadlist_remove(i);
	threadlist_waitany();

	i += 2;
}

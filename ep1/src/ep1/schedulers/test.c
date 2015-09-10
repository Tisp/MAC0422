#include "test.h"
#include "../threadlist.h"


#define PARALLEL 2



void test_update ()
{
	int size = threadlist_size();
	for(int i=0; i<size && i<PARALLEL; i++)
		threadlist_marktorun(threadlist_getid(i));
}



void test_wait ()
{
	int size = threadlist_size();
	for(int i=0; i<size && i<PARALLEL; i++)
		threadlist_wait(threadlist_getid(i));
}

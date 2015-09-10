#include "test.h"
#include "../threadlist.h"



void test_update ()
{
	int cores = threadlist_ncores();
	int size = threadlist_size();
	for(int i=0; i<size && i<cores; i++)
		threadlist_marktorun(threadlist_getid(i));
}



void test_wait ()
{
	int cores = threadlist_ncores();
	int size = threadlist_size();
	for(int i=0; i<size && i<cores; i++)
		threadlist_wait(threadlist_getid(i));

	threadlist_clear();
}

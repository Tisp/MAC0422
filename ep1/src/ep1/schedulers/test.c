#include "test.h"
#include "../threadlist.h"
#include "linkedlist.h"


extern linkedlist threadlist;

int parallel = 2;



void test_update ()
{
	int size = threadlist_size();
	for(int i=0; i<size && i<parallel; i++)
		threadlist_marktorun(threadlist_getid(i));
}



void test_wait ()
{
	int size = threadlist_size();
	for(int i=0; i<size && i<parallel; i++)
		threadlist_wait(threadlist_getid(i));
}

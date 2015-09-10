#include "roundrobin.h"
#include "../threadlist.h"
#include "util.h"


#define QUANTUM 1000*1000*0.1


static int cores;
static int* id_running;



void roundrobin_init ()
{
	cores = threadlist_ncores();
	id_running = fmalloc(sizeof(int) * cores);
	for(int i=0; i<cores; i++)
		id_running[i] = -1;
}



void roundrobin_destroy ()
{
	free(id_running);
}



void roundrobin_update ()
{
	static int last_running = -1;
	int new_last_running = -1;
	int ran = 0;
	bool first = true;
	int torun = 0;

	for(int i=0; i<cores; i++)
		if(id_running[i] == -1)
			torun++;

	int size = threadlist_size();
	for(int i=0; i<size && ran<torun; i++)
	{
		int id = threadlist_getid(i);

		if(id>last_running || first==false)
		{
			threadlist_marktorun(id);
			id_running[ran] = id;
			ran++;
			new_last_running = id;
		}
		else
		{
			if(i==size-1 && first==true)
			{
				first = false;
				i = 0;
				last_running = -1;
			}
		}
	}

	last_running = new_last_running;
}



void roundrobin_wait ()
{
	usleep(QUANTUM);

	int size = threadlist_size();

	int ran;
	for(int i=0; i<cores; i++)
		if(id_running[i] != -1)
			ran++;

	int tostop = size-ran;
	if(tostop > cores)
		tostop = cores;

    for(int i=0; i<tostop; i++)
	{
        threadlist_markstop(id_running[i]);
        id_running[i] = -1;
	}


}

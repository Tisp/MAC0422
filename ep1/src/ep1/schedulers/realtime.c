#include "realtime.h"
#include "../threadlist.h"


static int occupied = 0;



void realtime_update ()
{
	int size = threadlist_size();
	for(int i=0; i<size && occupied<threadlist_ncores(); i++)
	{
		int id = threadlist_getid(i);
		if(!threadlist_running(id))
		{
			bool run = true;

			for(int j=0; j<size; j++)
			{
				int id2 = threadlist_getid(j);
				if(!threadlist_running(id2))
				{
                    if(threadlist_get_deadline(id) > threadlist_get_deadline(id2))
						run = false;
				}
			}

			if(run)
			{
				threadlist_marktorun(id);
				occupied++;
				i = 0;
			}
		}

	}
}



void realtime_wait ()
{
	occupied -= threadlist_clear();
	usleep(1000*10);
}

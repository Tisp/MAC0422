#include "fcfs.h"
#include "../threadlist.h"


//numero de cores ocupados
static int occupied = 0;



void fcfs_update ()
{
	//colocamos threads para rodar na ordem que chegaram até encher todos os cores disponiveis ou não terem mais threads
	int size = threadlist_size();
	for(int i=0; i<size && occupied<threadlist_ncores(); i++)
	{
		int id = threadlist_getid(i);
		if(!threadlist_running(id))
		{
			threadlist_marktorun(id);
			occupied++;
		}
	}
}



void fcfs_wait ()
{
	//atualizamos o numero de cores livres
	occupied -= threadlist_clear();
	usleep(1000*10); //esperamos um pouco antes de rodar outra iteração para não gastar CPU demais
}

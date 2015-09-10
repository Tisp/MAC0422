#include "srtn.h"
#include "../threadlist.h"


//numero de cores ocupados
static int occupied = 0;



void srtn_update ()
{
	//primeiro preenchemos os cores com threads quaisquer
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

	if(occupied < threadlist_ncores()) //não há threads suficientes para ocupar todos os cores
		return;


	//agora percorremos as threads que estão rodando substituindo elas por threads que faltem menos tempo, se houverem
	for(int i=0; i<size; i++)
	{
		int id = threadlist_getid(i);
		if(threadlist_running(id))
		{
			for(int j=0; j<size; j++)
			{
				int id2 = threadlist_getid(j);
				if(!threadlist_running(id2))
				{
                    if(threadlist_get_remainingms(id) > threadlist_get_remainingms(id2)) //achamos uma thread que nao esta rodando com tempo menor
					{
						//tiramos a que estava rodando e colocamos ela no lugar, e reiniciamos a busca
						threadlist_markstop(id);
						threadlist_marktorun(id2);
						i = 0;
						break;
					}
				}
			}
		}
	}
}



void srtn_wait ()
{
	//atualizamos o numero de cores livres
	occupied -= threadlist_clear();
	usleep(1000*10); //esperamos um pouco antes de rodar outra iteração para não gastar CPU demais
}

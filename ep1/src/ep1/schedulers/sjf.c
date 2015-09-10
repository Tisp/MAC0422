#include "sjf.h"
#include "../threadlist.h"


//numero de cores ocupados
static int occupied = 0;



void sjf_update ()
{
	//atualizamos o numero de cores livres
	occupied -= threadlist_clear();

	//colocamos o menor thread para todar até encher todos os cores disponiveis ou não terem mais threads
	int size = threadlist_size();
	for(int i=0; i<size && occupied<threadlist_ncores(); i++)
	{
		int id = threadlist_getid(i);
		if(!threadlist_running(id)) //pegamos uma thread que não esta rodando e comparamos ela com todas as outras que tambem nao estão
		{
			bool run = true;

			for(int j=0; j<size; j++)
			{
				int id2 = threadlist_getid(j);
				if(!threadlist_running(id2))
				{
                    if(threadlist_dt(id) > threadlist_dt(id2))
						run = false;
				}
			}

			if(run) //a thread que verificamos é a mais curta
			{
				threadlist_marktorun(id);
				occupied++;
				i = 0; //temos que verificar todas as threads de novo
			}
		}

	}
}



void sjf_wait ()
{
}

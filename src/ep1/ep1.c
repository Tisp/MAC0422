#include <stdlib.h>
#include <stdio.h>
#include "threadlist.h"
#include "util.h"



int main (int argc, char** argv)
{
	//redirecionamento de entrada e saida, trocar os parametros de entrada
	/*char* argv2[] = {"./"};
	int argc2 = sizeof(argv2)/sizeof(*argv2);
	argc = argc2;
	argv = argv2;*/
	//freopen("./testdata/in", "r", stdin);
	//freopen("./testdata/out", "w", stdout);


	threadlist_init(THREADS);

	for(int i=0; i<(THREADS*THREAD_MESSAGES)/PARALLEL; i++)
	{
		printf("start run %d\n", i);

		for(int j=0; j<PARALLEL; j++)
			threadlist_marktorun((i+j)%THREADS);
		threadlist_run();

		//RAND_WAIT();

		threadlist_waitall();

		printf("end run %d\n\n", i);
	}

	threadlist_destroy();


	return EXIT_SUCCESS;
}

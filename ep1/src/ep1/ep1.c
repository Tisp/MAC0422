#include <stdlib.h>
#include <stdio.h>
#include "threadlist.h"
#include "scheduler.h"
#include "util.h"



int main (int argc, char** argv)
{
	//redirecionamento de entrada e saida, trocar os parametros de entrada
	/*char* argv2[] = {"./"};
	int argc2 = sizeof(argv2)/sizeof(*argv2);
	argc = argc2;
	argv = argv2;*/
	//freopen("./testdata/input0", "r", stdin);
	//freopen("./testdata/out", "w", stdout);


	/*int t0;
	char name[80];
	int dt;
	int deadline;
	int p;

	while(scanf("%d %s %d %d %d\n", &t0, name, &dt, &deadline, &p) == 5)
	{
		printf("%d	%s	%d	%d	%d\n", t0, name, dt, deadline, p);
	}*/


	threadlist_init();

	for(int i=0; !threadlist_empty(); i++)
	{
		printf("start run %d\n", i);

		scheduler_update();
		threadlist_signalrun();
		scheduler_wait();

		printf("end run %d\n\n", i);
	}

	threadlist_destroy();


	return EXIT_SUCCESS;
}

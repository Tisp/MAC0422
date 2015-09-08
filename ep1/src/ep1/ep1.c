#include <stdlib.h>
#include <stdio.h>
#include "threadlist.h"
#include "linkedlist.h"
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


	linkedlist_init();

	printf("adicionando\n");
	linkedlist_add('a');
	linkedlist_add('b');
	linkedlist_add('c');
	linkedlist_add('d');
	printf("\n");

	printf("size %d\n\n", linkedlist_size());

	printf("lendo\n");
	printf("0 %c\n", linkedlist_get(0));
	printf("0 %c\n", linkedlist_get(0));
	printf("1 %c\n", linkedlist_get(1));
	printf("2 %c\n", linkedlist_get(2));
	printf("3 %c\n", linkedlist_get(3));
	printf("\n");

	printf("deletando\n");
	linkedlist_delete(1);
	linkedlist_delete(0);
	linkedlist_delete(1);
	printf("\n");

	printf("size %d\n\n", linkedlist_size());

	printf("lendo\n");
	printf("0 %c\n", linkedlist_get(0));
	printf("\n");

	printf("adicionando\n");
	linkedlist_add('e');
	linkedlist_add('f');
	printf("\n");

	printf("size %d\n\n", linkedlist_size());

	printf("lendo\n");
	printf("0 %c\n", linkedlist_get(0));
	printf("1 %c\n", linkedlist_get(1));
	printf("2 %c\n", linkedlist_get(2));
	printf("\n");

	linkedlist_destroy();


	/*int t0;
	char name[80];
	int dt;
	int deadline;
	int p;

	while(scanf("%d %s %d %d %d\n", &t0, name, &dt, &deadline, &p) == 5)
	{
		printf("%d	%s	%d	%d	%d\n", t0, name, dt, deadline, p);
	}*/


	/*threadlist_init(THREADS);

	for(int i=0; !threadlist_empty(); i++)
	{
		printf("start run %d\n", i);

		scheduler_update();
		threadlist_signalrun();
		scheduler_wait();

		printf("end run %d\n\n", i);
	}

	threadlist_destroy();*/


	return EXIT_SUCCESS;
}

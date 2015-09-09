/*
	Este arquivo define a interface para uma lista de threads, assim como várias funções para manipulá-las. Isso inclui criação de threads, pausar ou continuar threads, etc.
	Todas as threads são identificadas por um ID único.
	As threads criadas aqui sempre executam uma função "dummy" que gasta tempo real de CPU, mas isso poderia ser modificado.
*/

#ifndef THREADLIST_H
#define THREADLIST_H


#include <unistd.h>
#include <pthread.h>
#include "util.h"


///Macro para esperar por um tempo aleatório, útil para forçar que apareçam bugs
#define RAND_WAIT() usleep(rand()%10000)


///Inicializar a estrutura
void threadlist_init ();

///Destroi a estrutura e as threads que estão contidas nela
void threadlist_destroy ();

///Cria uma nova thread e retorna seu ID
int threadlist_create ();

///Remove uma thread que já tenha terminado
void threadlist_remove (int id);

///Remove todas as threads que já terminaram
void threadlist_clear ();

///Marca uma thread para execução quando a função threalist_signalrun for chamadda
void threadlist_marktorun (int id);

///Sinaliza a uma thread que ela deve parar assim que possível
void threadlist_markstop (int id);

///Continua a execução de todas as threads marcadas com threadlist_marktorun
void threadlist_signalrun ();

///Espera que uma thread termine
void threadlist_wait (int id);

///Espera que uma thread qualquer termine
void threadlist_waitany ();

///Retorna false se não há nenhuma thread na estrutura e true caso contrário
bool threadlist_empty ();
#endif

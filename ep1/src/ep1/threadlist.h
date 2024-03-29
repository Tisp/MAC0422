/*
	Este arquivo define a interface para uma lista de threads, assim como várias funções para manipulá-las. Isso inclui criação de threads, pausar ou continuar threads, etc.
	Todas as threads são identificadas por um ID único.
	As threads criadas aqui sempre executam uma função "dummy" que gasta tempo real de CPU, mas isso poderia ser modificado.
*/

#ifndef THREADLIST_H
#define THREADLIST_H


#include "util.h"


//tamanho máximo do nome de processo
#define NAME_SIZE 100


///Clock global definido externamente. Deve ser iniciado antes de chamar qualquer função definida neste arquivo, exceto init
extern timer global_clock;

///Guarda o numero de mudancas de contexto que ocorreram
int context_changes;


///Inicializar a estrutura, é necessário informar o numero de cores, uma stream para imprimir a saida e uma flag indicando se é para imprimir menssagens debug.
void threadlist_init (int ncores, FILE* output, bool dbg);

///Destroi a estrutura e as threads que estão contidas nela
void threadlist_destroy ();

///Cria uma nova thread com os parametros fornecidos e retorna seu ID
int threadlist_create (char* name, int dt, int deadline);

///Remove uma thread que já tenha terminado
void threadlist_remove (int id);

///Remove todas as threads que já terminaram e retorna quantas foram removidas
int threadlist_clear ();

///Marca uma thread para execução quando a função threalist_signalrun for chamada. As threads devem estar travadas antes dessa função ser chamada.
void threadlist_marktorun (int id);

///Sinaliza a uma thread que ela deve parar assim que possível. As threads devem estar travadas antes dessa função ser chamada.
void threadlist_markstop (int id);

///Continua a execução de todas as threads marcadas com threadlist_marktorun
void threadlist_signalrun ();

///Espera que uma thread termine
void threadlist_wait (int id);

///Espera que uma thread qualquer termine
void threadlist_waitany ();

///Trava todas as threads
void threadlist_lockall ();

///Destrava todas as threas
void threadlist_unlockall ();

///Retorna o ID da index-ésima thread. É garantido que as threads estão ordenadas do menor para o maior ID
int threadlist_getid (int index);

///Returna true se a thread esta marcada para rodar e false caso contrario
bool threadlist_running (int id);

///Returna o tempo total que a thread deve executar em segundos
int threadlist_get_dt (int id);

///Returna o tempo que falta para a thread terminar em ms
int threadlist_get_remainingms (int id);

///Retorna o número de threads na estrutura
int threadlist_size ();

///Retorna false se não há nenhuma thread na estrutura e true caso contrário
bool threadlist_empty ();

///Retorna o numero de cores sendo simulados
int threadlist_ncores ();

///Retorna o deadline da thread
int threadlist_get_deadline (int id);
#endif

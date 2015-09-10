/*
	Funções úteis em geral, como se fosse uma stdlib.
*/

#ifndef UTIL_H
#define UTIL_H


#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#define NAN ((0.0)/(double)(0.0))


///Converte o argumento para string sem expandi-lo
#define STRINGIFY_NOEVAL(arg) #arg

///Converte o argumento para string
#define STRINGIFY(arg) STRINGIFY_NOEVAL(arg)

///Macro para chamar error com os parametros corretos
#define ERROR(...) error(__FILE__, __func__, __LINE__, __VA_ARGS__)

///Macro para chamar warn com os parametros corretos
#define WARN(...) warn(__FILE__, __func__, __LINE__, __VA_ARGS__)


///Estrutura usada para guardar tempo
typedef struct timer timer;


struct timer
{
	int start_time;
};


///@todo implementar isso, por enquanto usa a stdlib implicitamente mas isso nao existe no C99
char* strdup (const char*);
int usleep (unsigned int);

///Imprime o arquivo, linha, função e a mensagem de erro msg (mesmo formato do printf) e chama exit(EXIT_FAILURE)
void error (const char* const restrict file, const char* const restrict func, int line, const char* const msg, ...);

///Imprime o arquivo, linha, função e a mensagem de erro msg (mesmo formato do printf) e continua a execução
void warn (const char* const restrict file, const char* const restrict func, int line, const char* const msg, ...);

///Igual a malloc, mas caso não consiga alocar a memória a macro ERROR é chamada
void* fmalloc (const size_t tam);

///Igual a realloc, mas caso não consiga alocar a memória a macro ERROR é chamada
void* frealloc (void* p, const size_t tam);

///Igual a realloc, mas caso não consiga alocar a memória a macro ERROR é chamada
void* frealloc (void* p, const size_t tam);

///Inicializa um timer e o retorna, não precisa ser destruido pois não é alocada memória ao cria-lo
timer timer_start ();

///Retorna o número de segundos medido pelo timer desde que foi iniciado
int timer_gets (timer cl);

///Retorna o número de milisegundos medido pelo timer desde que foi iniciado
long int timer_getms (timer cl);
#endif

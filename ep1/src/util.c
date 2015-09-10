#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "util.h"



void error (const char* const restrict file, const char* const restrict func, int line, const char* const msg, ...)
{
	va_list args;
	va_start(args, msg);

	fprintf(stderr, "ERRO: %s:%s:%d: ", file, func, line);
	vfprintf(stderr, msg, args);
	fprintf(stderr, ".\n");
	exit(EXIT_FAILURE);

	va_end(args);
}



void warn (const char* const restrict file, const char* const restrict func, int line, const char* const msg, ...)
{
	va_list args;
	va_start(args, msg);

	fprintf(stderr, "ATENCAO: %s:%s:%d: ", file, func, line);
	vfprintf(stderr, msg, args);
	fprintf(stderr, ".\n");

	va_end(args);
}



void* fmalloc (const size_t tam)
{
	void* ret = malloc(tam);

	if(ret == NULL)
		ERROR("Sem memoria");

	return ret;
}



void* frealloc (void* p, const size_t tam)
{
	void* ret = realloc(p, tam);

	if(ret == NULL)
		ERROR("Sem memoria");

	return ret;
}



///@todo timer com milisegundos
timer timer_start ()
{
	timer ret;
	ret.start_time = time(NULL);
	return ret;
}



int timer_gets (timer cl)
{
	return time(NULL) - cl.start_time;
}



long int timer_getms (timer cl)
{
	return (time(NULL) - cl.start_time) * 1000;
}

/*
    Interface padrão para escalonadores. Deve implementar uma função que atualiza quais threads devem ser executadas e uma função que espera algum alguma condição.
    O funcionamento de ambas dessas funções depende de qual escalonador está sendo implementado.
*/

#ifndef ROUNDROBIN_H
#define ROUNDROBIN_H


void roundrobin_init ();

void roundrobin_destroy ();

void roundrobin_update ();

void roundrobin_wait ();
#endif

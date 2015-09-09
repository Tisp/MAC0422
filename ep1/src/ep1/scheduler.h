/*
    Interface para shedulers. Deve implementar uma função que atualiza quais threads devem ser executadas e uma função que espera algum alguma condição.
    O funcionamento de ambas dessas funções depende de qual escalonador está sendo implementado.
*/

#ifndef SCHEDULER_H
#define SCHEDULER_H


void scheduler_update ();

void scheduler_wait ();
#endif

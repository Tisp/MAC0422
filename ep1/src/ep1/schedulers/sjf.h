/*
    Interface padrão para escalonadores. Deve implementar uma função que atualiza quais threads devem ser executadas e uma função que espera algum alguma condição.
    O funcionamento de ambas dessas funções depende de qual escalonador está sendo implementado.
*/

#ifndef SJF_H
#define SJF_H


void sjf_update ();

void sjf_wait ();
#endif

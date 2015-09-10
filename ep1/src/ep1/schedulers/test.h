/*
    Interface padrão para escalonadores. Deve implementar uma função que atualiza quais threads devem ser executadas e uma função que espera algum alguma condição.
    O funcionamento de ambas dessas funções depende de qual escalonador está sendo implementado.
*/

#ifndef TEST_H
#define TEST_H


void test_update ();

void test_wait ();
#endif

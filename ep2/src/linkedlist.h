/*
	Tipo simples de lista ligada.
*/

#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include "util.h"


///Definição do tipo de uma instancia lista ligada
typedef struct node* linkedlist;

///Definicado do tipo de dados que é guardado pela lista ligada
typedef void* linkedlist_data;


///Cria e retorna uma nova instancia de lista ligada
linkedlist linkedlist_new ();

///Destroi uma instancia de lista ligada
void linkedlist_destroy (linkedlist list);

///Adiciona um elemento a uma lista
void linkedlist_add (linkedlist list, linkedlist_data e);

///Remove um elemento de uma lista
void linkedlist_delete (linkedlist list, int index);

///Retorna um elemento de uma lista
linkedlist_data linkedlist_get (linkedlist list, int index);

///Retorna o numero de elementos de uma lista ligada
int linkedlist_size (linkedlist list);

///Retorna true se a lista está vazia e false caso contrário
bool linkedlist_empty (linkedlist list);
#endif

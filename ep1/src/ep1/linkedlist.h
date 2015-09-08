#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include "threadlist.h"


typedef thread* linkedlist_data;


void linkedlist_init ();

void linkedlist_destroy ();

void linkedlist_add (linkedlist_data e);

void linkedlist_delete (int index);

linkedlist_data linkedlist_get (int index);

int linkedlist_size ();
#endif

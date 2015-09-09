/*
	Implementacao simples de lista ligada com cabeça.
*/

#include "linkedlist.h"
#include "util.h"


typedef struct node node;


struct node
{
    linkedlist_data data;
    node* next;
};



linkedlist linkedlist_new ()
{
	linkedlist list = fmalloc(sizeof(node));
	list->next = NULL;
	return list;
}



void linkedlist_destroy (linkedlist list)
{
	node* n = list;
	node* next = list->next;

	while(n != NULL)
	{
		next = n->next;
		free(n);
		n = next;
	}
}



void linkedlist_add (linkedlist list, linkedlist_data e)
{
	node* n = fmalloc(sizeof(node));

	n->data = e;
	n->next = NULL;

	node* last = list;
	while(last->next != NULL)
		last = last->next;

	last->next = n;
}



void linkedlist_delete (linkedlist list, int index)
{
	node* n = list->next;
	node* prev = list;

	for(int i=0; i<index; i++)
	{
		prev = n;
		n = n->next;
	}

	prev->next = n->next;
	free(n);
}



linkedlist_data linkedlist_get (linkedlist list, int index)
{
	node* n = list->next;

	for(int i=0; i<index; i++)
		n = n->next;

	return n->data;
}



int linkedlist_size (linkedlist list)
{
	node* n = list;

	int i;
	for(i=0; n->next!=NULL; i++)
		n = n->next;

	return i;
}

#include "linkedlist.h"
#include "util.h"


typedef struct node node;


struct node
{
    linkedlist_data data;
    node* next;
};


static node* start;



void linkedlist_init ()
{
	start = fmalloc(sizeof(node));
	start->next = NULL;
}



void linkedlist_destroy ()
{
	node* n = start;
	node* next = start->next;

	while(n != NULL)
	{
		next = n->next;
		free(n);
		n = next;
	}
}



void linkedlist_add (linkedlist_data e)
{
	node* n = fmalloc(sizeof(node));

	n->data = e;
	n->next = NULL;

	node* last = start;
	while(last->next != NULL)
		last = last->next;

	last->next = n;
}



void linkedlist_delete (int index)
{
	node* n = start->next;
	node* prev = start;

	for(int i=0; i<index; i++)
	{
		prev = n;
		n = n->next;
	}

	prev->next = n->next;
	free(n);
}



linkedlist_data linkedlist_get (int index)
{
	node* n = start->next;

	for(int i=0; i<index; i++)
		n = n->next;

	return n->data;
}



int linkedlist_size ()
{
	node* n = start;

	int i;
	for(i=0; n->next!=NULL; i++)
		n = n->next;

	return i;
}

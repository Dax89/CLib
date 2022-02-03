#include "list.h"

void list_delete(list* l)
{
    list_node* e = l->head;

    while(e)
    {
        list_node* next = e->next;
        if(l->free) l->free(*l, e);
        l->allocator->free(containerbase(e, void, l->offsethint));
        e = next;
    }

    l->size = 0;
    l->head = l->tail = NULL;
}

void _list_prepend(list* l, list_node* n)
{
    l->size++;

    if(l->head)
    {
        l->head->prev = n;
        n->next = l->head;
        l->head = n;
    }
    else
    {
        n->next = n->prev = NULL;
        l->head = n;
    }
}

void _list_append(list* l, list_node* n)
{
    l->size++;

    if(l->head)
    {
        n->prev = l->tail;
        n->next = NULL;

        l->tail->next = n;
        l->tail = n;
    }
    else
    {
        n->next = n->prev = NULL;
        l->head = l->tail = n;
        l->tail = n;
    }
}

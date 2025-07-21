#ifndef DLinkedL_H
#define DLinkedL_H

#include <stddef.h> 

typedef struct Node
{
    void * data;
    struct Node * left;
    struct Node * right;
} Node;

typedef struct DoublyLinkedList {
    Node *head;
    Node *tail;
    size_t size;
    void (*printData)(void *);
    int (*cmp)(void *, void *);
} DoublyLinkedList;


DoublyLinkedList * dll_init();

int dll_insert_head(DoublyLinkedList * list, void *data);

int dll_insert_tail(DoublyLinkedList * list, void *data);

void printDLL(DoublyLinkedList *list);

int dll_remove_head(DoublyLinkedList * list);

int dll_remove_tail(DoublyLinkedList * list);

int dll_remove_at(DoublyLinkedList * list, int index);

Node* dll_get(DoublyLinkedList * list, int index);

int dll_find(DoublyLinkedList * list, void *data);

// void dll_clear(DoublyLinkedList * list);


#endif







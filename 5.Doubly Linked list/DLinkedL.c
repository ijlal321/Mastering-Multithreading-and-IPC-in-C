#include <stdio.h>
#include <stdlib.h>
#include "DLinkedL.h"

/* static helper fns*/

static Node *create_Node();
static int isIndexValid(DoublyLinkedList *list, int index);
static void freeNode(Node *node);

static Node *create_Node()
{
    Node *newNode = calloc(1, sizeof(Node));
    return newNode;
}

static int isIndexValid(DoublyLinkedList *list, int index)
{
    if (list->size == 0 || index < 0 || (index + 1) > list->size)
    {
        return 0;
    }
    return 1;
}

static void freeNode(Node *node)
{
    free(node->data);
    free(node);
}

/*Implementations of .h fns*/

DoublyLinkedList *dll_init()
{
    DoublyLinkedList *newList = calloc(1, sizeof(DoublyLinkedList));
    return newList;
}

int dll_insert_head(DoublyLinkedList *list, void *data)
{
    Node *newNode = create_Node();
    newNode->data = data;

    if (list->size == 0)
    {
        list->head = newNode;
        list->tail = newNode;
        list->size = 1;
        return 1;
    }

    newNode->right = list->head;
    list->head->left = newNode;

    list->head = newNode;
    list->size++;
    return 1;
}

int dll_insert_tail(DoublyLinkedList *list, void *data)
{
    Node *newNode = create_Node();
    newNode->data = data;

    if (list->size == 0)
    {
        list->head = newNode;
        list->tail = newNode;
        list->size = 1;
        return 1;
    }

    newNode->left = list->tail;
    list->tail->right = newNode;

    list->tail = newNode;
    list->size++;
    return 1;
}

int dll_remove_head(DoublyLinkedList *list)
{
    if (list->size == 0)
    {
        return 0;
    }
    if (list->size == 1)
    {
        freeNode(list->head);
        list->size = 0;
        list->head = NULL;
        list->tail = NULL;
        return 1;
    }

    Node *nxt = list->head->right;
    nxt->left = NULL;
    freeNode(list->head);
    list->head = nxt;
    list->size--;
    return 1;
}

int dll_remove_tail(DoublyLinkedList *list)
{
    if (list->size == 0)
    {
        return 0;
    }
    if (list->size == 1)
    {
        freeNode(list->tail);
        list->size = 0;
        list->head = NULL;
        list->tail = NULL;
        return 1;
    }

    Node *prev = list->tail->left;
    prev->right = NULL;
    freeNode(list->tail);
    list->tail = prev;
    list->size--;
    return 1;
}

void printDLL(DoublyLinkedList *list)
{
    if (list->printData == NULL)
    {
        printf("cannot print List because print data function not defined");
        return;
    }

    Node *cur = list->head;

    printf("Starting printing List. Cur Size: %lu \n", list->size);

    while (cur != NULL)
    {
        list->printData(cur->data);
        if (cur->right != NULL)
        {
            printf("->");
        }
        cur = cur->right;
    }
    printf("\n");
}

int dll_remove_at(DoublyLinkedList *list, int index)
{
    if (isIndexValid(list, index) == 0)
        return 0;

    Node *cur = list->head;
    for (int i = 0; i < index; i++)
    {
        cur = cur->right;
    }

    Node *prev = cur->left;
    Node *nxt = cur->right;

    if (prev)
    {
        prev->right = nxt;
    }
    else
    { // meaning its head itself
        list->head = nxt;
    }

    if (nxt)
    {
        nxt->left = prev;
    }
    else
    { // meaning its head itself
        list->tail = prev;
    }
    freeNode(cur);
    list->size--;
    return 1;
}

Node *dll_get(DoublyLinkedList *list, int index)
{
    if (isIndexValid(list, index) == 0)
        return NULL;

    Node *cur = list->head;
    for (int i = 0; i < index; i++)
    {
        cur = cur->right;
    }
    return cur;
}

int dll_find(DoublyLinkedList *list, void *data)
{
    Node *cur = list->head;
    int index = 0;
    for (; index < list->size; index++)
    {
        if (list->cmp(cur->data, data) == 0)
        {
            return index;
        }
        cur = cur->right;
    }

    return -1;
}

void dll_clear(DoublyLinkedList *list)
{
    Node *cur = list->head;

    while (cur)
    {
        Node *nxt = cur->right;
        freeNode(cur);
        cur = nxt;
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
}

// example
/*Using the lib*/
void printData(void *data)
{
    printf("%d", *(int *)data);
}

int compareData(void *data1, void *data2)
{
    int val1 = *(int *)data1;
    int val2 = *(int *)data2;

    if (val1 == val2)
    {
        return 0;
    }
    else if (val1 > val2)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

int main()
{
    DoublyLinkedList *list = dll_init();
    list->printData = printData;
    list->cmp = compareData;

    int *data = malloc(sizeof(int));
    *data = 10;
    dll_insert_head(list, data);

    int *data2 = malloc(sizeof(int));
    *data2 = 20;
    dll_insert_head(list, data2);

    int *data3 = malloc(sizeof(int));
    *data3 = 30;
    dll_insert_head(list, data3);

    int *data4 = malloc(sizeof(int));
    *data4 = 40;
    dll_insert_head(list, data4);

    printDLL(list);
    // dll_remove_head(list);
    // printDLL(list);
    // dll_remove_tail(list);

    // dll_remove_at(list, 3);
    // dll_remove_at(list, 2);

    // Node *getNode = dll_get(list, 3);
    // if (getNode != NULL)
    // {
    //     printData(getNode->data);
    //     printf("\n");
    // }

    int find_data = 30;
    int res = dll_find(list, (void *)&find_data);
    if (res != -1)
    {
        printf("founded data, at index: %d \n", res);
    }

    printDLL(list);

    dll_clear(list);

    printDLL(list);
}
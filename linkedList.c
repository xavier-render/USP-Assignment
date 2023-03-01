/**
 * @file linkedlist.c
 * @author Xavier Render
 * @date 2022-05-29
 * 
 * REFERENCES: 
 * Write your own memcpy() and memmove() - https://www.geeksforgeeks.org/write-memcpy/ 
 * USP Lecture 05 - C system calls 
 * 
 */

#include "linkedList.h"

// own implementation of memcpy() which is used to copy a memory block from one location to another
void *mymemcpy(void *dest, void *src, size_t n) {
    for(int i = 0; i < n; i++) {
        ((char*)dest)[i] = ((char*)src)[i];
    }
}

// function use to iterating over the linked list 
void traverse(linked_list_t *ll) {
    node_ptr_t t = ll->head;
    while(t != NULL) {
        printf("%s\n", (char*)t->data);
        t = t->next;
    }
}

// Allocates memory for a node of the linked list
// and appends it to the tail of the linked list.
void insert(size_t data_size, void *data, linked_list_t *ll)
{
    // Empty linked list.
    if(ll->head == NULL)
    {
        ll->head = sbrk(sizeof(node_t));
        ll->head->data = sbrk(data_size);
        mymemcpy(ll->head->data, data, data_size);
        ll->tail = ll->head;
        ll->head->next = NULL;
    }
    else
    {
        // Linked list non empty.
        void *t = sbrk(sizeof(node_t));
        ll->tail->next = t;
        ((node_ptr_t)t)->data = sbrk(data_size);
        mymemcpy(((node_ptr_t)t)->data, data, data_size);
        ll->tail = t;
        ll->tail->next = NULL;
    }
}
/**
 * @file linkedlist.h
 * @author Xavier Render
 * @date 2022-05-29
 * 
 * @brief A header file for linkedList.c
 * 
 */

#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include "linkedList.h"

// Pointer to the node.
typedef struct node* node_ptr_t;
// Node type.
typedef struct node node_t;

struct node {
    // generic linked list node data.
    void *data;
    // next pointer.
    node_ptr_t next;
};

typedef struct linked_list linked_list_t;

// singly linked list.
struct linked_list {
    node_ptr_t head;
    node_ptr_t tail;
};

void traverse(linked_list_t *ll);
void insert(size_t data_size, void *data, linked_list_t *ll);
#endif
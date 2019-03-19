/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#ifndef QUEUE_H_
#define QUEUE_H_

#include "utility.h"

typedef struct queue
{
    process_t proc;
    struct queue *next;
} node_t;

extern void push(node_t **head, process_t proc);
extern process_t pop(node_t **head);
extern void print_list(node_t *head);

#endif /* QUEUE_H_ */
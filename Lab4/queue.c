/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void push(node_t **head, process_t proc)
{
    // Create a new node
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->proc = proc;
    new_node->next = NULL;

    // create pointer to current node
    node_t *temp = (*head);

    if (temp == NULL)
    {
        *head = new_node;
        return;
    }

    // Find the end of the list
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = new_node;
}

process_t pop(node_t **head)
{
    node_t *temp = (*head)->next;
    process_t returned = (*head)->proc;
    free(*head);
    *head = temp;
    return returned;
}

void print_list(node_t *head)
{
    node_t *current = head;

    while (current != NULL)
    {
        printf("%d %d %d %d %d %d %d %d\n", current->proc.arr_time, current->proc.priority, current->proc.proc_time, current->proc.mem_size, current->proc.printers, current->proc.scanners, current->proc.modems, current->proc.cds);
        current = current->next;
    }
}

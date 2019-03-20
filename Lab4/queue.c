/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018, Karan Jariwala (100619029), Harsh Patel (100580778), Aakash Patel (100616630), Karan Patel (100621178)
 * All rights reserved.
 * 
 */

#include "utility.h"
#include <stdlib.h>

/*  
    The code mostly written with the help of:
    http://www.learn-c.org/en/Linked_lists
*/
void push(node_t **head, proc_t process)
{
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->process = process;
    new_node->next = NULL;

    node_t *temp = (*head);

    if (temp == NULL)
    {
        *head = new_node;
        return;
    }

    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = new_node;
}

proc_t pop(node_t **head)
{
    node_t *temp = (*head)->next;

    proc_t returned = (*head)->process;
    free(*head);
    *head = temp;

    return returned;
}
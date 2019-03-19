/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "queue.h"

// Define your utility functions here, you will likely need to add more...

int mem_avail(resources_t *resources, int size, bool realtime)
{
    int loc = -1;
    int temp_loc = -1;
    int temp_size = 0;

    // Determine where to find available memory,
    // as per the rules 64 allocated for real time proc
    int start = 0;
    int limit = 1024 - 64;
    if (realtime)
    {
        start += limit;
        limit += 64;
    }

    // Try to find the first block of memory that can fit the requested memory size
    for (int i = start; i < limit; i++)
    {
        if (temp_size == size)
        {
            loc = temp_loc;
            break;
        }

        if (resources->mem[i] == 0)
        {
            if (temp_size == 0)
            {
                temp_loc = i;
            }
            temp_size++;
        }
        else
        {
            temp_size = 0;
        }
    }
    // Check if we have enough space to return a block of memory
    if (temp_size == size)
    {
        loc = temp_loc;
    }

    return loc;
}
void dispatch_proc(dispatcher_t *dispatcher)
{
    FILE *fpt = fopen("dispatchlist", "r");
    int jobs = 0;
    while (!feof(fpt))
    {
        process_t proc = {0};
        fscanf(fpt, "%d, %d, %d, %d, %d, %d, %d, %d", &proc.arr_time, &proc.priority, &proc.proc_time, &proc.mem_size,
               &proc.printers, &proc.scanners, &proc.modems, &proc.cds);

        if (proc.priority - 1 > 3)
        {
            return false;
        }

        if (proc.priority == 0)
        {
            push(&dispatcher->queue_real_time, proc);
        }
        else
        {
            push(&dispatcher->queues[proc.priority - 1], proc);
        }

        jobs++;
    }
}
// free_mem(resources res, int index, int size)
// {
//      ...
// }

// void load_dispatch(char *dispatch_file, node_t *queue)
// {
//      ...
// }

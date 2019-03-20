

#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int mem_avail(resources_t *resources, int size, bool realtime)
{
    int loc = -1;
    int temp_loc = -1;
    int temp_size = 0;

    // Determine where to find available memory
    int start = 0;
    int limit = MEMORY_SIZE - REALTIME_MEMORY_SIZE;
    if (realtime)
    {
        start += limit;
        limit += REALTIME_MEMORY_SIZE;
    }

    // Try to find the first block of memory that can fit the requested memory size
    for (int i = start; i < limit; i++)
    {
        if (temp_size == size)
        {
            loc = temp_loc;
            break;
        }

        if (resources->memory[i] == 0)
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

/*
    Didn't wanna play with pointers so using a built in functions.
    Credit: https://www.tutorialspoint.com/c_standard_library/c_function_memset.htm
*/
void mem_alloc(resources_t *resources, int loc, int size)
{
    memset(resources->memory + loc, 1, size);
}

void mem_free(resources_t *resources, int loc, int size)
{
    memset(resources->memory + loc, 0, size);
}

bool device_avail(pid_t *devices, int size, int count)
{
    int c = 0;
    for (int i = 0; i < size; i++)
    {
        if (devices[i] == 0)
        {
            c++;
        }
    }
    return c >= count;
}

void device_alloc(pid_t *devices, int size, int count, pid_t pid)
{
    int c = 0;
    for (int i = 0; i < size; i++)
    {
        if (devices[i] == 0)
        {
            devices[i] = pid;
            c++;
        }
    }
}

void device_free(pid_t *devices, int size, pid_t pid)
{
    for (int i = 0; i < size; i++)
    {
        if (devices[i] == pid)
        {
            devices[i] = 0;
        }
    }
}

bool load_dispatcher(dispatcher_t *dispatcher)
{
    FILE *fpt = fopen("dispatchlist", "r");

    while (!feof(fpt))
    {
        proc_t proc = {0};
        fscanf(fpt, "%d, %d, %d, %d, %d, %d, %d, %d", &proc.arrival_time, &proc.priority, &proc.processor_time, &proc.memory,
               &proc.printers, &proc.scanners, &proc.modems, &proc.cds);

        if (proc.priority == 0)
        {
            push(&dispatcher->queue_real_time, proc);
        }
        else
        {
            push(&dispatcher->queues[proc.priority - 1], proc);
        }
    }

    fclose(fpt);

    return true;
}

/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
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
    int limit = 1024 - 64; // only use 960 MB
    if (realtime)
    {
        start += limit;
        limit += 64;
    }

    // Try to find the first block of memory that can fit the requested memory size
    for (int i = start; i < limit; i++)
    {
        if (temp_size == size) // If we have no memory left.
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
    while (!feof(fpt) && jobs < 100)
    {
        process_t proc = {0};
        fscanf(fpt, "%d, %d, %d, %d, %d, %d, %d, %d", &proc.arr_time, &proc.priority, &proc.proc_time, &proc.mem_size,
               &proc.printers, &proc.scanners, &proc.modems, &proc.cds);

        if (proc.priority - 1 > 3)
        {
            exit(0);
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
void mem_alloc(resources_t *resources, int loc, int size)
{
    // Set memory as used
    memset(resources->mem + loc, 1, size);
}

void mem_free(resources_t *resources, int loc, int size)
{
    // Set memory as unused
    memset(resources->mem + loc, 0, size);
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

bool dispatcher_queue_execute(dispatcher_t *dispatcher, resources_t *resources, int queue_index)
{
    node_t **head;
    if (queue_index == -1)
    {
        // Real time queue
        head = &dispatcher->queue_real_time;
    }
    else
    {
        // Priority queue
        head = &dispatcher->queues[queue_index];
    }

    if (*head != NULL)
    {
        // Get a process
        process_t p = pop(head);

        // Check if the process is already running and is suspended
        if (p.running_status && p.suspend_status)
        {
            printf("Process %d was suspended, continuing execution\r\n", p.pid);

            // Signal continue
            kill(p.pid, SIGCONT);
            waitpid(p.pid, NULL, WCONTINUED);
        }

        // Attempt to create process
        pid_t pid = p.pid;
        if (pid == 0 && !p.suspend_status)
        {
            // Check if we have enough memory to execute this process
            int block;
            if ((block = mem_avail(resources, p.mem_size, p.priority == PRIORITY_REALTIME)) == -1)
            {
                // Failed, notify and push back to head
                printf("Not enough memory available to execute process, skipping...\r\n");
                if (queue_index != 2)
                {
                    push(&dispatcher->queues[queue_index + 1], p);
                }
                else
                {
                    push(&dispatcher->queues[queue_index], p);
                }
                return false;
            }

            // Check if we have the devices to execute the process
            if (p.priority != PRIORITY_REALTIME)
            {
                // Check if we have enough printers available
                if (!device_avail(resources->printers, PRINTER_COUNT, p.printers))
                {
                    // Failed, notify and push back to head
                    printf("Not enough printers available to execute process, skipping...\r\n");
                    push(head, p);

                    return false;
                }

                // Check if we have enough scanners available
                if (!device_avail(resources->scanners, SCANNER_COUNT, p.scanners))
                {
                    // Failed, notify and push back to head
                    printf("Not enough scanners available to execute process, skipping...\r\n");
                    push(head, p);

                    return false;
                }

                // Check if we have enough modems available
                if (!device_avail(resources->modems, MODEM_COUNT, p.modems))
                {
                    // Failed, notify and push back to head
                    printf("Not enough modems available to execute process, skipping...\r\n");
                    push(head, p);

                    return false;
                }

                // Check if we have enough CDs available
                if (!device_avail(resources->cds, CD_COUNT, p.cds))
                {
                    // Failed, notify and push back to head
                    printf("Not enough CD Drives available to execute process, skipping...\r\n");
                    push(head, p);

                    return false;
                }

                // Allocate devices
                device_alloc(resources->printers, PRINTER_COUNT, p.printers, p.pid);
                device_alloc(resources->scanners, SCANNER_COUNT, p.scanners, p.pid);
                device_alloc(resources->modems, MODEM_COUNT, p.modems, p.pid);
                device_alloc(resources->cds, CD_COUNT, p.cds, p.pid);
            }

            // Allocate memory
            mem_alloc(resources, block, p.mem_size);
            p.mem_addr = block;

            // Flush all output
            fflush(stdout);

            // Fork a process
            pid = fork();

            // Set state
            p.running_status = true;
            p.suspend_status = false;
        }

        // Execute
        if (pid == 0)
        {
            // Execute program, will be started in a suspended state
            int return_code = execl("process", "");

            // Exit process with the same exit code as the program executed
            exit(return_code);
        }
        else
        {
            // Store process id
            p.pid = pid;

            // Suspend child process
            kill(p.pid, SIGSTOP);

            // Print process information
            printf("\tProcess ID: %d", p.pid);
            printf("\tPriority: %d", p.priority);
            printf("\tProcessor Time: %ds", p.proc_time);
            printf("\tMemory: %dMB", p.mem_size);
            printf("\tMemory Address: %d\n", p.mem_addr);
            //printf("\tDevices:");
            printf("\tPrinters: %d", p.printers);
            printf("\tScanners: %d", p.scanners);
            printf("\tModems: %d", p.modems);
            printf("\tCD Drives: %d\n", p.cds);

            // Continue executing after we've printed our lines
            kill(p.pid, SIGCONT);

            // Check process priority, if realtime, then run and wait for exit
            // otherwise run, tick, then move to low priority ONLY if there is another process
            // waiting in a queue
            if (p.priority == PRIORITY_REALTIME || (p.suspend_status && p.proc_time == 1))
            {
                // Wait for process to finish its execution
                sleep(p.proc_time);
                kill(pid, SIGINT);
                waitpid(pid, NULL, 0);

                // Free up memory used by the process
                mem_free(resources, p.mem_addr, p.mem_size);
                p.mem_addr = 0;

                // Free up hardware used by the process
                device_free(resources->printers, PRINTER_COUNT, pid);
                device_free(resources->scanners, SCANNER_COUNT, pid);
                device_free(resources->modems, MODEM_COUNT, pid);
                device_free(resources->cds, CD_COUNT, pid);
            }
            else
            {
                // Active process, run for a tick length (N seconds)
                sleep(PRIORITY_PROCESSOR_TIME);
                kill(pid, SIGSTOP);

                // Lower processor time
                p.proc_time -= PRIORITY_PROCESSOR_TIME;

                // Set as suspended
                p.suspend_status = true;

                // Requeue depending on if a process is waiting to be executed (Pre-emption)
                if (queue_index != 2)
                {
                    push(&dispatcher->queues[queue_index + 1], p);
                }
                else
                {
                    push(&dispatcher->queues[queue_index], p);
                }
            }
        }
    }
    return true;
}

bool dispatcher_tick(dispatcher_t *dispatcher, resources_t *resources)
{
    // As long as there is a runtime process, give it tick priority
    // otherwise, check which queue has processes to be executed from 1-3 (3 being lowest)
    if (dispatcher->queue_real_time != NULL)
    {
        printf("REAL TIME QUEUE\n---------------\n");
        dispatcher_queue_execute(dispatcher, resources, -1);
        return true;
    }

    for (int i = 0; i < PRIORITY_QUEUE_COUNT; i++)
    {
        if (dispatcher->queues[i] != NULL)
        {
            printf("PRIORITY QUEUE: %d\n---------------\n", i + 1);
            dispatcher_queue_execute(dispatcher, resources, i);
            return true;
        }
    }

    return false;
}
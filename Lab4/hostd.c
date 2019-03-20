/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018, Karan Jariwala (100619029), Harsh Patel (100580778), Aakash Patel (100616630), Karan Patel (100621178)
 * All rights reserved.
 * 
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utility.h"

bool run_dispatcher(dispatcher_t *dispatcher, resources_t *resources, int queue_index);

int main()
{
    dispatcher_t dispatcher = {0};
    resources_t resources = {0};

    load_dispatcher(&dispatcher);

    while (dispatcher.queue_real_time != NULL)
    {
        printf("\nREAL TIME QUEUE \n---------------\n");
        run_dispatcher(&dispatcher, &resources, -1);
    }

    for (int i = 0; i < PRIORITY_QUEUE_COUNT; i++)
    {
        while (dispatcher.queues[i] != NULL)
        {
            printf("\nQUEUE: %d\n---------------\n", i + 1);
            run_dispatcher(&dispatcher, &resources, i);
        }
    }

    printf("\nDISPATCHER DONE EXITING ...\n");
    return EXIT_SUCCESS;
}

bool run_dispatcher(dispatcher_t *dispatcher, resources_t *resources, int queue_index)
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
        proc_t p = pop(head);

        // Check if the process is already running and is suspended
        if (p.running && p.suspended)
        {
            printf("Process %d was suspended\r\n", p.pid);

            // Signal continue
            kill(p.pid, SIGCONT);
            waitpid(p.pid, NULL, WCONTINUED);
        }

        pid_t pid = p.pid;
        if (pid == 0 && !p.suspended)
        {
            int block = mem_avail(resources, p.memory, p.priority == PRIORITY_REALTIME);
            if (block == -1)
            {
                printf("Insufficient Memory, Process Preempted...\r\n");
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

            if (p.priority != PRIORITY_REALTIME)
            {
                if (!device_avail(resources->printers, PRINTER_COUNT, p.printers))
                {
                    printf("Insufficient Printers...\r\n");
                    push(head, p);

                    return false;
                }

                if (!device_avail(resources->scanners, SCANNER_COUNT, p.scanners))
                {
                    printf("Insufficient Scanners...\r\n");
                    push(head, p);

                    return false;
                }

                if (!device_avail(resources->modems, MODEM_COUNT, p.modems))
                {
                    printf("Insufficient Modems...\r\n");
                    push(head, p);

                    return false;
                }

                if (!device_avail(resources->cds, CD_COUNT, p.cds))
                {
                    printf("Insufficient CDs...\r\n");
                    push(head, p);

                    return false;
                }

                device_alloc(resources->printers, PRINTER_COUNT, p.printers, p.pid);
                device_alloc(resources->scanners, SCANNER_COUNT, p.scanners, p.pid);
                device_alloc(resources->modems, MODEM_COUNT, p.modems, p.pid);
                device_alloc(resources->cds, CD_COUNT, p.cds, p.pid);
            }

            mem_alloc(resources, block, p.memory);
            p.address = block;

            fflush(stdout);

            pid = fork();

            p.running = true;
            p.suspended = false;
        }

        if (pid == 0)
        {
            int return_code = execl("process", "");

            exit(return_code);
        }
        else
        {
            p.pid = pid;
            kill(p.pid, SIGSTOP);

            printf("Executing Process ID: %d\n", p.pid);
            printf("Priority: %d\n", p.priority);
            printf("Processor Time: %ds\n", p.processor_time);
            printf("Memory: %dMB\n", p.memory);
            printf("Process Address: %d \n", p.address);
            printf("Printers: %d\n", p.printers);
            printf("Scanners: %d\n", p.scanners);
            printf("Modems: %d\n", p.modems);
            printf("CD Drives: %d\n\n", p.cds);

            kill(p.pid, SIGCONT);

            if (p.priority == PRIORITY_REALTIME || (p.suspended && p.processor_time == 1))
            {
                sleep(p.processor_time);
                kill(pid, SIGINT);
                waitpid(pid, NULL, 0);

                mem_free(resources, p.address, p.memory);
                p.address = 0;

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
                p.processor_time -= PRIORITY_PROCESSOR_TIME;

                // Set as suspended
                p.suspended = true;

                // Requeue depending on if a process is waiting to be executed
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
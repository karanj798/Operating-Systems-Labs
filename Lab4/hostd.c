/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
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

// Put macros or constants here using #define
#define MEMORY 1024

// Put global environment variables here

// Define functions declared in hostd.h here
int main()
{
    // ==================== YOUR CODE HERE ==================== //

    // Load the dispatchlist
    dispatcher_t dispatcher = {0};
    resources_t resources = {0};

    // Add each process structure instance to the job dispatch list queue
    // Allocate the resources for each process before it's executed
    dispatch_proc(&dispatcher);

    // Execute the process binary using fork and exec

    // Perform the appropriate signal handling / resource allocation and de-alloaction

    // Repeat until all processes have been executed, all queues are empty
    while (1)
    {
        if (!dispatcher_tick(&dispatcher, &resources))
            break;
    }
    printf("DISPATCHER EXIT\n");
    return EXIT_SUCCESS;
}
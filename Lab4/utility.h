/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 * 
 */
#include <sys/types.h>
#include <stdbool.h>

#ifndef UTILITY_H_
#define UTILITY_H_

// The amount of available memory
#define MEMORY 1024

// Resources structure containing integers for each resource constraint and an
// array of 1024 for the memory
// typedef struct {
//  ...
//  ...
// } resources;

typedef struct
{
    int printers;
    int scanners;
    int modems;
    int cds;
    int mem[MEMORY];
} resources_t;
// Processes structure containing all of the process details parsed from the
// input file, should also include the memory address (an index) which indicates
// where in the resources memory array its memory was allocated
// typedef struct {
//  ...
//  ...
// } process;

typedef struct
{
    // Parsed data
    int arr_time;
    int priority;
    int proc_time;
    int mem_size;
    int printers;
    int scanners;
    int modems;
    int cds;

    // Conventional tinggs
    pid_t pid;
    int mem_addr;
    int suspend_status;
    int running_status;

} process_t;

typedef struct
{
    struct node_t *queue_real_time;
    struct node_t *queues[3];
} dispatcher_t;

extern int mem_avail(resources_t *resources, int size, bool realtime);
extern void mem_alloc(resources_t *resources, int loc, int size);
extern void mem_free(resources_t *resources, int loc, int size);

extern bool device_avail(pid_t *devices, int size, int count);
extern void device_alloc(pid_t *devices, int size, int count, pid_t pid);
extern void device_free(pid_t *devices, int size, pid_t pid);

extern void dispatch_proc(dispatcher_t *dispatcher);

#endif /* UTILITY_H_ */
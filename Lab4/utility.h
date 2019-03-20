/*
 * Host Dispatcher Shell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018, Karan Jariwala (100619029), Harsh Patel (100580778), Aakash Patel (100616630), Karan Patel (100621178)
 * All rights reserved.
 * 
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdbool.h>
#include <sys/types.h>

#define BUFFER_SIZE 256

#define MEMORY_SIZE 1024
#define REALTIME_MEMORY_SIZE 64

#define PRINTER_COUNT 2
#define SCANNER_COUNT 1
#define MODEM_COUNT 1
#define CD_COUNT 2

#define PRIORITY_QUEUE_COUNT 3
#define PRIORITY_REALTIME 0
#define PRIORITY_PROCESSOR_TIME 1

#define DISPATCHER_JOB_LIMIT 100

// Forward declarations
struct node_t;

typedef struct
{
    char memory[MEMORY_SIZE];
    pid_t printers[PRINTER_COUNT];
    pid_t scanners[SCANNER_COUNT];
    pid_t modems[MODEM_COUNT];
    pid_t cds[CD_COUNT];
} resources_t;

typedef struct
{
    // Process information
    char name[BUFFER_SIZE];
    int arrival_time;
    int priority;
    int processor_time;
    int memory;
    int printers;
    int scanners;
    int modems;
    int cds;

    // Resources used by the processes
    int address;

    // State information
    pid_t pid;
    bool running;
    bool suspended;
} proc_t;

typedef struct
{
    struct node_t *queue_real_time;
    struct node_t *queues[PRIORITY_QUEUE_COUNT];
} dispatcher_t;

typedef struct queue_node_t
{
    proc_t process;
    struct queue_node_t *next;
} node_t;

// queue functions
extern void push(node_t **head, proc_t process);
extern proc_t pop(node_t **head);

// mem/dev functions
extern int mem_avail(resources_t *resources, int size, bool realtime);
extern void mem_alloc(resources_t *resources, int loc, int size);
extern void mem_free(resources_t *resources, int loc, int size);

extern bool device_avail(pid_t *devices, int size, int count);
extern void device_alloc(pid_t *devices, int size, int count, pid_t pid);
extern void device_free(pid_t *devices, int size, pid_t pid);

// helper function
extern bool load_dispatcher(dispatcher_t *dispatcher);

#endif
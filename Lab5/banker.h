/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018, Karan Jariwala (100619029), Harsh Patel (100580778), Aakash Patel (100616630), Karan Patel (100621178)
 * All rights reserved.
 * 
 */
#ifndef BANKER_H_
#define BANKER_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CUSTOMERS 5
#define NUM_RESOURCES 3

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_res = PTHREAD_MUTEX_INITIALIZER;

int available[NUM_RESOURCES];
int maximum[NUM_CUSTOMERS][NUM_RESOURCES];
int allocation[NUM_CUSTOMERS][NUM_RESOURCES];
int need[NUM_CUSTOMERS][NUM_RESOURCES];

extern bool request_res(int n_customer);
extern void release_res(int n_customer);

extern void *thread_runner(void *params);
extern void subtractor();
extern void print_matrix(int arr[NUM_CUSTOMERS][NUM_RESOURCES]);
extern void generate_grid();
extern void generate_allocate_matrix();
extern void print_need(int n_customer);
void print_avail();

#endif /* BANKER_H_ */
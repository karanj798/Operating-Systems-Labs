/*
 * Banker's Algorithm for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018, Karan Jariwala (100619029), Harsh Patel (100580778), Aakash Patel (100616630), Karan Patel (100621178)
 * All rights reserved.
 * 
 */

#include "banker.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Invalid. \n");
        printf("Usage: \n\t./banker 10 5 7\n");
        exit(0);
    }
    srand(time(NULL));

    for (int i = 0; i < 3; i++)
    {
        available[i] = atoi(argv[i + 1]);
    }

    generate_grid();
    printf("Maximum Resources Matrix\n");
    print_matrix(maximum);

    generate_allocate_matrix();
    printf("Allocation Matrix\n");
    print_matrix(allocation);

    printf("Need Matrix\n");
    subtractor();
    print_matrix(need);

    pthread_t c_threads[NUM_CUSTOMERS];
    int customer_id[NUM_CUSTOMERS];
    int tid;
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        customer_id[i] = i;
        tid = pthread_create(&c_threads[i], NULL, thread_runner, (void *)&customer_id[i]);
        if (tid)
        {
            printf("Thread Creation Error %d", tid);
            exit(0);
        }
    }
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        pthread_join(c_threads[i], NULL);
    }

    return EXIT_SUCCESS;
} // Release resources, returns true if successful

void *thread_runner(void *params)
{
    bool flag = false;
    int id = *(int *)params;

    while (!flag)
    {
        pthread_mutex_lock(&mutex);
        if (request_res(id))
        {
            flag = true;
        }
        pthread_mutex_unlock(&mutex);

        sleep(2); //needed to address all the requests before releasing resources.

        pthread_mutex_lock(&mutex);
        if (flag)
        {
            release_res(id);
        }
        pthread_mutex_unlock(&mutex);

        if (flag)
        {
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
}

bool request_res(int n_customer)
{
    bool flag = true;
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        flag = flag && (need[n_customer][i] < available[i]);
    }
    pthread_mutex_lock(&mutex_res);
    printf("Customer: %d\n-------------\n", n_customer + 1);
    printf("\tRequest\t\tAvailable\tState\n\t");
    pthread_mutex_unlock(&mutex_res);
    if (flag)
    {
        for (int i = 0; i < NUM_RESOURCES; i++)
        {
            available[i] = available[i] - need[n_customer][i];
        }
        pthread_mutex_lock(&mutex_res);
        print_need(n_customer);
        printf("\t\t");
        print_avail();
        printf("\t\tSafe\nRequest Granted.\n\n");
        sleep(1);
        pthread_mutex_unlock(&mutex_res);
        return true;
    }
    else
    {
        pthread_mutex_lock(&mutex_res);
        print_need(n_customer);
        printf("\t\t");
        print_avail();
        printf("\t\tNot Safe\nRequest Denied.\n\n");
        sleep(1);
        pthread_mutex_unlock(&mutex_res);
        return false;
    }
}

void release_res(int n_customer)
{
    pthread_mutex_lock(&mutex_res);
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        available[i] = available[i] + need[n_customer][i];
    }
    printf("Customer: %d\n-------------\n", n_customer + 1);
    printf("\tRelease\t\tAvailable\n\t");
    print_need(n_customer);
    printf("\t\t");
    print_avail();
    printf("\n\n");
    pthread_mutex_unlock(&mutex_res);
}
void generate_grid()
{
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            maximum[i][j] = rand() % available[j];
        }
    }
}

void generate_allocate_matrix()
{
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            if (maximum[i][j] == 0)
            {
                allocation[i][j] = 0;
            }
            else
            {
                allocation[i][j] = rand() % maximum[i][j];
            }
        }
    }
}

void subtractor()
{
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }
}

void print_matrix(int arr[NUM_CUSTOMERS][NUM_RESOURCES])
{
    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        for (int j = 0; j < NUM_RESOURCES; j++)
        {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

void print_need(int n_customer)
{
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        printf("%d ", need[n_customer][i]);
    }
}

void print_avail()
{
    for (int i = 0; i < NUM_RESOURCES; i++)
    {
        printf("%d ", available[i]);
    }
}
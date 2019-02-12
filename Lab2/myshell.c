/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017
 * All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include "utility.h"
#include "myshell.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256



int main(int argc, char *argv[], char *envp[])
{

    char buffer[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};
    char arg[20][200] = {{0}};
    printf("%s", buffer);

    print_prompt();
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
        if (strcmp(buffer, "\n"))
        {
            char *str = strtok(buffer, "\n");
            char *com = strtok(str, " ");
            char *tok = strtok(NULL, " ");

            int count = 0;
            while (tok != NULL)
            {
                strcpy(arg[count], tok);
                tok = strtok(NULL, " ");
                count++;
            }
            strcpy(command, com);

            
        }
        print_prompt();
    }
    return EXIT_SUCCESS;
}
void print_help()
{
    FILE *file;
    file = fopen("readme", "r");
    if (file == NULL)
    {
        printf("Cannot open file\n");
    }
    char c = fgetc(file);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(file);
    }
    fclose(file);
}

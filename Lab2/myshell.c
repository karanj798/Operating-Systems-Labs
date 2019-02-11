/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2017, <GROUP MEMBERS>
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

#define BUFFER_LEN 256


void changeDIR(char *command, char *arg)
{
    getcwd(command, sizeof(command));
    // Check for any argument
    if (strcmp(arg, "") == 0)
    {
        printf("Still at current path.");
    }
    else
    {
        //Sets the environment variable
        setenv("PWD", arg, 2);
        //Checks if directory exists
        if (chdir(getenv("PWD")) != 0)
        {
            printf("Directory Not Found.\n");
        }
        else
        {
            printf("Now at directory %s\n", getenv("PWD"));
        }
    }
}

int main()
{
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};
    char arg[BUFFER_LEN] = {0};
    char *result = NULL;

    // Parse the commands provided using argc and argv

    // Perform an infinite loop getting command input from users
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL)
    {
        // Perform string tokenization to get the command and argument
        result = strtok(buffer, " ");
        strcpy(command, result);
        result = strtok(NULL, " ");

        while (result != NULL)
        {
            strcat(arg, result);
            strcat(arg, " ");
            result = strtok(NULL, " ");
        }


        // cd <directory> - Change the current default directory to
        if (strcmp(command, "cd") == 0)
        {
            changeDIR(command, arg);
        }
      
        else if (strcmp(command, "clr") == 0)
        {
            printf("\%c[2J", 033);
        }
        else if (strcmp(command, "dir") == 0)
        {

        }
        else if (strcmp(command, "environ") == 0)
        {
            system("printenv");
        }

        else if (strcmp(command, "echo") == 0)
        {
            printf("%s\n", "");
        }

        
        // help - Display the user manual using the more filter.
        else if (strcmp(command, "help") == 0)
        {
            
        }
        
        // pause - Pause operation of the shell until 'Enter' is pressed.
        else if (strcmp(command, "pause") == 0)
        {
            
        }
    
        else if (strcmp(command, "quit") == 0)
        {
            return EXIT_SUCCESS;
        }

       
        else
        {
            fputs("Unsupported command, use help to display the manual\n", stderr);
        }
    }
    return EXIT_SUCCESS;
}

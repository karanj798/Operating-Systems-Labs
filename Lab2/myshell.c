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

// Put macros or constants here using #define
#define BUFFER_LEN 256

// Put global environment variables here

// Define functions declared in myshell.h here

int main(int argc, char *argv[])
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

        printf("Command: %s\n", command);
        printf("Args: %s\n", arg);


        // cd <directory> - Change the current default directory to
        if (strcmp(command, "cd") == 0)
        {
            
        }

        // clr - Clear the screen.
        else if (strcmp(command, "clr") == 0)
        {
            
        }

        // dir <directory> - List the contents of directory <directory>.
        else if (strcmp(command, "dir") == 0)
        {
            
        }
        
        // environ - List all the environment strings.
        else if (strcmp(command, "environ") == 0)
        {
            
        }
        
        // echo <comment> - Display <comment> on the display followed by a
        else if (strcmp(command, "echo") == 0)
        {
            printf("%s\n", arg);
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

/*
 * MyShell Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2018, Karan Jariwala (100619029), Harsh Patel (100580778), Aakash Patel (100616630), Karan Patel (100621178)
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
char PWD[256] = ".";
char SHELL[256];

// Defining couple of myshell functions
void print_pwd();
void print_prompt();
void print_dir();
void print_help();

int main(int argc, char *argv[], char *envp[])
{
    // File I/O
    if (argc == 2)
    {
        FILE *fileout;
        fileout = freopen(argv[1], "r", stdin);
        if (fileout == NULL)
        {
            printf("Cannot open file");
        }
        fclose(fileout);
    }

    char buffer[BUFFER_LEN] = {0};
    char command[BUFFER_LEN] = {0};
    char arg[20][200] = {{0}};

    print_prompt(); // Printing prompt

    // Perform an infinite loop getting command input from users
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

            // Checking which commands are inputted

            if (strcmp(command, "cd") == 0)
            {
                if (strcmp(arg[0], "cd ") != 0)
                {
                    if (chdir(arg[0]) == -1)
                    {
                        perror(arg[0]);
                        printf("Please enter a valid directory \n");
                    }
                }
                print_pwd();
            }
            else if (strcmp(command, "pwd") == 0)
            {
                print_pwd();
            }
            else if (strcmp(command, "clr") == 0)
            {
                printf("\033[H\033[J");
            }
            else if (strcmp(command, "dir") == 0)
            {
                print_dir();
            }
            else if (strcmp(command, "pause") == 0)
            {
                printf("Press ENTER to continue");
                char d;
                d = getchar();
                while (d != 0x0A)
                {
                    d = getchar();
                }
            }
            else if (strcmp(command, "environ") == 0)
            {
                for (char **env = envp; *env != 0; env++)
                {
                    char *thisEnv = *env;
                    printf("%s\n", thisEnv);
                }
            }
            else if (strcmp(command, "echo") == 0)
            {
                for (int i = 0; i < count; i++)
                {
                    printf("%s ", arg[i]);
                }
                printf("\n");
            }
            else if (strcmp(command, "help") == 0)
            {
                print_help();
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
        print_prompt();
    }
    return EXIT_SUCCESS;
}

void print_pwd()
{
    getcwd(PWD, sizeof(PWD));
    printf("\n%s\n", PWD);
}

void print_prompt()
{
    getcwd(PWD, sizeof(PWD));
    char *shl = strcat(PWD, "/myshell $ ");
    strcpy(SHELL, shl);
    printf("%s", SHELL);
}

void print_dir()
{
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
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

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

int sudoku[9][9];
bool row_invalid;

void *validate_row(void *param)
{
    //TODO: Use the logic of counting sort.

    for (int i = 0; i < 9; i++)
    {
        int temp_row[9] = {0};
        for (int j = 0; j < 9; j++)
        {
            if (temp_row[sudoku[i][j] - 1] == 0)
            {
                temp_row[sudoku[i][j] - 1] = 1;
            }
            else
            {
                row_invalid = true;
                break;
            }
        }
    }
    pthread_exit(NULL);
}

void *validate_col()
{
    for (int j = 0; j < 9; j++)
    {
        int temp_row[9] = {0};
        for (int i = 0; i < 9; i++)
        {
            if (temp_row[sudoku[i][j] - 1] == 0)
            {
                temp_row[sudoku[i][j] - 1] = 1;
            }
            else
            {
                row_invalid = true;
                break;
            }
        }
    }
    pthread_exit(NULL);
}

void *validate_box()
{
}

int main()
{

    char board[9][9];

    FILE *fpt;
    fpt = fopen("puzzle.txt", "r");

    if (fpt == NULL)
    {
        printf("File not found");
        return 0;
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fscanf(fpt, " %c ", &board[i][j]);
        }
    }

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] == '-')
            {
                board[i][j] = '0';
            }
            sudoku[i][j] = board[i][j] - '0';
            printf("%d", sudoku[i][j]);
        }
        printf("\n");
    }

    fclose(fpt);

    pthread_t t_row, t_col, t_box;
    if (pthread_create(&t_row, NULL, validate_row, NULL))
    {
        printf("Thread Error occurred.\n");
        exit(0);
    }

    if (pthread_create(&t_col, NULL, validate_col, NULL))
    {
        printf("Thread Error occurred.\n");
        exit(0);
    }
}

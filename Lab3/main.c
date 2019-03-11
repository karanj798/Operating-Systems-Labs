#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

int sudoku[9][9];
bool row_invalid = false, col_invalid = false;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int curr_row;
int row_sum;

struct index
{
    int i;
    int j;
};

void *solve_row()
{
    pthread_mutex_lock(&mutex);

    struct index missing_index;

    for (int j = 0; j < 9; j++)
    {
        printf("Row: %d, Col:%d, elem: %d\n", curr_row, j, sudoku[curr_row][j]);
        row_sum += sudoku[curr_row][j];
        if (sudoku[curr_row][j] == 0)
        {
            missing_index.i = curr_row;
            missing_index.j = j;
        }
    }

    printf("Row: %d, Sum: %d\n", curr_row, row_sum);
    printf("Missing: %d @ %d, %d\n", (45 - row_sum), missing_index.i, missing_index.j);
    printf("--------------------\n");
    sudoku[missing_index.i][missing_index.j] = 45 - row_sum;
    curr_row += 1;
    row_sum = 0;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void print_board()
{
    printf("+++++++++++++++++++++++++++++++++++++\n");

    for (int i = 0; i < 9; i++)
    {
        printf("|");
        for (int j = 0; j < 9; j++)
        {

            printf(" %d |", sudoku[i][j]);
        }
        printf("\n+++++++++++++++++++++++++++++++++++++\n");
    }
}

void parse_puzzel()
{
    char board[9][9];

    FILE *fpt;
    fpt = fopen("puzzle.txt", "r");

    if (fpt == NULL)
    {
        printf("File not found");
        return;
    }
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            fscanf(fpt, " %c ", &board[i][j]);
            if (board[i][j] == '-')
            {
                board[i][j] = '0';
            }
            sudoku[i][j] = board[i][j] - '0';
        }
    }
    fclose(fpt);
}

int main()
{
    parse_puzzel();
    pthread_t t_row[9],
        t_col, t_box;
    for (int i = 0; i < 9; i++)
    {
        if (pthread_create(&t_row[i], NULL, *solve_row, NULL))
        {
            printf("Thread Error occurred.\n");
            exit(0);
        }
    }

    for (int i = 0; i < 9; i++)
    {
        pthread_join(t_row[i], NULL);
    }

    print_board();
}

#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

int sudoku[9][9];
bool row_invalid = false, col_invalid = false;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int curr_row;
int curr_col;
int row_sum;
int col_sum;
int box_sum;

struct index
{
    int i;
    int j;
};

void *solve_row()
{
    pthread_mutex_lock(&mutex);

    struct index missing_index;
    printf("\n--- ROW LOG ---\n");
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

    if (45 > row_sum)
    {
        printf("Missing: %d @ %d, %d\n", (45 - row_sum), missing_index.i, missing_index.j);
        sudoku[missing_index.i][missing_index.j] = 45 - row_sum;
    }
    printf("--------------------\n");
    row_sum = 0;
    curr_row += 1;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *solve_col()
{
    pthread_mutex_lock(&mutex);
    printf("\n--- COL LOG ---\n");
    struct index missing_index;

    for (int i = 0; i < 9; i++)
    {
        printf("Row: %d, Col:%d, elem: %d\n", i, curr_col, sudoku[i][curr_col]);
        col_sum += sudoku[i][curr_col];
        if (sudoku[i][curr_col] == 0)
        {
            missing_index.i = i;
            missing_index.j = curr_col;
        }
    }

    printf("Col: %d, Sum: %d\n", curr_col, col_sum);
    if (45 > col_sum)
    {
        printf("Missing: %d @ %d, %d\n", (45 - col_sum), missing_index.i, missing_index.j);
        sudoku[missing_index.i][missing_index.j] = 45 - col_sum;
    }
    printf("--------------------\n");
    col_sum = 0;
    curr_col += 1;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void *solve_box(void *param)
{
    pthread_mutex_lock(&mutex);
    printf("\n--- BOX LOG ---\n");
    struct index *start_pos = param;
    struct index missing_index;

    int s_i = start_pos->i;
    int s_j = start_pos->j;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("Row: %d, Col:%d, elem: %d\n", i + s_i, j + s_j, sudoku[i + s_i][j + s_j]);
            box_sum += sudoku[i + s_i][j + s_j];
            if (sudoku[i + s_i][j + s_j] == 0)
            {
                missing_index.i = i + s_i;
                missing_index.j = j + s_j;
            }
        }
    }
    printf("Sum: %d\n", box_sum);
    if (45 > box_sum && (45 - box_sum) > 0 && (45 - box_sum) < 10)
    {
        printf("Missing: %d @ %d, %d\n", (45 - box_sum), missing_index.i, missing_index.j);
        sudoku[missing_index.i][missing_index.j] = 45 - box_sum;
    }
    printf("--------------------\n");
    box_sum = 0;
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
    pthread_t t_row[9], t_col[9], t_box[9];

    struct index coordinates[9];
    int counter = 0;
    for (int i = 0; i <= 6; i += 3)
    {
        for (int j = 0; j <= 6; j += 3)
        {
            coordinates[counter].i = i;
            coordinates[counter].j = j;
            if (pthread_create(&t_box[counter], NULL, *solve_box, (void *)&coordinates[counter]))
            {
                printf("Thread Error occurred.\n");
                exit(0);
            }
            counter++;
        }
    }

    for (int i = 0; i < 9; i++)
    {
        if (pthread_create(&t_row[i], NULL, *solve_row, NULL))
        {
            printf("Thread Error occurred.\n");
            exit(0);
        }
        if (pthread_create(&t_col[i], NULL, *solve_col, NULL))
        {
            printf("Thread Error occurred.\n");
            exit(0);
        }
    }

    for (int i = 0; i < 9; i++)
    {
        pthread_join(t_row[i], NULL);
        pthread_join(t_col[i], NULL);
        pthread_join(t_box[i], NULL);
    }

    print_board();
}

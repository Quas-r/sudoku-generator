#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRUE        1
#define FALSE       0
#define GRID_DIM    9
#define SQR_DIM     3

void display_grid(int grid[GRID_DIM][GRID_DIM]);
int check_column(int grid[GRID_DIM][GRID_DIM], int value, int row, int column);
int check_row(int grid[GRID_DIM][GRID_DIM], int value, int row, int column);
int check_square(int grid[GRID_DIM][GRID_DIM], int value, int row, int column);
int try_insert_into_point(int grid[GRID_DIM][GRID_DIM], int value, int row, int column);
int try_insert_value(int grid[GRID_DIM][GRID_DIM], int value);
void setup_grid(int grid[GRID_DIM][GRID_DIM]);
int fill_grid(int grid[GRID_DIM][GRID_DIM], int numbers[]);
void randomize_numbers(int numbers[]);
void *thread(void* ptr);


int line_seperator_length = GRID_DIM + (GRID_DIM / SQR_DIM) + 1;
int total_cells = GRID_DIM * GRID_DIM;
long threads_done = FALSE;
pthread_mutex_t mutex;



int main(int argc, char *argv[])
{
    srand(time(NULL));

    pthread_t thread1, thread2, thread3, thread4, thread5, thread6, thread7, thread8;
    pthread_create(&thread1, NULL, *thread, (void*) threads_done);
    pthread_create(&thread2, NULL, *thread, (void*) threads_done);
    pthread_create(&thread3, NULL, *thread, (void*) threads_done);
    pthread_create(&thread4, NULL, *thread, (void*) threads_done);
    pthread_create(&thread5, NULL, *thread, (void*) threads_done);
    pthread_create(&thread6, NULL, *thread, (void*) threads_done);
    pthread_create(&thread7, NULL, *thread, (void*) threads_done);
    pthread_create(&thread8, NULL, *thread, (void*) threads_done);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(thread4, NULL);
    pthread_join(thread5, NULL);
    pthread_join(thread6, NULL);
    pthread_join(thread7, NULL);
    pthread_join(thread8, NULL);
    return 0;
}

void display_grid(int grid[GRID_DIM][GRID_DIM]) {
    for (int i = 0; i < GRID_DIM; i++) {
        if (i % SQR_DIM == 0) {
            for (int j = 0; j < line_seperator_length; j++) {
                putchar('-'); 
                putchar(' ');
            }
            putchar('\n');
        }
        for (int j = 0; j < GRID_DIM; j++) {
            if (j % SQR_DIM == 0) {
                putchar('|'); 
                putchar(' ');
            }
            putchar(grid[i][j] + '0');
            putchar(' ');
        }
        putchar('|');
        putchar('\n');
    }
    for (int j = 0; j < line_seperator_length; j++) {
        putchar('-'); 
        putchar(' ');
    }
}

int check_column(int grid[GRID_DIM][GRID_DIM], int value, int row, int column) {
    if (grid[row][column] != 0) {
        return FALSE;
    }
    for (int i = 0; i < GRID_DIM; i++) {
        if (i != row && grid[i][column] == value) { return FALSE; }
    }
    return TRUE;
}

int check_row(int grid[GRID_DIM][GRID_DIM], int value, int row, int column) {
    if (grid[row][column] != 0) {
        return FALSE;
    }
    for (int i = 0; i < GRID_DIM; i++) {
        if (i != column && grid[row][i] == value) { return FALSE; }
    }
    return TRUE;
}

int check_square(int grid[GRID_DIM][GRID_DIM], int value, int row, int column) {
    if (grid[row][column] != 0) {
        return FALSE;
    }
    int row_sqr_num = row / SQR_DIM;
    int column_sqr_num = column / SQR_DIM;
    int i, j, row_index, column_index;
    for (i = 0; i < SQR_DIM; i++) {
        row_index = row_sqr_num * SQR_DIM + i;
        for (j = 0; j < SQR_DIM; j++) {
            column_index = column_sqr_num * SQR_DIM + j;
            if (grid[row_index][column_index] == value) { return FALSE; }
        }
    }
    return TRUE;
}

int try_insert_into_point(int grid[GRID_DIM][GRID_DIM], int value, int row, int column) {
    if (check_square(grid, value, row, column) &&
            check_row(grid, value, row, column) &&
            check_column(grid, value, row, column)) {
        grid[row][column] = value;
        return TRUE;
    }
    return FALSE;
}

int try_insert_value(int grid[GRID_DIM][GRID_DIM], int value) {
    for (int i = 0; i < GRID_DIM; i++) {
        for (int j = 0; j < GRID_DIM; j++) {
            if (try_insert_into_point(grid, value, i, j)) { return TRUE; }
        }
    }
    return FALSE;
}

void setup_grid(int grid[GRID_DIM][GRID_DIM]) {
    for (int i = 0; i < GRID_DIM; i++) {
        for (int j = 0; j < GRID_DIM; j++) { grid[i][j] = 0; }
    }
}

int fill_grid(int grid[GRID_DIM][GRID_DIM], int numbers[]) {
    int filled_cells = 0;
    int attempts = 0;
    int value;
    while (filled_cells < total_cells) {
        // randomize_numbers(numbers);
        int changed = FALSE;
        for (int i = 0; i < 9; i++) {
            if (try_insert_value(grid, value)) {
                filled_cells++;
                changed = TRUE;
            }
        }
        if (!changed) return FALSE;
    }
    putchar('\n');
    return TRUE;
}

void *thread(void* ptr) {
    int grid[GRID_DIM][GRID_DIM];
    int numbers[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    long threads_done_t = FALSE;
    while (!threads_done) {
        setup_grid(grid);
        threads_done_t = fill_grid(grid, numbers);
        if (threads_done_t) {
            pthread_mutex_lock(&mutex);
            threads_done = TRUE;
            pthread_mutex_unlock(&mutex);
            display_grid(grid);
        }
    }
    return 0;
}

void randomize_numbers(int numbers[]) {
    int x, y, z;
    for (int i = 0; i < 5; i++) {
        x = rand() % 9;
        y = rand() % 9;
        z = numbers[x];
        numbers[x] = numbers[y];
        numbers[y] = z;
    }
}

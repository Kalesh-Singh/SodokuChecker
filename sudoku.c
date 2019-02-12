//
// Created by kalesh on 2/11/19.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9
#define ROWS 0
#define COLS 1
#define BOXES 2

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
        {1,0,0,0,0,0,0,0,0},
        {0,2,0,0,0,0,0,0,0},
        {0,0,3,0,0,0,0,0,0},
        {0,0,0,4,0,0,0,0,0},
        {0,0,0,0,5,0,0,0,0},
        {0,0,0,0,0,6,0,0,0},
        {0,0,0,0,0,0,7,0,0},
        {0,0,0,0,0,0,0,8,0},
        {0,0,0,0,0,0,0,0,9},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
    //check if the rowindex has duplicates
    int row_index = *((int *)args);
    int rows[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    for (int i = 0; i < SIZE; ++i) {
        if (rows[board[row_index][i] - 1] == 0){
            row_check[row_index] = false;
            return NULL;
        }
        rows[board[row_index][i] - 1] = 0;
    }
    row_check[row_index] = true;
    return NULL;
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {

    int col_index = *((int *)args);
    int cols[9] = {-1, -1, -1, -1, -1, -1, -1, -1, -1};
    for(int i = 0; i < SIZE; i++){
        if (cols[board[i][col_index] - 1] == 0) {
            col_check[col_index] = false;
            return NULL;
        }
        cols[board[i][col_index] - 1] = 0;
    }
    col_check[col_index] = true;
    return NULL;
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
    int boxNum = *((int *)args);
    int cols[10] = {false, false, false, false, false, false, false, false, false};
    int col_start = ((boxNum) % 3) * 3;
    int row_start = ((int)((boxNum) / 3)) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++){
            cols[board[row_start + i][col_start + j] - 1] = true;
        }
    }
    for (int i = 0; i < 10; i++){
        if (cols[i] == false) {
            box_check[boxNum] = false;
            return NULL;
        }
    }
    box_check[boxNum] = true;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() {
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);

    // 2. Create pthread_t objects for our threads.
    pthread_t threads[3][SIZE];

    // 3. Create a thread for each cell of each matrix operation.
    // Threads that will check the rows
    int rowArgs[SIZE];
    for (int i = 0; i < SIZE; i++) {
        rowArgs[i] = i;
        pthread_create(&(threads[ROWS][i]), NULL, checkRow, (void*) &rowArgs[i]);
    }

    // Threads that will check the cols
    int colArgs[SIZE];
    for (int i = 0; i < SIZE; i++) {
        colArgs[i] = i;
        pthread_create(&(threads[COLS][i]), NULL, checkCol, (void*) &colArgs[i]);
    }

    // Threads that will check the boxes
    int boxArgs[SIZE];
    for (int i = 0; i < SIZE; i++) {
        boxArgs[i] = i;
        pthread_create(&(threads[BOXES][i]), NULL, checkBox, (void*) &boxArgs[i]);
    }

    // 4. Wait for all threads to finish.
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < SIZE; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }

    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }

    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }

    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}

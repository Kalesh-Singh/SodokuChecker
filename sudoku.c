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
    int rowIndex = *((int *)args);
    int digits[SIZE+1] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    for (int i = 0; i < SIZE; ++i) {
        int digit = board[rowIndex][i];
        if (digits[digit] == 0){
            row_check[rowIndex] = false;
            return NULL;
        }
        if (digit != 0) {
            digits[digit] = 0;
        }
    }
    row_check[rowIndex] = true;
    return NULL;
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
    int colIndex = *((int *)args);
    int digits[SIZE+1] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    for(int i = 0; i < SIZE; i++) {
        int digit = board[i][colIndex];
        if (digits[digit] == 0) {
            col_check[colIndex] = false;
            return NULL;
        }
        if (digit != 0) {
            digits[digit] = 0;
        }
    }
    col_check[colIndex] = true;
    return NULL;
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
/*
 *   Columns  0  1  2   3  4  5  6  7  8
 *           |--------|--------|--------|
 *  Rows 0   |  Box 0 |  Box 1 |  Box 2 |
 *       1   | %3 = 0 | %3 = 1 | %3 = 2 |
 *       2   | /3 = 0 | /3 = 0 | /3 = 0 |
 *           |--------|--------|--------|
 *       3   |  Box 3 |  Box 4 |  Box 5 |
 *       4   | %3 = 0 | %3 = 1 | %3 = 2 |
 *       5   | /3 = 1 | /3 = 1 | /3 = 1 |
 *           |--------|--------|--------|
 *       6   |  Box 6 |  Box 7 |  Box 8 |
 *       7   | %3 = 0 | %3 = 1 | %3 = 2 |
 *       8   | /3 = 2 | /3 = 2 | /3 = 2 |
 *           |--------|--------|--------|
 *
 */
void* checkBox(void* args) {
    int boxNum = *((int *)args);
    int digits[SIZE+1] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    int colStart = ((boxNum) % 3) * 3;
    int rowStart = ((int)((boxNum) / 3)) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++){
            int digit = board[rowStart+i][colStart+j];
            if (digits[digit] == 0) {
                box_check[boxNum] = false;
                return NULL;
            }
            if (digit != 0) {
                digits[digit] = 0;
            }
        }
    }
    box_check[boxNum] = true;
    return NULL;
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

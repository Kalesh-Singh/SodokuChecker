# Multi-threaded Sudoku Checker ##

This is a simple multithreaded sudoku checker, that utilizes the pthread library.

## Specifications ##
+ There are 9 rows in the sudoku grid.
+ There are 9 columns in the sudoku grid.
+ There are 9 `3x3` boxes in the sudoku grid. 
+ A row is correct if it doesn't contain any duplicates of the digits [1-9].
+ A column is correct if it doesn't contain any duplicated of the digits [1-9]
+ A box is correct if it doesn't contain any duplicates of the digits [1-9].
+ Digit `0` is not checked, as it is used to represent a position that hasn't been filled in.

## Multi-threading ##
The checker is implemented to use 27 threads:
+ 9 for checking each of the rows.
+ 9 for checking each of the columns.
+ 9 for checking each of the boxes.

The main thread joins each of these threads and waits for them to perform the appropriate check procedure (`row_check, col_check, box_check`).

## Compliling the Checker ##
The checker can be complied by running the `make` command.
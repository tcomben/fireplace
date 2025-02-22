#ifndef GRIDUTILS_H
#define GRIDUTILS_H

#ifndef CELL_TYPE
#error CELL_TYPE must be defined as a data type before the inclusion of grid_utils.h
#endif

#include <stdlib.h>

#define MIN(X, Y)       ((X) < (Y) ? (X) : (Y))
#define MAX(X, Y)       ((X) > (Y) ? (X) : (Y))
#define CLAMP(X, A, B)  (MAX((A),MIN((X),(B))))

/**
 * Initializes a new grid of cells to be printed in the terminal
 * @param rows the number of rows in the terminal
 * @param cols the number of columns in the terminal
 */
CELL_TYPE** new_grid(size_t rows, size_t cols) {
    CELL_TYPE** out = malloc(rows * sizeof(CELL_TYPE*));
    for (size_t i = 0; i < rows; i++) {
        //initialize the grid to be all zeroes
        out[i] = calloc(cols, sizeof(CELL_TYPE));
    }
    return out;
}

/**
 * Frees a grid previously created by the function new_grid
 * @param rows number of rows in the grid
 */
void free_grid(CELL_TYPE** in, size_t rows)
{
    for (size_t i = 0; i < rows; i++) {
        free(in[i]);
    }
    free(in);
}

/**
 * Copies grid data from src to dest
 * @param dest the grid into which the data will be copied
 * @param src the grid from which the data will be copied
 * @param rows the number of rows to be copied
 * @param cols the number of columns to be copied
 */
void copy_grid(CELL_TYPE*** dest, CELL_TYPE*** src, size_t rows, size_t cols){
     for(size_t i = 0; i < rows; i++){
         for (size_t j = 0; j < cols; j++){
             (*dest)[i][j] = (*src)[i][j];
         }
     }
 }

/**
 * Resize a grid with a custom copy function. Useful for when the terminal is resized and/or when
 * a SIGWINCH is caught. A user may wish not to begin copying from the top left of the screen moving
 * left-to-right. For this reason we allow the user to define their own copy routine and pass it to
 * this function as a function pointer.
 * @param grid the grid to be resized
 * @param old_r the current (old) number of rows
 * @param old_c the current (old) number of columns
 * @param new_r the number of rows in the resized grid
 * @param new_c the number of columns in the resized grid
 * @param copyfunc a function pointer to a user-defined function to perform the array copying
 */
void resize_grid_cust(CELL_TYPE*** grid, size_t old_r, size_t old_c, size_t new_r, size_t new_c,
    void (*copyfunc)(CELL_TYPE***, CELL_TYPE***, const size_t, const size_t) )
{
    CELL_TYPE** resized = new_grid(new_r, new_c);
    size_t row_copylimit = MIN(old_r, new_r);
    size_t col_copylimit = MIN(old_c, new_c);
    copyfunc(&resized, grid, row_copylimit, col_copylimit);
    free_grid(*grid, old_r);
    *grid = resized;
}

/**
 * Resize a grid. Useful for when the terminal is resized and/or when a SIGWINCH is caught
 * @param grid the grid to be resized
 * @param old_r the current (old) number of rows
 * @param old_c the current (old) number of columns
 * @param new_r the number of rows in the resized grid
 * @param new_c the number of columns in the resized grid
 */
void resize_grid(CELL_TYPE*** grid, size_t old_r, size_t old_c, size_t new_r, size_t new_c){
    resize_grid_cust(grid, old_r, old_c, new_r, new_c, &copy_grid);
}

#endif /* GRIDUTILS_H */

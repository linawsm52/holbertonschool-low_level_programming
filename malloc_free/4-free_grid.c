#include "main.h"
#include <stdlib.h>

/**
 * free_grid - frees a 2D grid previously created by alloc_grid
 * @grid: pointer to an array of row pointers
 * @height: number of rows in the grid
 *
 * Frees each row first, then frees the array of row pointers.
 * Does nothing if @grid is NULL or @height is non-positive.
 */
void free_grid(int **grid, int height)
{
    int i;

    if (grid == NULL || height <= 0)
        return;

    /* Free each row first */
    for (i = 0; i < height; i++)
        free(grid[i]);

    /* Then free the array of row pointers */
    free(grid);
}

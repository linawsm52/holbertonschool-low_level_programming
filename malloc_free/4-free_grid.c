#include "main.h"
#include <stdlib.h>

/**
 * free_grid - frees a 2D grid previously created by alloc_grid
 * @grid: pointer to an array of int* (rows)
 * @height: number of rows in the grid
 *
 * Return: void
 */
void free_grid(int **grid, int height)
{
    int i;

    if (grid == NULL)
        return;

    /* Free each row */
    for (i = 0; i < height; i++)
        free(grid[i]);

    /* Free the array of row pointers */
    free(grid);
}

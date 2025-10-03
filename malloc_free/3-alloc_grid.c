#include "main.h"
#include <stdlib.h>

/**
 * alloc_grid - allocates a 2D grid of ints initialized to 0
 * @width: number of columns
 * @height: number of rows
 * Return: pointer to grid, or NULL on failure/invalid size
 */
int **alloc_grid(int width, int height)
{
    int **grid;
    int i, j;

    if (width <= 0 || height <= 0)
        return NULL;

    grid = malloc(sizeof(int *) * height);
    if (grid == NULL)
        return NULL;

    for (i = 0; i < height; i++)
    {
        grid[i] = malloc(sizeof(int) * width);
        if (grid[i] == NULL)
        {
            /* clean up rows already allocated */
            while (i--)
                free(grid[i]);
            free(grid);
            return NULL;
        }
        for (j = 0; j < width; j++)
            grid[i][j] = 0;
    }

    return grid;
}

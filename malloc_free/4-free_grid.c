#include "main.h"
#include <stdlib.h>

/**
 * free_grid - frees a 2D grid previously created by alloc_grid
 * @grid: pointer to rows
 * @height: number of rows
 */
void free_grid(int **grid, int height)
{
    int i;

    if (grid == NULL || height <= 0)
        return;

    /* حرر الصفوف أولاً */
    for (i = 0; i < height; i++)
        free(grid[i]);

    /* بعدين حرر مصفوفة مؤشرات الصفوف */
    free(grid);
}

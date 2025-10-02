#include "main.h"

/**
 * print_square - prints a square using '#'
 * @size: size of the square (number of rows and columns)
 *
 * Return: void
 */
void print_square(int size)
{
	int r, c;

	if (size <= 0)
	{
		_putchar('\n');
		return;
	}

	for (r = 0; r < size; r++)
	{
		for (c = 0; c < size; c++)
			_putchar('#');
		_putchar('\n');
	}
}

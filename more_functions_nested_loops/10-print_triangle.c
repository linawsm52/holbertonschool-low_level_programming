#include "main.h"

/**
 * print_triangle - prints a triangle, followed by a new line
 * @size: size of the triangle
 *
 * Return: void
 */
void print_triangle(int size)
{
	int row, space, hash;

	if (size <= 0)
	{
		_putchar('\n');
	}
	else
	{
		for (row = 1; row <= size; row++)
		{
			/* اطبع المسافات قبل الـ # */
			for (space = size - row; space > 0; space--)
				_putchar(' ');

			/* اطبع الـ # */
			for (hash = 1; hash <= row; hash++)
				_putchar('#');

			_putchar('\n');
		}
	}
}

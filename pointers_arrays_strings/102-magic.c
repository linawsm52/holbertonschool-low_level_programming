#include "main.h"

/**
 * print_buffer - prints a buffer’s content in hex + ASCII
 * @b: pointer to buffer
 * @size: number of bytes to print
 */
void print_buffer(char *b, int size)
{
	int i, j, k;

	if (size <= 0)
	{
		_putchar('\n');
		return;
	}

	for (i = 0; i < size; i += 10)
	{
		/* print offset */
		for (j = 28; j >= 0; j -= 4)
			_putchar(((i >> j) & 0xF) + (((i >> j) & 0xF) < 10 ? '0' : 'a' - 10));

		_putchar(':');
		_putchar(' ');

		/* print hex bytes */
		for (j = 0; j < 10; j++)
		{
			if (j % 2 == 0 && j != 0)
				_putchar(' ');
			if (i + j < size)
			{
				unsigned char c = b[i + j];
				for (k = 4; k >= 0; k -= 4)
					_putchar(((c >> k) & 0xF) + (((c >> k) & 0xF) < 10 ? '0' : 'a' - 10));
			}
			else
			{
				_putchar(' ');
				_putchar(' ');
			}
		}

		_putchar(' ');

		/* print ASCII */
		for (j = 0; j < 10 && i + j < size; j++)
		{
			char c = b[i + j];
			if (c >= 32 && c <= 126)
				_putchar(c);
			else
				_putchar('.');
		}

		_putchar('\n');
	}
}

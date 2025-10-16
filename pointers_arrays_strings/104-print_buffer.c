#include "main.h"
#include <stdio.h>

/**
 * print_buffer - prints the content of a buffer
 * @b: pointer to the buffer
 * @size: number of bytes to print from @b
 *
 * Format:
 * 00000000: 5468 6973 2069 7320 6120  This is a
 * Each line shows:
 * - the offset (8 hex chars), then ": "
 * - 10 bytes as hex, printed as 5 groups of 2 bytes (pairs) separated by a
 *   single space. Missing bytes are padded with spaces to keep alignment.
 * - a space, then the ASCII representation (non-printables as '.')
 * If size <= 0, just print a newline.
 */
void print_buffer(char *b, int size)
{
	int i, j;

	if (size <= 0)
	{
		printf("\n");
		return;
	}

	for (i = 0; i < size; i += 10)
	{
		/* offset */
		printf("%08x: ", i);

		/* hex part: 10 bytes => 5 groups of 2 bytes */
		for (j = 0; j < 10; j++)
		{
			if (j && j % 2 == 0)
				printf(" ");

			if (i + j < size)
				printf("%02x", (unsigned char)b[i + j]);
			else
				printf("  ");
		}

		printf(" ");

		/* ascii part */
		for (j = 0; j < 10 && i + j < size; j++)
		{
			unsigned char c = (unsigned char)b[i + j];

			if (c >= 32 && c <= 126) /* printable */
				printf("%c", c);
			else
				printf(".");
		}
		printf("\n");
	}
}

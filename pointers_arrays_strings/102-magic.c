#include "main.h"
#include <stdio.h>

/**
 * print_buffer - prints the content of a buffer
 * @b: pointer to the buffer
 * @size: number of bytes to print
 *
 * Each line shows:
 * - the offset (8 hex chars)
 * - 10 bytes of hex (2 chars each), grouped in pairs, separated by a space
 * - the printable ASCII representation ('.' for non-printables)
 * If size <= 0, prints only a newline.
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
		printf("%08x: ", i);

		for (j = 0; j < 10; j++)
		{
			if (j % 2 == 0 && j != 0)
				printf(" ");
			if (i + j < size)
				printf("%02x", (unsigned char)b[i + j]);
			else
				printf("  ");
		}

		printf(" ");

		for (j = 0; j < 10 && i + j < size; j++)
		{
			unsigned char c = b[i + j];
			if (c >= 32 && c <= 126)
				printf("%c", c);
			else
				printf(".");
		}
		printf("\n");
	}
}

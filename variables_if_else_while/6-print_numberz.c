#include <stdio.h>

/**
 * main - Entry point
 *
 * Description: prints all single digit numbers of base 10
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	int i;

	for (i = 0; i < 10; i++)
		putchar(i + '0'); /* i=0 => '0', i=1 => '1' ... */

	putchar('\n');
	return (0);
}


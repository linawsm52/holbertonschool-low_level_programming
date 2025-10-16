#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * main - generates a valid random password for 101-crackme
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	int sum = 0, ch;

	srand(time(NULL));

	while (sum < 2772)
	{
		ch = rand() % 128;
		if ((sum + ch) > 2772)
			break;
		sum += ch;
		putchar(ch);
	}
	putchar(2772 - sum);
	return (0);
}

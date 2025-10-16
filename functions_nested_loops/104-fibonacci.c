#include <stdio.h>

/**
 * main - prints the first 98 Fibonacci numbers, starting with 1 and 2
 * Return: Always 0 (Success)
 */
int main(void)
{
	unsigned long int i, j, k, j1, j2, k1, k2;

	i = 0;
	j = 1;

	for (k = 1; k <= 98; k++)
	{
		unsigned long int next;

		next = i + j;
		if (next < 10000000000)
		{
			printf("%lu", next);
		}
		else
		{
			j1 = i / 10000000000;
			j2 = i % 10000000000;
			k1 = j / 10000000000;
			k2 = j % 10000000000;

			j1 = j1 + k1;
			j2 = j2 + k2;

			if (j2 >= 10000000000)
			{
				j1++;
				j2 %= 10000000000;
			}

			printf("%lu%010lu", j1, j2);
		}
		i = j;
		j = next;
		if (k != 98)
			printf(", ");
	}
	printf("\n");
	return (0);
}

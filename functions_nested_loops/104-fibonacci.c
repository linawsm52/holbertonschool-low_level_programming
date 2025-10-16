#include <stdio.h>

/**
 * main - prints the first 98 Fibonacci numbers, starting with 1 and 2
 * Return: Always 0 (Success)
 */
int main(void)
{
	unsigned long int i, j, k;
	unsigned long int i1, i2, j1, j2, k1, k2;

	i = 1;
	j = 2;

	printf("%lu, %lu", i, j);

	for (k = 3; k <= 98; k++)
	{
		if (i + j < 10000000000)
		{
			printf(", %lu", i + j);
			j = i + j;
			i = j - i;
		}
		else
		{
			i1 = i / 10000000000;
			i2 = i % 10000000000;
			j1 = j / 10000000000;
			j2 = j % 10000000000;

			k1 = i1 + j1;
			k2 = i2 + j2;

			if (k2 >= 10000000000)
			{
				k1 += 1;
				k2 %= 10000000000;
			}

			printf(", %lu%010lu", k1, k2);

			i1 = j1;
			i2 = j2;
			j1 = k1;
			j2 = k2;
			i = 0;
			j = 0;
		}
	}
	printf("\n");
	return (0);
}

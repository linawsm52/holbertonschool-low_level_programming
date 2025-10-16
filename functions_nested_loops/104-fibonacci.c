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

	for (k = 1; k <= 91; k++)
	{
		printf("%lu, ", i);
		j = j + i;
		i = j - i;
	}
	i1 = i / 1000000000;
	i2 = i % 1000000000;
	j1 = j / 1000000000;
	j2 = j % 1000000000;
	for (; k <= 98; k++)
	{
		printf("%lu%09lu", j1, j2);
		if (k != 98)
			printf(", ");
		k1 = i1 + j1;
		k2 = i2 + j2;
		if (k2 >= 1000000000)
		{
			k1 += 1;
			k2 %= 1000000000;
		}
		i1 = j1;
		i2 = j2;
		j1 = k1;
		j2 = k2;
	}
	printf("\n");
	return (0);
}

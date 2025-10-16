#include <stdio.h>

/**
 * main - prints the first 98 Fibonacci numbers, starting with 1 and 2
 * Return: Always 0 (Success)
 */
int main(void)
{
	unsigned long int a = 1, b = 2, next;
	unsigned long int a1, a2, b1, b2, next1, next2;
	int i;

	printf("%lu, %lu", a, b);

	for (i = 3; i <= 98; i++)
	{
		if (a + b < 10000000000)
		{
			next = a + b;
			printf(", %lu", next);
			a = b;
			b = next;
		}
		else
		{
			if (a < 10000000000)
			{
				a1 = 0;
				a2 = a;
			}
			else
			{
				a1 = a / 10000000000;
				a2 = a % 10000000000;
			}

			if (b < 10000000000)
			{
				b1 = 0;
				b2 = b;
			}
			else
			{
				b1 = b / 10000000000;
				b2 = b % 10000000000;
			}

			next1 = a1 + b1;
			next2 = a2 + b2;
			if (next2 >= 10000000000)
			{
				next1 += 1;
				next2 %= 10000000000;
			}
			printf(", %lu%010lu", next1, next2);
			a1 = b1;
			a2 = b2;
			b1 = next1;
			b2 = next2;
			a = b;
			b = 0;
		}
	}
	printf("\n");
	return (0);
}

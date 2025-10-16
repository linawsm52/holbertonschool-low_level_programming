#include <stdio.h>

/**
 * main - Prints the first 98 Fibonacci numbers, starting with 1 and 2.
 *
 * Description: The program prints the first 98 Fibonacci numbers
 * without using arrays, pointers, or long double types.
 * Large numbers are handled by splitting them into two parts
 * (the high and low segments).
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	unsigned long int a = 1, b = 2, c;
	unsigned long int a1, a2, b1, b2, c1, c2;
	int i;

	/* Print the first two numbers */
	printf("%lu, %lu", a, b);

	/* Print Fibonacci numbers up to the 92nd value */
	for (i = 3; i <= 92; i++)
	{
		c = a + b;
		printf(", %lu", c);
		a = b;
		b = c;
	}

	/* Split the large numbers into two parts (high and low) */
	a1 = a / 10000000000;
	a2 = a % 10000000000;
	b1 = b / 10000000000;
	b2 = b % 10000000000;

	/* Print the remaining numbers up to the 98th Fibonacci number */
	for (i = 93; i <= 98; i++)
	{
		c1 = a1 + b1;
		c2 = a2 + b2;
		if (c2 >= 10000000000)
		{
			c1 += 1;
			c2 %= 10000000000;
		}
		printf(", %lu%010lu", c1, c2);
		a1 = b1;
		a2 = b2;
		b1 = c1;
		b2 = c2;
	}
	printf("\n");
	return (0);
}

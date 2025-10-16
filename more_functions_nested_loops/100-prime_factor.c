#include <stdio.h>
#include <math.h>

/**
 * main - finds and prints the largest prime factor of 612852475143
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	long number = 612852475143;
	long factor;

	/* divide by 2 until odd */
	while (number % 2 == 0)
		number /= 2;

	/* check odd factors starting from 3 */
	for (factor = 3; factor <= sqrt(number); factor += 2)
	{
		while (number % factor == 0)
			number /= factor;
	}

	/* if number > 2, it is prime */
	if (number > 2)
		printf("%ld\n", number);
	else
		printf("%ld\n", factor - 2);

	return (0);
}

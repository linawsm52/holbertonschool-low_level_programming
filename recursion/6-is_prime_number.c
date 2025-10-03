#include "main.h"

/* helper: checks divisors recursively up to sqrt(n) */
static int prime_helper(int n, int d)
{
	if (d > n / d)          /* equivalent to d * d > n without overflow */
		return (1);
	if (n % d == 0)
		return (0);
	return (prime_helper(n, d + 1));
}

/**
 * is_prime_number - returns 1 if n is a prime number, otherwise 0
 * @n: integer to check
 *
 * Return: 1 if prime, 0 otherwise
 */
int is_prime_number(int n)
{
	if (n <= 1)
		return (0);
	if (n == 2)
		return (1);
	if (n % 2 == 0)
		return (0);

	return (prime_helper(n, 3));
}

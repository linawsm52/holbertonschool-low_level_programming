#include "main.h"

/**
 * prime_check - recursive helper to test divisors
 * @n: number to test
 * @d: current divisor
 *
 * Return: 1 if prime so far, 0 if composite
 */
int prime_check(int n, int d)
{
	if (d > n / d)          /* نفس معنى d*d > n بدون خطر overflow */
		return (1);
	if (n % d == 0)
		return (0);
	return (prime_check(n, d + 1));
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

	return (prime_check(n, 2));
}

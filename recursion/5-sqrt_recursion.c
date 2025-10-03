#include "main.h"

/**
 * sqrt_helper - recursively searches for the natural square root
 *               by trying integers i = 1, 2, 3, ... until i*i passes n
 * @n: number we want the root of
 * @i: current candidate
 *
 * Return: i if i*i == n, -1 if n has no natural square root
 */
int sqrt_helper(int n, int i)
{
	/* stop if i^2 already exceeded n (use division to avoid overflow) */
	if (i > n / i)
		return (-1);

	/* i*i == n  <=>  i == n / i  and  n % i == 0 (no multiplication) */
	if (i == n / i && n % i == 0)
		return (i);

	return (sqrt_helper(n, i + 1));
}

/**
 * _sqrt_recursion - returns the natural square root of a number
 * @n: number
 *
 * Return: natural square root of n; -1 if n has no natural square root
 */
int _sqrt_recursion(int n)
{
	if (n < 0)
		return (-1);
	if (n == 0)
		return (0);
	if (n == 1)
		return (1);

	return (sqrt_helper(n, 1));
}

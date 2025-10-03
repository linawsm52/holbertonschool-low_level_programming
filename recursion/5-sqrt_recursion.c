#include "main.h"

/* helper: tries integers i = 0,1,2,... until i*i == n or passes it */
static int sqrt_helper(int n, int i)
{
	if (i > n / i)          /* avoids overflow from i * i */
		return (-1);
	if (i * i == n)
		return (i);
	return (sqrt_helper(n, i + 1));
}

/**
 * _sqrt_recursion - returns the natural square root of a number
 * @n: the number
 *
 * Return: natural square root of n; -1 if n has no natural square root
 */
int _sqrt_recursion(int n)
{
	if (n < 0)
		return (-1);
	return (sqrt_helper(n, 0));
}

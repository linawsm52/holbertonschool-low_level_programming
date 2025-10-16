#include "main.h"

/**
 * infinite_add - adds two numbers stored as strings
 * @n1: first number (string of digits)
 * @n2: second number (string of digits)
 * @r: buffer to store result
 * @size_r: size of buffer r
 *
 * Return: pointer to result inside r, or 0 if result can't fit
 */
char *infinite_add(char *n1, char *n2, char *r, int size_r)
{
	int i = 0, j = 0, k, carry = 0, sum;

	/* find lengths */
	while (n1[i] != '\0')
		i++;
	while (n2[j] != '\0')
		j++;

	/* r must have at least one char for '\0' */
	if (size_r < 2)
		return (0);

	/* write from the end of r to avoid a separate reverse step */
	k = size_r - 1;
	r[k] = '\0';
	k--;

	i--;
	j--;

	/* add from least significant digit to most significant */
	while (i >= 0 || j >= 0 || carry)
	{
		if (k < 0)              /* no more space in r */
			return (0);

		sum = carry;
		if (i >= 0)
			sum += n1[i--] - '0';
		if (j >= 0)
			sum += n2[j--] - '0';

		r[k--] = (sum % 10) + '0';
		carry = sum / 10;
	}

	/* result starts at r[k + 1] */
	return (r + k + 1);
}

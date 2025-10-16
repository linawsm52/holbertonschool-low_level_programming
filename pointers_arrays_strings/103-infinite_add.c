#include "main.h"

/**
 * infinite_add - adds two numbers stored as strings
 * @n1: first number
 * @n2: second number
 * @r: buffer to store the result
 * @size_r: size of the buffer
 *
 * Return: pointer to the result (r), or 0 if the result can't fit
 */
char *infinite_add(char *n1, char *n2, char *r, int size_r)
{
	int i = 0, j = 0, k, l, m, n;

	while (n1[i])
		i++;
	while (n2[j])
		j++;

	if (i > size_r || j > size_r)
		return (0);

	l = 0;
	i--;
	j--;
	m = 0;

	while (i >= 0 || j >= 0 || m)
	{
		n = m;
		if (i >= 0)
			n += n1[i--] - '0';
		if (j >= 0)
			n += n2[j--] - '0';

		if (n > 9)
		{
			m = 1;
			n -= 10;
		}
		else
			m = 0;

		if (l >= size_r - 1)
			return (0);

		r[l++] = n + '0';
	}
	r[l] = '\0';

	for (k = 0; k < l / 2; k++)
	{
		char temp = r[k];
		r[k] = r[l - 1 - k];
		r[l - 1 - k] = temp;
	}

	return (r);
}

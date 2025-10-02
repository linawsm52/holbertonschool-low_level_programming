#include "main.h"

/**
 * _atoi - convert a string to an integer
 * @s: the string to convert
 *
 * Return: the converted integer, or 0 if no digits are found
 */
int _atoi(char *s)
{
	int i = 0;
	int sign = 1;
	int result = 0;

	/* skip non-digits, track sign */
	while (s[i] != '\0' && (s[i] < '0' || s[i] > '9'))
	{
		if (s[i] == '-')
			sign *= -1;
		i++;
	}

	/* build value; build negative directly to handle INT_MIN */
	while (s[i] >= '0' && s[i] <= '9')
	{
		if (sign == 1)
			result = result * 10 + (s[i] - '0');
		else
			result = result * 10 - (s[i] - '0');
		i++;
	}

	return (result);
}

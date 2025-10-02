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
	int started = 0;
	int num = 0;

	while (s[i] != '\0')
	{
		if (s[i] == '-')
		{
			if (!started)
				sign = -sign;
		}
		else if (s[i] == '+')
		{
			/* ignore '+' before number */
		}
		else if (s[i] >= '0' && s[i] <= '9')
		{
			started = 1;
			num = num * 10 + (s[i] - '0');
		}
		else if (started)
		{
			break;
		}
		i++;
	}

	return (sign * num);
}

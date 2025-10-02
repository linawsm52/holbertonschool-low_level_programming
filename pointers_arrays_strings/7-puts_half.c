#include "main.h"

/**
 * puts_half - prints half of a string
 * @str: pointer to the string
 *
 * Return: void
 */
void puts_half(char *str)
{
	int len = 0;
	int start;

	/* احسب الطول */
	while (str[len] != '\0')
	{
		len++;
	}

	/* حدد البداية */
	if (len % 2 == 0)
		start = len / 2;
	else
		start = (len - 1) / 2 + 1;

	/* اطبع من النص */
	while (str[start] != '\0')
	{
		_putchar(str[start]);
		start++;
	}

	_putchar('\n');
}

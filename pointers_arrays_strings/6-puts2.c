#include "main.h"

/**
 * puts2 - prints every other character of a string
 *         starting with the first character
 * @str: pointer to the string
 *
 * Return: void
 */
void puts2(char *str)
{
	int i = 0;

	while (str[i] != '\0')
	{
		_putchar(str[i]);
		i++;
		if (str[i] == '\0') /* لو وصلنا للنهاية نوقف */
			break;
		i++;
	}
	_putchar('\n');
}

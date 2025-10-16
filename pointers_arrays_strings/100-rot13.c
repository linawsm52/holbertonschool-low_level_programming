#include "main.h"

/**
 * rot13 - encodes a string using ROT13 (in place)
 * @s: pointer to the string to encode
 *
 * Return: pointer to the same string (encoded)
 */
char *rot13(char *s)
{
	int i, j;
	char from[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char to[]   = "NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm";

	for (i = 0; s[i] != '\0'; i++)
	{
		for (j = 0; from[j] != '\0'; j++)
		{
			if (s[i] == from[j]) /* <- only if in the whole function */
			{
				s[i] = to[j];
				break;
			}
		}
	}
	return (s);
}

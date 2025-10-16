#include "main.h"

/**
 * rot13 - encodes a string using rot13
 * @s: string to encode
 *
 * Return: pointer to the encoded string (same buffer)
 */
char *rot13(char *s)
{
	char *p = s;
	char from[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	char to[]   = "NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm";
	int i, j;

	/* loop over each character in s */
	for (; *p != '\0'; p++)
	{
		/* loop over from[] to find a match */
		for (i = 0; from[i] != '\0'; i++)
		{
			if (*p == from[i]) /* only one if in the whole function */
			{
				*p = to[i];
				break;
			}
		}
	}
	return (s);
}

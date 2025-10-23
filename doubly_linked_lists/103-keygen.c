#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * f4 - finds max char and returns randomized value
 * @s: username
 * @len: length of username
 * Return: char for key[3]
 */
char f4(char *s, unsigned int len)
{
	unsigned long ch = 0;
	unsigned int i;

	for (i = 0; i < len; i++)
		if ((unsigned long)s[i] > ch)
			ch = (unsigned long)s[i];
	srand((unsigned int)(ch ^ 14UL));
	return ((char)(rand() & 63));
}

/**
 * f5 - computes sum of squares
 * @s: username
 * @len: length
 * Return: char for key[4]
 */
char f5(char *s, unsigned int len)
{
	unsigned long ch = 0;
	unsigned int i;

	for (i = 0; i < len; i++)
		ch += (unsigned long)(s[i] * s[i]);
	return ((char)((ch ^ 239UL) & 63));
}

/**
 * f6 - random looped value
 * @s: username
 * Return: char for key[5]
 */
char f6(char *s)
{
	unsigned long ch = 0;
	unsigned int i;

	for (i = 0; i < (unsigned int)s[0]; i++)
		ch = (unsigned long)rand();
	return ((char)((ch ^ 229UL) & 63));
}

/**
 * main - generates a valid key for crackme5
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 1 if usage error
 */
int main(int argc, char *argv[])
{
	unsigned int i, len;
	unsigned long ch;
	char *username;
	char key[7];
	unsigned long ops[] = {
		0x3877445248432d41UL, 0x42394530534e6c37UL,
		0x4d6e706762695432UL, 0x74767a5835737956UL,
		0x2b554c59634a474fUL, 0x71786636576a6d34UL,
		0x723161513346655aUL, 0x6b756f494b646850UL
	};

	if (argc != 2)
	{
		printf("Usage: %s username\n", argv[0]);
		return (1);
	}

	username = argv[1];
	len = (unsigned int)strlen(username);

	key[0] = (char)(((len ^ 59) & 63));

	ch = 0UL;
	for (i = 0; i < len; i++)
		ch += (unsigned long)username[i];
	key[1] = (char)(((ch ^ 79UL) & 63));

	ch = 1UL;
	for (i = 0; i < len; i++)
		ch *= (unsigned long)username[i];
	key[2] = (char)(((ch ^ 85UL) & 63));

	key[3] = f4(username, len);
	key[4] = f5(username, len);
	key[5] = f6(username);

	for (i = 0; i < 6; i++)
		putchar(((char *)ops)[(unsigned int)key[i]]);
	putchar('\n');

	return (0);
}

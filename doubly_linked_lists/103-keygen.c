/* 103-keygen.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * main - generates a key depending on a username for crackme5
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 on success, 1 on incorrect usage
 */
int main(int argc, char *argv[])
{
	unsigned int i, len;
	unsigned long ch;
	char *username;
	char key[7];
	unsigned long ops[] = {
		0x3877445248432d41UL,
		0x42394530534e6c37UL,
		0x4d6e706762695432UL,
		0x74767a5835737956UL,
		0x2b554c59634a474fUL,
		0x71786636576a6d34UL,
		0x723161513346655aUL,
		0x6b756f494b646850UL
	};

	if (argc != 2)
	{
		printf("Usage: %s username\n", argv[0]);
		return (1);
	}

	username = argv[1];
	len = (unsigned int) strlen(username);

	key[0] = (char)(((len ^ 59) & 63));

	ch = 0UL;
	for (i = 0; i < len; i++)
		ch += (unsigned long)username[i];
	key[1] = (char)(((ch ^ 79UL) & 63));

	ch = 1UL;
	for (i = 0; i < len; i++)
		ch *= (unsigned long)username[i];
	key[2] = (char)(((ch ^ 85UL) & 63));

	ch = 0UL;
	for (i = 0; i < len; i++)
		if ((unsigned long)username[i] > ch)
			ch = (unsigned long)username[i];
	srand((unsigned int)(ch ^ 14UL));
	key[3] = (char)(rand() & 63);

	ch = 0UL;
	for (i = 0; i < len; i++)
		ch += (unsigned long)(username[i] * username[i]);
	key[4] = (char)(((ch ^ 239UL) & 63));

	for (i = 0; i < (unsigned int)username[0]; i++)
		ch = (unsigned long)rand();
	key[5] = (char)(((ch ^ 229UL) & 63));

	/* print the 6 characters by indexing the bytes of ops */
	for (i = 0; i < 6; i++)
		putchar(((char *)ops)[(unsigned int) key[i]]);
	putchar('\n');

	return (0);
}

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
	unsigned long long ch;
	char *username;
	char key[7];
	/*
	 * ops contains 8 * 8 bytes = 64 chars when interpreted as bytes.
	 * Use unsigned long (with UL suffix) to store these 64-bit constants.
	 */
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
	len = (unsigned int)strlen(username);

	key[0] = ((len ^ 59) & 63);

	ch = 0;
	for (i = 0; i < len; i++)
		ch += username[i];
	key[1] = ((ch ^ 79) & 63);

	ch = 1;
	for (i = 0; i < len; i++)
		ch *= username[i];
	key[2] = ((ch ^ 85) & 63);

	ch = 0;
	for (i = 0; i < len; i++)
		if ((unsigned long long)username[i] > ch)
			ch = username[i];
	srand((unsigned int)(ch ^ 14));
	key[3] = (rand() & 63);

	ch = 0;
	for (i = 0; i < len; i++)
		ch += (username[i] * username[i]);
	key[4] = ((ch ^ 239) & 63);

	for (i = 0; i < (unsigned int)username[0]; i++)
		ch = rand();
	key[5] = ((ch ^ 229) & 63);

	/* print the 6 chars by indexing the bytes of ops */
	for (i = 0; i < 6; i++)
		putchar(((char *)ops)[key[i]]);
	putchar('\n');

	return (0);
}

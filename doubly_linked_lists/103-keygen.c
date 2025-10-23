/* 103-keygen.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * compute_first_three - compute key[0], key[1], key[2]
 * @s: username
 * @len: username length
 * @key: array to fill (at least 6 bytes)
 *
 * This fills key[0..2] according to crackme5 algorithm.
 */
void compute_first_three(char *s, unsigned int len, unsigned char key[6])
{
	unsigned long ch;
	unsigned int i;

	/* key[0] from length */
	key[0] = (unsigned char)(((len ^ 59) & 63));

	/* key[1] from sum of chars */
	ch = 0UL;
	for (i = 0; i < len; i++)
		ch += (unsigned long)s[i];
	key[1] = (unsigned char)(((ch ^ 79UL) & 63));

	/* key[2] from product of chars */
	ch = 1UL;
	for (i = 0; i < len; i++)
		ch *= (unsigned long)s[i];
	key[2] = (unsigned char)(((ch ^ 85UL) & 63));
}

/**
 * compute_last_three - compute key[3], key[4], key[5]
 * @s: username
 * @len: username length
 * @key: array to fill (at least 6 bytes)
 *
 * This fills key[3..5] using max char, sum of squares and rand loop.
 */
void compute_last_three(char *s, unsigned int len, unsigned char key[6])
{
	unsigned long ch;
	unsigned int i;

	/* key[3]: srand based on max char */
	ch = 0UL;
	for (i = 0; i < len; i++)
		if ((unsigned long)s[i] > ch)
			ch = (unsigned long)s[i];

	srand((unsigned int)(ch ^ 14UL));
	key[3] = (unsigned char)(rand() & 63);

	/* key[4]: sum of squares */
	ch = 0UL;
	for (i = 0; i < len; i++)
		ch += (unsigned long)(s[i] * s[i]);
	key[4] = (unsigned char)(((ch ^ 239UL) & 63));

	/* key[5]: loop rand username[0] times */
	ch = 0UL;
	for (i = 0; i < (unsigned int)s[0]; i++)
		ch = (unsigned long)rand();
	key[5] = (unsigned char)(((ch ^ 229UL) & 63));
}

/**
 * print_key - print 6-byte key using the ops table
 * @key: key array (6 bytes)
 * @ops: table with 8 unsigned long values (8 * 8 = 64 bytes)
 *
 * This reinterprets ops as a byte array and indexes it by key values.
 */
void print_key(unsigned char key[6], unsigned long ops[8])
{
	unsigned int i;

	for (i = 0; i < 6; i++)
		putchar(((char *)ops)[(unsigned int)key[i]]);
	putchar('\n');
}

/**
 * main - generate key for crackme5 username
 * @argc: number of arguments
 * @argv: argument vector
 * Return: 0 on success, 1 on usage error
 */
int main(int argc, char *argv[])
{
	unsigned int len;
	char *username;
	unsigned char key[6];
	unsigned long ops[8] = {
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

	compute_first_three(username, len, key);
	compute_last_three(username, len, key);
	print_key(key, ops);

	return (0);
}

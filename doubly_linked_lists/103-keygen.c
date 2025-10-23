/* 103-keygen.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * gen_k0 - generate key[0] from length
 * @len: length of username
 * Return: value for key[0]
 */
unsigned char gen_k0(unsigned int len)
{
	return (unsigned char)(((len ^ 59) & 63));
}

/**
 * gen_k1 - generate key[1] from sum of chars
 * @s: username
 * @len: length
 * Return: value for key[1]
 */
unsigned char gen_k1(char *s, unsigned int len)
{
	unsigned long ch = 0;
	unsigned int i;

	for (i = 0; i < len; i++)
		ch += (unsigned long)s[i];

	return (unsigned char)(((ch ^ 79UL) & 63));
}

/**
 * gen_k2 - generate key[2] from product of chars
 * @s: username
 * @len: length
 * Return: value for key[2]
 */
unsigned char gen_k2(char *s, unsigned int len)
{
	unsigned long ch = 1;
	unsigned int i;

	for (i = 0; i < len; i++)
		ch *= (unsigned long)s[i];

	return (unsigned char)(((ch ^ 85UL) & 63));
}

/**
 * gen_k3 - generate key[3] using max char and rand
 * @s: username
 * @len: length
 * Return: value for key[3]
 */
unsigned char gen_k3(char *s, unsigned int len)
{
	unsigned long ch = 0;
	unsigned int i;

	for (i = 0; i < len; i++)
		if ((unsigned long)s[i] > ch)
			ch = (unsigned long)s[i];

	srand((unsigned int)(ch ^ 14UL));
	return (unsigned char)(rand() & 63);
}

/**
 * gen_k4 - generate key[4] from sum of squares
 * @s: username
 * @len: length
 * Return: value for key[4]
 */
unsigned char gen_k4(char *s, unsigned int len)
{
	unsigned long ch = 0;
	unsigned int i;

	for (i = 0; i < len; i++)
		ch += (unsigned long)(s[i] * s[i]);

	return (unsigned char)(((ch ^ 239UL) & 63));
}

/**
 * gen_k5 - generate key[5] from looping rand
 * @s: username
 * Return: value for key[5]
 */
unsigned char gen_k5(char *s)
{
	unsigned long ch = 0;
	unsigned int i;

	for (i = 0; i < (unsigned int)s[0]; i++)
		ch = (unsigned long)rand();

	return (unsigned char)(((ch ^ 229UL) & 63));
}

/**
 * print_key - print 6-byte key using ops table
 * @key: key array
 * @ops: ops table (8 * 8 bytes = 64 bytes)
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
 * @argc: arg count
 * @argv: arg vector
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

	key[0] = gen_k0(len);
	key[1] = gen_k1(username, len);
	key[2] = gen_k2(username, len);
	key[3] = gen_k3(username, len);
	key[4] = gen_k4(username, len);
	key[5] = gen_k5(username);

	print_key(key, ops);

	return (0);
}

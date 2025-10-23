#include "hash_tables.h"

/**
 * hash_djb2 - Implementation of the djb2 algorithm
 * @str: The string used to generate the hash value
 *
 * Return: The calculated hash value
 */
unsigned long int hash_djb2(const unsigned char *str)
{
	unsigned long int hash;
	int c;

	hash = 5381;
	while ((c = *str++))
	{
		/* hash * 33 + c */
		hash = ((hash << 5) + hash) + c;
	}
	return (hash);
}

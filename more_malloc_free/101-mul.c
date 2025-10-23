#include "main.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * _isdigit - checks if a string contains only digits
 * @s: string to check
 * Return: 1 if all are digits, 0 otherwise
 */
int _isdigit(char *s)
{
	int i = 0;

	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/**
 * _strlen - returns the length of a string
 * @s: string
 * Return: length
 */
int _strlen(char *s)
{
	int i = 0;

	while (s[i] != '\0')
		i++;
	return (i);
}

/**
 * errors - prints "Error" and exits with status 98
 */
void errors(void)
{
	printf("Error\n");
	exit(98);
}

/**
 * main - multiplies two positive numbers
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	char *num1, *num2;
	int len1, len2, len, i, carry, n1, n2, *result, start = 0;

	if (argc != 3)
		errors();

	num1 = argv[1];
	num2 = argv[2];

	if (!_isdigit(num1) || !_isdigit(num2))
		errors();

	len1 = _strlen(num1);
	len2 = _strlen(num2);
	len = len1 + len2 + 1;

	result = malloc(sizeof(int) * len);
	if (!result)
		return (1);

	for (i = 0; i < len; i++)
		result[i] = 0;

	for (len1 = len1 - 1; len1 >= 0; len1--)
	{
		n1 = num1[len1] - '0';
		carry = 0;
		for (len2 = _strlen(num2) - 1; len2 >= 0; len2--)
		{
			n2 = num2[len2] - '0';
			carry += result[len1 + len2 + 1] + (n1 * n2);
			result[len1 + len2 + 1] = carry % 10;
			carry /= 10;
		}
		if (carry > 0)
			result[len1 + len2 + 1] += carry;
	}

	while (start < len - 1 && result[start] == 0)
		start++;

	/* handle zero result */
	if (start == len - 1)
	{
		printf("0\n");
		free(result);
		return (0);
	}

	for (; start < len - 1; start++)
		printf("%d", result[start]);
	printf("\n");

	free(result);
	return (0);
}

#include <stdio.h>

/**
 * main - prints the program name, followed by a new line
 * @argc: number of command-line arguments
 * @argv: array of pointers to the arguments
 *
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
	(void)argc;            /* silence unused-parameter warning */
	printf("%s\n", argv[0]);
	return (0);
}

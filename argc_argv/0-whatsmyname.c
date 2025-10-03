#include <stdio.h>

/**
 * main - prints the program name, followed by a new line
 * @argc: number of command-line arguments (unused)
 * @argv: array of command-line argument strings
 *
 * Return: Always 0.
 */
int main(int argc __attribute__((unused)), char *argv[])
{
	printf("%s\n", argv[0]);
	return (0);
}

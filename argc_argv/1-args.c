#include <stdio.h>

/**
 * main - prints the number of arguments passed to the program
 * @argc: number of command-line arguments
 * @argv: array of argument strings (unused)
 *
 * Return: Always 0.
 */
int main(int argc, char *argv[] __attribute__((unused)))
{
	printf("%d\n", argc - 1);
	return (0);
}

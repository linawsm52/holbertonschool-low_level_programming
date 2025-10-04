#include "function_pointers.h"

/**
 * print_name - prints a name using the function given
 * @name: name to print
 * @f: pointer to function that takes a char * and returns nothing
 *
 * Return: nothing
 */
void print_name(char *name, void (*f)(char *))
{
	if (name == NULL || f == NULL)
		return;

	f(name);
}

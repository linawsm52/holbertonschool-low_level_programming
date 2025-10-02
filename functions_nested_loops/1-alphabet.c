#include "main.h"

/**
 * print_alphabet - prints the alphabet in lowercase
 *
 * Description: This function prints all lowercase letters
 * from 'a' to 'z', followed by a new line.
 *
 * Return: void
 */
void print_alphabet(void)
{
    char ch;

    for (ch = 'a'; ch <= 'z'; ch++)
    {
        _putchar(ch);
    }
    _putchar('\n');
}

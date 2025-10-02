#include "main.h"

/**
 * _strcat - concatenates two strings
 * @dest: destination string
 * @src: source string
 *
 * Return: pointer to dest
 */
char *_strcat(char *dest, char *src)
{
    int i = 0;
    int j = 0;

    /* move i to the end of dest */
    while (dest[i] != '\0')
    {
        i++;
    }

    /* copy src into dest starting at the end */
    while (src[j] != '\0')
    {
        dest[i] = src[j];
        i++;
        j++;
    }

    dest[i] = '\0'; /* add null terminator */
    return (dest);
}

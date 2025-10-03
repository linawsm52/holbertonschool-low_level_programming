#include <stdlib.h>

/**
 * _strdup - returns a pointer to a newly-allocated duplicate of a string
 * @str: string to duplicate
 *
 * Return: pointer to new string (heap) on success,
 *         NULL if @str is NULL or if memory allocation fails.
 */
char *_strdup(char *str)
{
	char *dup;
	unsigned int len = 0, i;

	if (str == NULL)
		return (NULL);

	/* get length */
	while (str[len] != '\0')
		len++;

	dup = (char *)malloc((len + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);

	for (i = 0; i < len; i++)
		dup[i] = str[i];
	dup[len] = '\0';

	return (dup);
}

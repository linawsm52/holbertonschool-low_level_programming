#include <stdlib.h>

/**
 * str_concat - concatenates two strings into a newly allocated buffer
 * @s1: first string (may be NULL, treated as empty)
 * @s2: second string (may be NULL, treated as empty)
 *
 * Return: pointer to new "s1s2" string, or NULL on allocation failure.
 */
char *str_concat(char *s1, char *s2)
{
	char *res;
	unsigned int len1 = 0, len2 = 0, i, j;

	if (s1 != NULL)
		while (s1[len1] != '\0')
			len1++;

	if (s2 != NULL)
		while (s2[len2] != '\0')
			len2++;

	res = (char *)malloc((len1 + len2 + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);

	for (i = 0; i < len1; i++)
		res[i] = s1[i];

	for (j = 0; j < len2; j++)
		res[i + j] = s2[j];

	res[len1 + len2] = '\0';
	return (res);
}

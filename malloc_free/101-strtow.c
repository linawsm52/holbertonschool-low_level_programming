#include "main.h"
#include <stdlib.h>

/**
 * count_words - counts the number of words in a string
 * @str: the string
 * Return: number of words
 */
int count_words(char *str)
{
	int count = 0, in_word = 0;

	while (*str)
	{
		if (*str != ' ' && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (*str == ' ')
			in_word = 0;
		str++;
	}
	return (count);
}

/**
 * copy_word - copies a single word from a string
 * @str: pointer to start of word
 * @len: word length
 * Return: pointer to new word (malloc'd)
 */
char *copy_word(char *str, int len)
{
	char *word;
	int i;

	word = malloc(sizeof(char) * (len + 1));
	if (word == NULL)
		return (NULL);
	for (i = 0; i < len; i++)
		word[i] = str[i];
	word[i] = '\0';
	return (word);
}

/**
 * strtow - splits a string into words
 * @str: string to split
 * Return: pointer to an array of strings (words), or NULL on failure
 */
char **strtow(char *str)
{
	char **words;
	int i = 0, k = 0, len, word_count, start;

	if (str == NULL || *str == '\0')
		return (NULL);

	word_count = count_words(str);
	if (word_count == 0)
		return (NULL);

	words = malloc(sizeof(char *) * (word_count + 1));
	if (words == NULL)
		return (NULL);

	while (str[i])
	{
		while (str[i] == ' ')
			i++;
		if (str[i] == '\0')
			break;

		start = i;
		while (str[i] != ' ' && str[i] != '\0')
			i++;
		len = i - start;

		words[k] = copy_word(str + start, len);
		if (words[k] == NULL)
		{
			while (k--)
				free(words[k]);
			free(words);
			return (NULL);
		}
		k++;
	}
	words[k] = NULL;
	return (words);
}

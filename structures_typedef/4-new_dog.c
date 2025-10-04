#include "dog.h"
#include <stdlib.h>

/* Helpers without using <string.h> */

/**
 * _strlen - returns the length of a string
 * @s: string
 * Return: length (number of characters before '\0')
 */
static int _strlen(char *s)
{
	int i = 0;

	if (s == NULL)
		return (0);

	while (s[i] != '\0')
		i++;

	return (i);
}

/**
 * _strcpy - copies a string from src to dest (including '\0')
 * @dest: destination buffer
 * @src: source string
 * Return: pointer to dest
 */
static char *_strcpy(char *dest, char *src)
{
	int i = 0;

	if (dest == NULL || src == NULL)
		return (dest);

	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

/**
 * new_dog - creates a new dog (makes copies of name and owner)
 * @name: dog's name
 * @age: dog's age
 * @owner: dog's owner
 *
 * Return: pointer to the new dog_t, or NULL if any allocation fails
 */
dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *nd;
	int len_name, len_owner;

	if (name == NULL || owner == NULL)
		return (NULL);

	nd = malloc(sizeof(dog_t));
	if (nd == NULL)
		return (NULL);

	len_name = _strlen(name);
	len_owner = _strlen(owner);

	nd->name = malloc(len_name + 1);
	if (nd->name == NULL)
	{
		free(nd);
		return (NULL);
	}
	_strcpy(nd->name, name);

	nd->owner = malloc(len_owner + 1);
	if (nd->owner == NULL)
	{
		free(nd->name);
		free(nd);
		return (NULL);
	}
	_strcpy(nd->owner, owner);

	nd->age = age;
	return (nd);
}

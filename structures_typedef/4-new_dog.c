#include "dog.h"
#include <stdlib.h>
#include <string.h>

/**
 * new_dog - creates a new dog
 * @name: dog's name
 * @age: dog's age
 * @owner: dog's owner
 *
 * Return: pointer to new dog, or NULL if fail
 */
dog_t *new_dog(char *name, float age, char *owner)
{
	dog_t *new;
	char *new_name, *new_owner;

	if (name == NULL || owner == NULL)
		return (NULL);

	new = malloc(sizeof(dog_t));
	if (new == NULL)
		return (NULL);

	/* allocate and copy name */
	new_name = malloc(strlen(name) + 1);
	if (new_name == NULL)
	{
		free(new);
		return (NULL);
	}
	strcpy(new_name, name);

	/* allocate and copy owner */
	new_owner = malloc(strlen(owner) + 1);
	if (new_owner == NULL)
	{
		free(new_name);
		free(new);
		return (NULL);
	}
	strcpy(new_owner, owner);

	new->name = new_name;
	new->age = age;
	new->owner = new_owner;

	return (new);
}

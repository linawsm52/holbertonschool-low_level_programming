#include "dog.h"
#include <stdlib.h>

/**
 * free_dog - frees a dog_t structure and its allocated fields
 * @d: pointer to dog_t to free
 *
 * Return: nothing
 */
void free_dog(dog_t *d)
{
	if (d == NULL)
		return;

	if (d->name != NULL)
		free(d->name);

	if (d->owner != NULL)
		free(d->owner);

	free(d);
}

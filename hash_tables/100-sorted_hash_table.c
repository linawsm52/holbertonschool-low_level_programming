#include "hash_tables.h"

/**
 * shash_table_create - Creates a sorted hash table
 * @size: Size of the array
 * Return: Pointer to the new sorted hash table, or NULL on failure
 */
shash_table_t *shash_table_create(unsigned long int size)
{
	shash_table_t *ht;

	ht = malloc(sizeof(shash_table_t));
	if (ht == NULL)
		return (NULL);

	ht->size = size;
	ht->array = calloc(size, sizeof(shash_node_t *));
	if (ht->array == NULL)
	{
		free(ht);
		return (NULL);
	}

	ht->shead = NULL;
	ht->stail = NULL;
	return (ht);
}

/**
 * shash_table_set - Adds or updates an element in a sorted hash table
 * @ht: The hash table
 * @key: The key (cannot be empty)
 * @value: The value associated with the key
 * Return: 1 on success, 0 on failure
 */
int shash_table_set(shash_table_t *ht, const char *key, const char *value)
{
	unsigned long int index;
	shash_node_t *node, *tmp;
	char *value_copy;

	if (ht == NULL || key == NULL || *key == '\0' || value == NULL)
		return (0);

	index = key_index((const unsigned char *)key, ht->size);
	value_copy = strdup(value);
	if (value_copy == NULL)
		return (0);

	/* Check if key exists, update */
	for (node = ht->array[index]; node != NULL; node = node->next)
	{
		if (strcmp(node->key, key) == 0)
		{
			free(node->value);
			node->value = value_copy;
			return (1);
		}
	}

	/* Create new node */
	node = malloc(sizeof(shash_node_t));
	if (node == NULL)
	{
		free(value_copy);
		return (0);
	}
	node->key = strdup(key);
	if (node->key == NULL)
	{
		free(value_copy);
		free(node);
		return (0);
	}
	node->value = value_copy;
	node->next = ht->array[index];
	ht->array[index] = node;

	/* Insert into sorted list */
	node->sprev = node->snext = NULL;
	if (ht->shead == NULL)
	{
		ht->shead = ht->stail = node;
	}
	else
	{
		tmp = ht->shead;
		while (tmp && strcmp(node->key, tmp->key) > 0)
			tmp = tmp->snext;

		if (tmp == ht->shead)
		{
			node->snext = tmp;
			node->sprev = NULL;
			tmp->sprev = node;
			ht->shead = node;
		}
		else if (tmp == NULL)
		{
			node->sprev = ht->stail;
			ht->stail->snext = node;
			ht->stail = node;
		}
		else
		{
			node->snext = tmp;
			node->sprev = tmp->sprev;
			tmp->sprev->snext = node;
			tmp->sprev = node;
		}
	}
	return (1);
}

/**
 * shash_table_get - Retrieves a value associated with a key
 */
char *shash_table_get(const shash_table_t *ht, const char *key)
{
	unsigned long int index;
	shash_node_t *node;

	if (ht == NULL || key == NULL || *key == '\0')
		return (NULL);

	index = key_index((const unsigned char *)key, ht->size);
	for (node = ht->array[index]; node != NULL; node = node->next)
	{
		if (strcmp(node->key, key) == 0)
			return (node->value);
	}
	return (NULL);
}

/**
 * shash_table_print - Prints a sorted hash table in order
 */
void shash_table_print(const shash_table_t *ht)
{
	shash_node_t *node;
	int first = 1;

	if (ht == NULL)
		return;

	printf("{");
	for (node = ht->shead; node != NULL; node = node->snext)
	{
		if (!first)
			printf(", ");
		printf("'%s': '%s'", node->key, node->value);
		first = 0;
	}
	printf("}\n");
}

/**
 * shash_table_print_rev - Prints a sorted hash table in reverse order
 */
void shash_table_print_rev(const shash_table_t *ht)
{
	shash_node_t *node;
	int first = 1;

	if (ht == NULL)
		return;

	printf("{");
	for (node = ht->stail; node != NULL; node = node->sprev)
	{
		if (!first)
			printf(", ");
		printf("'%s': '%s'", node->key, node->value);
		first = 0;
	}
	printf("}\n");
}

/**
 * shash_table_delete - Deletes a sorted hash table
 */
void shash_table_delete(shash_table_t *ht)
{
	shash_node_t *node, *temp;

	if (ht == NULL)
		return;

	node = ht->shead;
	while (node)
	{
		temp = node->snext;
		free(node->key);
		free(node->value);
		free(node);
		node = temp;
	}
	free(ht->array);
	free(ht);
}

#include "hash_tables.h"

/**
 * shash_table_create - Creates a sorted hash table
 * @size: Size of the array
 * Return: Pointer to the new sorted hash table, or NULL on failure
 */
shash_table_t *shash_table_create(unsigned long int size)
{
	shash_table_t *ht = malloc(sizeof(shash_table_t));

	if (!ht)
		return (NULL);
	ht->size = size;
	ht->array = calloc(size, sizeof(shash_node_t *));
	if (!ht->array)
	{
		free(ht);
		return (NULL);
	}
	ht->shead = NULL;
	ht->stail = NULL;
	return (ht);
}

/**
 * shash_table_set - Adds an element to a sorted hash table
 * @ht: The sorted hash table
 * @key: The key (string)
 * @value: The value (string)
 * Return: 1 on success, 0 otherwise
 */
int shash_table_set(shash_table_t *ht, const char *key, const char *value)
{
	unsigned long int i;
	shash_node_t *node, *tmp;
	char *valcpy;

	if (!ht || !key || !*key || !value)
		return (0);
	i = key_index((const unsigned char *)key, ht->size);
	for (node = ht->array[i]; node; node = node->next)
		if (strcmp(node->key, key) == 0)
		{
			free(node->value);
			node->value = strdup(value);
			return (node->value ? 1 : 0);
		}
	node = malloc(sizeof(shash_node_t));
	if (!node)
		return (0);
	node->key = strdup(key);
	valcpy = strdup(value);
	if (!node->key || !valcpy)
		return (free(node->key), free(node), 0);
	node->value = valcpy;
	node->next = ht->array[i];
	ht->array[i] = node;
	node->sprev = node->snext = NULL;

	if (!ht->shead)
		return (ht->shead = ht->stail = node, 1);
	tmp = ht->shead;
	while (tmp && strcmp(node->key, tmp->key) > 0)
		tmp = tmp->snext;
	if (tmp == ht->shead)
	{
		node->snext = tmp, tmp->sprev = node, ht->shead = node;
	}
	else if (!tmp)
	{
		node->sprev = ht->stail, ht->stail->snext = node, ht->stail = node;
	}
	else
	{
		node->snext = tmp, node->sprev = tmp->sprev;
		tmp->sprev->snext = node, tmp->sprev = node;
	}
	return (1);
}

/**
 * shash_table_get - Retrieves a value associated with a key
 * @ht: The sorted hash table
 * @key: The key to look for
 * Return: The value or NULL if not found
 */
char *shash_table_get(const shash_table_t *ht, const char *key)
{
	unsigned long int i;
	shash_node_t *node;

	if (!ht || !key || !*key)
		return (NULL);
	i = key_index((const unsigned char *)key, ht->size);
	for (node = ht->array[i]; node; node = node->next)
		if (strcmp(node->key, key) == 0)
			return (node->value);
	return (NULL);
}

/**
 * shash_table_print - Prints a sorted hash table in order
 * @ht: The sorted hash table
 */
void shash_table_print(const shash_table_t *ht)
{
	shash_node_t *n;
	int f = 0;

	if (!ht)
		return;
	printf("{");
	for (n = ht->shead; n; n = n->snext)
	{
		if (f)
			printf(", ");
		printf("'%s': '%s'", n->key, n->value), f = 1;
	}
	printf("}\n");
}

/**
 * shash_table_print_rev - Prints a sorted hash table in reverse order
 * @ht: The sorted hash table
 */
void shash_table_print_rev(const shash_table_t *ht)
{
	shash_node_t *n;
	int f = 0;

	if (!ht)
		return;
	printf("{");
	for (n = ht->stail; n; n = n->sprev)
	{
		if (f)
			printf(", ");
		printf("'%s': '%s'", n->key, n->value), f = 1;
	}
	printf("}\n");
}

/**
 * shash_table_delete - Frees a sorted hash table
 * @ht: The sorted hash table
 */
void shash_table_delete(shash_table_t *ht)
{
	shash_node_t *node, *tmp;

	if (!ht)
		return;
	for (node = ht->shead; node; node = tmp)
	{
		tmp = node->snext;
		free(node->key);
		free(node->value);
		free(node);
	}
	free(ht->array);
	free(ht);
}

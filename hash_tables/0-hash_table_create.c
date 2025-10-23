#include <stdlib.h>
#include "hash_tables.h"

/**
 * hash_table_create - Creates a hash table.
 * @size: The size of the array.
 *
 * Return: A pointer to the newly created hash table,
 *         or NULL if something went wrong.
 */
hash_table_t *hash_table_create(unsigned long int size)
{
    hash_table_t *table;
    unsigned long int i;

    /* Allocate memory for the hash table structure */
    table = malloc(sizeof(hash_table_t));
    if (table == NULL)
        return (NULL);

    /* Allocate memory for the array of pointers */
    table->array = malloc(sizeof(hash_node_t *) * size);
    if (table->array == NULL)
    {
        free(table);
        return (NULL);
    }

    /* Initialize each element of the array to NULL */
    for (i = 0; i < size; i++)
        table->array[i] = NULL;

    table->size = size;

    return (table);
}

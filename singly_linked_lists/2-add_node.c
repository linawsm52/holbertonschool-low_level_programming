#include "lists.h"
#include <stdlib.h>
#include <string.h>

/**
 * add_node - adds a new node at the beginning of a list_t list
 * @head: address of pointer to the head node
 * @str: string to duplicate into the new node
 *
 * Return: pointer to the new node, or NULL on failure
 */
list_t *add_node(list_t **head, const char *str)
{
	list_t *new_node;
	char *dup;
	unsigned int len = 0;

	if (head == NULL || str == NULL)
		return (NULL);

	new_node = malloc(sizeof(list_t));
	if (new_node == NULL)
		return (NULL);

	dup = strdup(str);
	if (dup == NULL)
	{
		free(new_node);
		return (NULL);
	}

	while (dup[len] != '\0')
		len++;

	new_node->str = dup;
	new_node->len = len;
	new_node->next = *head;
	*head = new_node;

	return (new_node);
}

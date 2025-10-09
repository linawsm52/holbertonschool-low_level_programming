#include "lists.h"
#include <stdlib.h>

/**
 * insert_dnodeint_at_index - inserts a new node at a given position
 * @h: address of pointer to head of the list
 * @idx: index where the new node should be added (starts at 0)
 * @n: value to store in the new node
 *
 * Return: address of the new node, or NULL if it failed / index is out of range
 */
dlistint_t *insert_dnodeint_at_index(dlistint_t **h, unsigned int idx, int n)
{
	dlistint_t *new_node, *cur;
	unsigned int i;

	if (h == NULL)
		return (NULL);

	/* Insert at the beginning */
	if (idx == 0)
		return (add_dnodeint(h, n));

	/* Traverse to the node just before the desired index */
	cur = *h;
	for (i = 0; cur != NULL && i < idx - 1; i++)
		cur = cur->next;

	/* If we reached NULL, index is out of range */
	if (cur == NULL)
		return (NULL);

	/* Insert at the end */
	if (cur->next == NULL)
		return (add_dnodeint_end(h, n));

	/* Insert in the middle */
	new_node = malloc(sizeof(dlistint_t));
	if (new_node == NULL)
		return (NULL);

	new_node->n = n;
	new_node->prev = cur;
	new_node->next = cur->next;

	cur->next->prev = new_node;
	cur->next = new_node;

	return (new_node);
}

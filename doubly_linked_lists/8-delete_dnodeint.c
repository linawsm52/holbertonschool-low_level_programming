#include "lists.h"
#include <stdlib.h>

/**
 * delete_dnodeint_at_index - deletes the node at a given index
 * @head: address of pointer to the head of the list
 * @index: index of the node that should be deleted (starts at 0)
 *
 * Return: 1 if success, -1 if failure
 */
int delete_dnodeint_at_index(dlistint_t **head, unsigned int index)
{
	dlistint_t *cur;
	unsigned int i;

	if (head == NULL || *head == NULL)
		return (-1);

	cur = *head;

	/* Delete first node */
	if (index == 0)
	{
		*head = cur->next;
		if (cur->next != NULL)
			cur->next->prev = NULL;
		free(cur);
		return (1);
	}

	/* Traverse to the node before the one to delete */
	for (i = 0; cur != NULL && i < index; i++)
		cur = cur->next;

	/* If node not found */
	if (cur == NULL)
		return (-1);

	/* Re-link and delete */
	if (cur->next != NULL)
		cur->next->prev = cur->prev;
	if (cur->prev != NULL)
		cur->prev->next = cur->next;

	free(cur);
	return (1);
}

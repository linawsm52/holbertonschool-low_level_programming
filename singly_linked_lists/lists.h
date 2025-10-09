#ifndef LISTS_H
#define LISTS_H

#include <stddef.h> /* for size_t */

/**
 * struct list_s - singly linked list node
 * @str: string (malloc'ed)
 * @len: length of the string
 * @next: pointer to the next node
 *
 * Description: singly linked list node structure
 */
typedef struct list_s
{
	char *str;
	unsigned int len;
	struct list_s *next;
} list_t;

/* Prototypes used across the project (لا بأس تبقينها أو تحذفين غير اللازمة) */
size_t list_len(const list_t *h);
list_t *add_node(list_t **head, const char *str);

#endif /* LISTS_H */

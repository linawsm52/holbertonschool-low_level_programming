#ifndef LISTS_H
#define LISTS_H

#include <stddef.h>

/**
 * struct list_s - singly linked list
 * @str: string (malloc'ed)
 * @len: length of the string
 * @next: points to the next node
 *
 * Description: singly linked list node structure
 */
typedef struct list_s
{
    char *str;
    unsigned int len;
    struct list_s *next;
} list_t;

size_t print_list(const list_t *h);
#ifndef LISTS_H
#define LISTS_H

#include <stddef.h>  /* for size_t */

/**
 * struct list_s - singly linked list
 * @str: string (malloc'ed)
 * @len: length of the string
 * @next: pointer to the next node
 */
typedef struct list_s
{
	char *str;
	unsigned int len;
	struct list_s *next;
} list_t;

/* Prototype required by the task */
size_t list_len(const list_t *h);

#endif /* LISTS_H */


#endif /* LISTS_H */

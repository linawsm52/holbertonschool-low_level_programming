# 0x12. C - Singly linked lists

## Description
This project is part of the Holberton School low-level programming curriculum.  
It focuses on the implementation and manipulation of **singly linked lists** in C.  
We learn how to create, traverse, and free linked lists.

---

## Learning Objectives
At the end of this project, you should be able to explain, without the help of Google:
- When and why using linked lists vs arrays
- How to build and use linked lists in C
- How to manage memory when creating and freeing nodes
- The structure of a `list_t` node

---

## Data Structure
The following structure is used for this project, defined in `lists.h`:

```c
/**
 * struct list_s - singly linked list
 * @str: string - (malloc'ed string)
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

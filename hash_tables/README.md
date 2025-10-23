# 0x1A. C - Hash tables

##  Description
This project is part of the Holberton School low-level programming track.  
It focuses on implementing and understanding **hash tables** — a data structure that stores key-value pairs and provides very fast lookups.

In this task, we are asked to:
- Write a function that creates a hash table.
- Return a pointer to the newly created hash table.
- Return `NULL` if something went wrong.

---

##  Files
| File | Description |
|------|--------------|
| `0-hash_table_create.c` | Contains the function that creates a new hash table. |
| `hash_tables.h` | Header file with all data structures and function prototypes. |
| `README.md` | Project documentation. |
| `0-main.c` | Test file provided by Holberton to verify functionality. |

---

## Prototype
```c
hash_table_t *hash_table_create(unsigned long int size);

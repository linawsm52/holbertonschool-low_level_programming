# 0x0B. C - malloc, free

Small utilities to practice dynamic memory allocation in C using `malloc` and `free`.

## Requirements

- Ubuntu 20.04 LTS
- `gcc` with flags: `-Wall -Wextra -Werror -pedantic -std=gnu89`
- Code follows **Betty** style
- No global variables

## Files

| File | Prototype | Description |
|------|-----------|-------------|
| `0-create_array.c` | `char *create_array(unsigned int size, char c);` | Allocates an array of `size` chars and initializes it with the char `c`. Returns `NULL` if `size` is 0 or allocation fails. |

> Add more rows here as you implement further tasks (e.g., `_strdup`, `str_concat`, `alloc_grid`, `free_grid`, …).

## Build & Run

Example with the first task:

```bash
gcc -Wall -Wextra -Werror -pedantic -std=gnu89 0-main.c 0-create_array.c -o create_array
./create_array

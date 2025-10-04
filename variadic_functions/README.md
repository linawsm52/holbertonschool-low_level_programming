# Variadic functions

This project is about learning and practicing variadic functions in C, using macros from `<stdarg.h>`.

## Learning Objectives
- What variadic functions are
- How to use `va_start`, `va_arg`, and `va_end` macros
- Why and how to use the `const` type qualifier

## Files

- **0-sum_them_all.c**  
  Function that returns the sum of all its parameters.  
  Prototype: `int sum_them_all(const unsigned int n, ...);`

- **1-print_numbers.c**  
  Function that prints numbers, followed by a new line.  
  Prototype: `void print_numbers(const char *separator, const unsigned int n, ...);`

- **2-print_strings.c**  
  Function that prints strings, followed by a new line.  
  Prototype: `void print_strings(const char *separator, const unsigned int n, ...);`

- **3-print_all.c**  
  Function that prints anything (char, int, float, string).  
  Prototype: `void print_all(const char * const format, ...);`

## Compilation
All files are compiled with:
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o <output>

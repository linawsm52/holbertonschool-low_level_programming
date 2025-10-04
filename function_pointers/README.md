# Function pointers

This project is about learning and practicing function pointers in C.

## Learning Objectives
- What are function pointers and how to use them
- What does a function pointer exactly hold
- Where does a function pointer point to in the virtual memory

## Files

- **0-print_name.c**  
  Function that prints a name using a given function pointer.

- **1-array_iterator.c**  
  Function that executes a function given as a parameter on each element of an array.

- **2-int_index.c**  
  Function that searches for an integer using a function pointer.

- **3-calc.h, 3-op_functions.c, 3-get_op_func.c, 3-main.c**  
  Program that performs simple operations (+, -, *, /, %).

## Compilation
All files are compiled with:
```bash
gcc -Wall -pedantic -Werror -Wextra -std=gnu89 *.c -o <output>

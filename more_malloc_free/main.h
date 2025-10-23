#ifndef MAIN_H
#define MAIN_H

int _putchar(char c);

/* 0. malloc_checked */
void *malloc_checked(unsigned int b);

/* 1. string_nconcat */
char *string_nconcat(char *s1, char *s2, unsigned int n);

/* 2. _calloc */
void *_calloc(unsigned int nmemb, unsigned int size);

/* 3. array_range */
int *array_range(int min, int max);

/* 100. _realloc */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void *_calloc(unsigned int nmemb, unsigned int size);
int *array_range(int min, int max);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

#endif /* MAIN_H */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * main - generate a valid key for 101-crackme
 *
 * Return: 0 on success
 */
int main(void)
{
	int sum = 0, ch;

	srand(time(NULL));

	/* print characters until the sum reaches 2772 */
	while (sum < 2772)
	{
		/* pick a printable ASCII char (32..126) */
		ch = (rand() % 95) + 32;

		/* if adding ch would exceed 2772, set ch to the remaining difference */
		if (sum + ch > 2772)
			ch = 2772 - sum;

		/* ch will never be 0 here, because sum < 2772 and ch >= 1 */
		putchar(ch);
		sum += ch;
	}
	/* do not print newline; do not print NUL */
	return (0);
}

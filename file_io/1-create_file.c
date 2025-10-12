#include "main.h"

/**
 * create_file - create a file and write text into it
 * @filename: name of the file to create
 * @text_content: NULL-terminated string to write inside the file
 *
 * Description:
 * - File permissions must be rw------- (0600).
 * - If the file exists, it is truncated.
 * - If filename is NULL, return -1.
 * - If text_content is NULL, create an empty file.
 *
 * Return: 1 on success, -1 on failure.
 */
int create_file(const char *filename, char *text_content)
{
	int fd;
	ssize_t wrote, total = 0;
	ssize_t len = 0;

	if (filename == NULL)
		return (-1);

	/* open: create/truncate, write-only, mode 0600 (rw-------) */
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
		return (-1);

	if (text_content != NULL)
	{
		/* compute length without strlen to stay within project limits */
		while (text_content[len] != '\0')
			len++;

		/* handle partial writes safely */
		while (total < len)
		{
			wrote = write(fd, text_content + total, len - total);
			if (wrote == -1)
			{
				close(fd);
				return (-1);
			}
			total += wrote;
		}
	}

	if (close(fd) == -1)
		return (-1);

	return (1);
}

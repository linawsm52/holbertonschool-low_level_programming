#include "main.h"

/**
 * append_text_to_file - appends text at the end of a file
 * @filename: name of the file to append to
 * @text_content: NULL-terminated string to add at the end of the file
 *
 * Description:
 * - If filename is NULL, return -1.
 * - If text_content is NULL, do not add anything to the file.
 * - Do not create the file if it does not exist.
 *
 * Return: 1 on success, -1 on failure.
 */
int append_text_to_file(const char *filename, char *text_content)
{
	int fd;
	ssize_t wrote, total = 0;
	ssize_t len = 0;

	if (filename == NULL)
		return (-1);

	fd = open(filename, O_WRONLY | O_APPEND);
	if (fd == -1)
		return (-1);

	if (text_content != NULL)
	{
		/* manually calculate string length */
		while (text_content[len] != '\0')
			len++;

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

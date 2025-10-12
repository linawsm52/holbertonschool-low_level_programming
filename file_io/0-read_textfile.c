#include "main.h"

/**
 * read_and_print - helper to read and print from fd
 * @fd: file descriptor
 * @letters: number of bytes to read
 *
 * Return: number of bytes printed, or 0 on failure
 */
static ssize_t read_and_print(int fd, size_t letters)
{
	char *buf;
	ssize_t nread, nw, total = 0;

	buf = malloc(letters);
	if (buf == NULL)
		return (0);

	nread = read(fd, buf, letters);
	if (nread <= 0)
	{
		free(buf);
		return (0);
	}

	while (total < nread)
	{
		nw = write(STDOUT_FILENO, buf + total, nread - total);
		if (nw <= 0)
		{
			free(buf);
			return (0);
		}
		total += nw;
	}
	free(buf);
	return (total);
}

/**
 * read_textfile - reads a text file and prints to STDOUT
 * @filename: path to the file
 * @letters: number of bytes to read and print
 *
 * Return: number of bytes printed, or 0 on failure
 */
ssize_t read_textfile(const char *filename, size_t letters)
{
	int fd;
	ssize_t total;

	if (filename == NULL || letters == 0)
		return (0);

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (0);

	total = read_and_print(fd, letters);

	if (close(fd) == -1)
		return (0);

	return (total);
}

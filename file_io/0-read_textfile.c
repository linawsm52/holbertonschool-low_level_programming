#include "main.h"

/**
 * read_textfile - reads a text file and prints it to POSIX standard output
 * @filename: path to the file to read
 * @letters: number of bytes to read and print
 *
 * Return: the actual number of bytes printed,
 *         or 0 on any error (open/read/write failure, NULL filename).
 */
ssize_t read_textfile(const char *filename, size_t letters)
{
	int fd;
	ssize_t nread, nwritten, total = 0;
	char *buf;

	if (filename == NULL || letters == 0)
		return (0);

	buf = malloc(letters);
	if (buf == NULL)
		return (0);

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		free(buf);
		return (0);
	}

	nread = read(fd, buf, letters);
	if (nread <= 0) /* read error or empty file */
	{
		free(buf);
		close(fd);
		return (0);
	}

	/* write may write fewer bytes; loop until all nread bytes are written */
	while (total < nread)
	{
		nwritten = write(STDOUT_FILENO, buf + total, nread - total);
		if (nwritten <= 0) /* write error */
		{
			free(buf);
			close(fd);
			return (0);
		}
		total += nwritten;
	}

	free(buf);
	if (close(fd) == -1)
		return (0);

	return (total);
}

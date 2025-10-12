#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

/**
 * error_exit - print message to STDERR and exit with code
 * @code: exit status
 * @fmt: printf-like format string
 * @arg: string argument used in the message (may be "")
 */
static void error_exit(int code, const char *fmt, const char *arg)
{
	dprintf(STDERR_FILENO, fmt, arg);
	exit(code);
}

/**
 * main - copy the content of a file to another file
 * @ac: argument count
 * @av: argument vector (av[1] = file_from, av[2] = file_to)
 *
 * Return: 0 on success, otherwise exits with codes:
 * 97 (usage), 98 (read error), 99 (write/create error), 100 (close error)
 */
int main(int ac, char **av)
{
	int fd_from, fd_to;
	ssize_t r, w, done;
	char buf[BUFFER_SIZE];

	if (ac != 3)
		error_exit(97, "Usage: cp file_from file_to\n", "");

	fd_from = open(av[1], O_RDONLY);
	if (fd_from == -1)
		error_exit(98, "Error: Can't read from file %s\n", av[1]);

	fd_to = open(av[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_to == -1)
	{
		close(fd_from);
		error_exit(99, "Error: Can't write to %s\n", av[2]);
	}

	/* Always check read result immediately before any write */
	for (;;)
	{
		r = read(fd_from, buf, BUFFER_SIZE);
		if (r == -1)
		{
			close(fd_from);
			close(fd_to);
			error_exit(98, "Error: Can't read from file %s\n", av[1]);
		}
		if (r == 0) /* EOF */
			break;

		done = 0;
		while (done < r)
		{
			w = write(fd_to, buf + done, r - done);
			if (w == -1)
			{
				close(fd_from);
				close(fd_to);
				error_exit(99, "Error: Can't write to %s\n", av[2]);
			}
			done += w;
		}
	}

	if (close(fd_from) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_from);
		exit(100);
	}
	if (close(fd_to) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd_to);
		exit(100);
	}

	return (0);
}

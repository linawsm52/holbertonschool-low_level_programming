#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

/**
 * error_exit - print error message to STDERR and exit with given code
 * @code: exit status
 * @message: error message format
 * @arg: string argument for message
 */
void error_exit(int code, const char *message, const char *arg)
{
	dprintf(STDERR_FILENO, message, arg);
	exit(code);
}

/**
 * main - copies the content of a file to another file
 * @ac: argument count
 * @av: argument vector (av[1] = file_from, av[2] = file_to)
 *
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	int fd_from, fd_to, cl;
	ssize_t r, w;
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

	while ((r = read(fd_from, buf, BUFFER_SIZE)) > 0)
	{
		w = write(fd_to, buf, r);
		if (w == -1 || w != r)
		{
			close(fd_from);
			close(fd_to);
			error_exit(99, "Error: Can't write to %s\n", av[2]);
		}
	}

	if (r == -1)
	{
		close(fd_from);
		close(fd_to);
		error_exit(98, "Error: Can't read from file %s\n", av[1]);
	}

	cl = close(fd_from);
	if (cl == -1)
		error_exit(100, "Error: Can't close fd %d\n", av[1]);

	cl = close(fd_to);
	if (cl == -1)
		error_exit(100, "Error: Can't close fd %d\n", av[2]);

	return (0);
}

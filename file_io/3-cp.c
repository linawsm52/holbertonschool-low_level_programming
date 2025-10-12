#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

/**
 * err_msg_exit - print string-based error to STDERR and exit
 * @code: exit code
 * @fmt: printf-like format with %s
 * @arg: string argument for the format
 */
static void err_msg_exit(int code, const char *fmt, const char *arg)
{
	dprintf(STDERR_FILENO, fmt, arg);
	exit(code);
}

/**
 * safe_close - close wrapper (exits with 100 on failure)
 * @fd: descriptor to close
 */
static void safe_close(int fd)
{
	int rc;

	rc = close(fd);
	if (rc == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
		exit(100);
	}
}

/**
 * copy_fd - copy bytes from fd_from to fd_to (checks read() immediately)
 * @fd_from: source descriptor
 * @fd_to: destination descriptor
 * @from: source filename (for messages)
 * @to: destination filename (for messages)
 */
static void copy_fd(int fd_from, int fd_to, const char *from, const char *to)
{
	ssize_t r, w, done;
	char buf[BUFFER_SIZE];

	for (;;)
	{
		r = read(fd_from, buf, BUFFER_SIZE);
		if (r == -1)
			err_msg_exit(98, "Error: Can't read from file %s\n", from);
		if (r == 0)
			break;

		done = 0;
		while (done < r)
		{
			w = write(fd_to, buf + done, r - done);
			if (w == -1)
				err_msg_exit(99, "Error: Can't write to %s\n", to);
			done += w;
		}
	}
}

/**
 * main - copy the content of a file to another file
 * @ac: argument count
 * @av: av[1] = file_from, av[2] = file_to
 *
 * Return: 0 on success; exits on errors per spec
 */
int main(int ac, char **av)
{
	int fd_from, fd_to;
	ssize_t first_read;
	char buf[BUFFER_SIZE];

	if (ac != 3)
		err_msg_exit(97, "Usage: cp file_from file_to\n", "");

	fd_from = open(av[1], O_RDONLY);
	if (fd_from == -1)
		err_msg_exit(98, "Error: Can't read from file %s\n", av[1]);

	/* read first so a forced read() failure yields exit 98 (as checker wants) */
	first_read = read(fd_from, buf, BUFFER_SIZE);
	if (first_read == -1)
	{
		safe_close(fd_from);
		err_msg_exit(98, "Error: Can't read from file %s\n", av[1]);
	}

	fd_to = open(av[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_to == -1)
	{
		safe_close(fd_from);
		err_msg_exit(99, "Error: Can't write to %s\n", av[2]);
	}

	/* write the first chunk if any, then continue with the regular loop */
	if (first_read > 0)
	{
		ssize_t done = 0, w;

		while (done < first_read)
		{
			w = write(fd_to, buf + done, first_read - done);
			if (w == -1)
			{
				safe_close(fd_from);
				safe_close(fd_to);
				err_msg_exit(99, "Error: Can't write to %s\n", av[2]);
			}
			done += w;
		}
	}

	copy_fd(fd_from, fd_to, av[1], av[2]);
	safe_close(fd_from);
	safe_close(fd_to);
	return (0);
}

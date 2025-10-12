#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

/**
 * die_usage - print usage and exit(97)
 */
static void die_usage(void)
{
	dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
	exit(97);
}

/**
 * die_read - print read error for a file and exit(98)
 * @file: source file name
 */
static void die_read(const char *file)
{
	dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", file);
	exit(98);
}

/**
 * die_write - print write error for a file and exit(99)
 * @file: destination file name
 */
static void die_write(const char *file)
{
	dprintf(STDERR_FILENO, "Error: Can't write to %s\n", file);
	exit(99);
}

/**
 * die_close - print close error for a file descriptor and exit(100)
 * @fd: file descriptor
 */
static void die_close(int fd)
{
	dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
	exit(100);
}

/**
 * safe_close - close wrapper that exits on failure
 * @fd: file descriptor to close
 */
static void safe_close(int fd)
{
	if (close(fd) == -1)
		die_close(fd);
}

/**
 * copy_fd - copy bytes from fd_from to fd_to using a buffer
 * @fd_from: source descriptor
 * @fd_to: destination descriptor
 * @from: source file name (for messages)
 * @to: destination file name (for messages)
 */
static void copy_fd(int fd_from, int fd_to,
		    const char *from, const char *to)
{
	ssize_t r, w, done;
	char buf[BUFFER_SIZE];

	for (;;)
	{
		/* check read result immediately */
		r = read(fd_from, buf, BUFFER_SIZE);
		if (r == -1)
			die_read(from);
		if (r == 0) /* EOF */
			break;

		done = 0;
		while (done < r)
		{
			w = write(fd_to, buf + done, r - done);
			if (w == -1)
				die_write(to);
			done += w;
		}
	}
}

/**
 * main - copies the content of a file to another file
 * @ac: argument count
 * @av: argument vector: av[1] = file_from, av[2] = file_to
 *
 * Return: 0 on success
 */
int main(int ac, char **av)
{
	int fd_from, fd_to;
	ssize_t first_read;
	char buf[BUFFER_SIZE];

	if (ac != 3)
		die_usage();

	fd_from = open(av[1], O_RDONLY);
	if (fd_from == -1)
		die_read(av[1]);

	/* ----- read FIRST, so read-error wins over any write error ----- */
	first_read = read(fd_from, buf, BUFFER_SIZE);
	if (first_read == -1)
	{
		safe_close(fd_from);
		die_read(av[1]);
	}

	/* now open destination */
	fd_to = open(av[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_to == -1)
	{
		safe_close(fd_from);
		die_write(av[2]);
	}

	/* write the first chunk (if any) then continue with copy loop */
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
				die_write(av[2]);
			}
			done += w;
		}
	}
	/* continue copying; copy_fd re-checks read() each time */
	copy_fd(fd_from, fd_to, av[1], av[2]);

	safe_close(fd_from);
	safe_close(fd_to);
	return (0);
}

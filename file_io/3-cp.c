#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

/* ---------- error helpers ---------- */
static void die_usage(void)
{
	dprintf(STDERR_FILENO, "Usage: cp file_from file_to\n");
	exit(97);
}

static void die_read(const char *file)
{
	dprintf(STDERR_FILENO, "Error: Can't read from file %s\n", file);
	exit(98);
}

static void die_write(const char *file)
{
	dprintf(STDERR_FILENO, "Error: Can't write to %s\n", file);
	exit(99);
}

static void die_close(int fd)
{
	dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
	exit(100);
}

/* close wrapper that exits on failure (keeps main short & clean) */
static void safe_close(int fd)
{
	if (close(fd) == -1)
		die_close(fd);
}

/* ---------- copying logic (kept < 40 lines) ---------- */
static void copy_fd(int fd_from, int fd_to, const char *from, const char *to)
{
	ssize_t r, w, done;
	char buf[BUFFER_SIZE];

	for (;;)
	{
		/* check read() result immediately */
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

/* ---------- main (now short) ---------- */
int main(int ac, char **av)
{
	int fd_from, fd_to;

	if (ac != 3)
		die_usage();

	fd_from = open(av[1], O_RDONLY);
	if (fd_from == -1)
		die_read(av[1]);

	fd_to = open(av[2], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (fd_to == -1)
	{
		safe_close(fd_from);
		die_write(av[2]);
	}

	copy_fd(fd_from, fd_to, av[1], av[2]);

	safe_close(fd_from);
	safe_close(fd_to);
	return (0);
}

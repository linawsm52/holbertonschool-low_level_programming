/*
 * File: 100-elf_header.c
 * Author: Lina
 * Description: Displays information from the ELF header (Holberton project)
 */

#include <elf.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/* ---------- helpers ---------- */

/**
 * die - print error message to stderr then exit 98
 * @s: message
 */
static void die(char *s)
{
	while (*s)
		write(STDERR_FILENO, s++, 1);
	write(STDERR_FILENO, "\n", 1);
	_exit(98);
}

/**
 * wdec - print unsigned int in decimal
 * @n: number
 */
static void wdec(unsigned int n)
{
	char buf[12];
	int i = 0;

	if (n == 0)
	{
		write(STDOUT_FILENO, "0", 1);
		return;
	}
	while (n)
	{
		buf[i++] = '0' + (n % 10);
		n /= 10;
	}
	while (i--)
		write(STDOUT_FILENO, &buf[i], 1);
}

/**
 * whex - print unsigned long in lowercase hexadecimal
 * @n: number
 */
static void whex(unsigned long n)
{
	char map[] = "0123456789abcdef";
	char buf[32];
	int i = 0;

	if (n == 0)
	{
		write(STDOUT_FILENO, "0", 1);
		return;
	}
	while (n)
	{
		buf[i++] = map[n & 0xF];
		n >>= 4;
	}
	while (i--)
		write(STDOUT_FILENO, &buf[i], 1);
}

/**
 * print_header - read & print ELF header fields
 * @fd: file descriptor
 */
static void print_header(int fd)
{
	unsigned char id[EI_NIDENT];
	ssize_t n;

	n = read(fd, id, EI_NIDENT);
	if (n != EI_NIDENT)
		die("Error: Can't read ELF header");
	if (!(id[0] == 0x7f && id[1] == 'E' && id[2] == 'L' && id[3] == 'F'))
		die("Error: Not an ELF file");

	write(STDOUT_FILENO, "ELF Header:\n", 12);

	/* Magic */
	{
		int i;
		write(STDOUT_FILENO, "  Magic:   ", 11);
		for (i = 0; i < EI_NIDENT; i++)
		{
			char hi = "0123456789abcdef"[id[i] >> 4];
			char lo = "0123456789abcdef"[id[i] & 0xF];
			write(STDOUT_FILENO, &hi, 1);
			write(STDOUT_FILENO, &lo, 1);
			if (i != EI_NIDENT - 1)
				write(STDOUT_FILENO, " ", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
	}

	/* Class */
	write(STDOUT_FILENO, "  Class:                             ", 36);
	if (id[EI_CLASS] == ELFCLASS32)
		write(STDOUT_FILENO, "ELF32\n", 6);
	else if (id[EI_CLASS] == ELFCLASS64)
		write(STDOUT_FILENO, "ELF64\n", 6);
	else
		write(STDOUT_FILENO, "Invalid class\n", 14);

	/* Data */
	write(STDOUT_FILENO, "  Data:                              ", 36);
	if (id[EI_DATA] == ELFDATA2LSB)
		write(STDOUT_FILENO, "2's complement, little endian\n", 30);
	else if (id[EI_DATA] == ELFDATA2MSB)
		write(STDOUT_FILENO, "2's complement, big endian\n", 27);
	else
		write(STDOUT_FILENO, "Invalid data encoding\n", 22);

	/* Version */
	write(STDOUT_FILENO, "  Version:                           ", 36);
	wdec(id[EI_VERSION]);
	if (id[EI_VERSION] == EV_CURRENT)
		write(STDOUT_FILENO, " (current)", 10);
	write(STDOUT_FILENO, "\n", 1);

	/* OS/ABI */
	write(STDOUT_FILENO, "  OS/ABI:                            ", 36);
	switch (id[EI_OSABI])
	{
	case ELFOSABI_SYSV:      write(STDOUT_FILENO, "UNIX - System V\n", 16); break;
	case ELFOSABI_NETBSD:    write(STDOUT_FILENO, "UNIX - NetBSD\n", 14); break;
	case ELFOSABI_SOLARIS:   write(STDOUT_FILENO, "UNIX - Solaris\n", 15); break;
	case ELFOSABI_GNU:       write(STDOUT_FILENO, "UNIX - GNU\n", 11); break;
	case ELFOSABI_FREEBSD:   write(STDOUT_FILENO, "UNIX - FreeBSD\n", 15); break;
	case ELFOSABI_OPENBSD:   write(STDOUT_FILENO, "UNIX - OpenBSD\n", 15); break;
	case 103:                write(STDOUT_FILENO, "UNIX - Sortix\n", 14); break;
	default:
		write(STDOUT_FILENO, "<unknown: ", 10);
		wdec(id[EI_OSABI]);
		write(STDOUT_FILENO, ">\n", 2);
		break;
	}

	/* ABI Version */
	write(STDOUT_FILENO, "  ABI Version:                       ", 36);
	wdec(id[EI_ABIVERSION]);
	write(STDOUT_FILENO, "\n", 1);

	/* short header read again */
	if (id[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr h;
		unsigned short t;
		unsigned int ent;

		lseek(fd, 0, SEEK_SET);
		if (read(fd, &h, sizeof(h)) != (ssize_t)sizeof(h))
			die("Error: Can't read full header");

		t = h.e_type;
		ent = h.e_entry;

		if (id[EI_DATA] == ELFDATA2MSB)
		{
			t = (t >> 8) | (t << 8);
			ent = ((ent & 0xFF) << 24) | ((ent & 0xFF00) << 8) |
			      ((ent & 0xFF0000) >> 8) | ((ent & 0xFF000000) >> 24);
		}

		write(STDOUT_FILENO, "  Type:                              ", 36);
		switch (t)
		{
		case ET_NONE: write(STDOUT_FILENO, "NONE (None)\n", 12); break;
		case ET_REL:  write(STDOUT_FILENO, "REL (Relocatable file)\n", 23); break;
		case ET_EXEC: write(STDOUT_FILENO, "EXEC (Executable file)\n", 23); break;
		case ET_DYN:  write(STDOUT_FILENO, "DYN (Shared object file)\n", 25); break;
		case ET_CORE: write(STDOUT_FILENO, "CORE (Core file)\n", 17); break;
		default: write(STDOUT_FILENO, "<unknown: 0x", 12); whex(t);
			write(STDOUT_FILENO, ">\n", 2); break;
		}

		write(STDOUT_FILENO,
		      "  Entry point address:               0x", 40);
		whex(ent);
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (id[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr h;
		unsigned short t;
		unsigned long ent;

		lseek(fd, 0, SEEK_SET);
		if (read(fd, &h, sizeof(h)) != (ssize_t)sizeof(h))
			die("Error: Can't read full header");

		t = h.e_type;
		ent = h.e_entry;

		if (id[EI_DATA] == ELFDATA2MSB)
		{
			t = (t >> 8) | (t << 8);
			ent = ((ent & 0xFFULL) << 56) |
			      ((ent & 0xFF00ULL) << 40) |
			      ((ent & 0xFF0000ULL) << 24) |
			      ((ent & 0xFF000000ULL) << 8) |
			      ((ent & 0xFF00000000ULL) >> 8) |
			      ((ent & 0xFF0000000000ULL) >> 24) |
			      ((ent & 0xFF000000000000ULL) >> 40) |
			      ((ent & 0xFF00000000000000ULL) >> 56);
		}

		write(STDOUT_FILENO, "  Type:                              ", 36);
		switch (t)
		{
		case ET_NONE: write(STDOUT_FILENO, "NONE (None)\n", 12); break;
		case ET_REL:  write(STDOUT_FILENO, "REL (Relocatable file)\n", 23); break;
		case ET_EXEC: write(STDOUT_FILENO, "EXEC (Executable file)\n", 23); break;
		case ET_DYN:  write(STDOUT_FILENO, "DYN (Shared object file)\n", 25); break;
		case ET_CORE: write(STDOUT_FILENO, "CORE (Core file)\n", 17); break;
		default: write(STDOUT_FILENO, "<unknown: 0x", 12); whex(t);
			write(STDOUT_FILENO, ">\n", 2); break;
		}

		write(STDOUT_FILENO,
		      "  Entry point address:               0x", 40);
		whex(ent);
		write(STDOUT_FILENO, "\n", 1);
	}
	else
		die("Error: Unknown ELF class");
}

/**
 * main - program entry
 * @ac: arg count
 * @av: arg vector
 * Return: 0
 */
int main(int ac, char **av)
{
	int fd;

	if (ac != 2)
		die("Usage: elf_header elf_filename");

	fd = open(av[1], O_RDONLY);
	if (fd == -1)
		die("Error: Can't open file");

	print_header(fd);

	if (close(fd) == -1)
		die("Error: Can't close file");
	return (0);
}

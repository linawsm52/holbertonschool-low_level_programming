/*
 * File: 100-elf_header.c
 * Author: Lina
 * Description: Display selected fields of an ELF header (like `readelf -h`).
 */

#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/* ---------- helpers without stdio ---------- */

/**
 * die - print error message to stderr and exit with code 98
 * @msg: message
 */
static void die(const char *msg)
{
	while (*msg)
		write(STDERR_FILENO, msg++, 1);
	write(STDERR_FILENO, "\n", 1);
	_exit(98);
}

/**
 * wdec - write an unsigned integer in decimal to fd 1
 * @v: value
 */
static void wdec(unsigned int v)
{
	char buf[12];
	int i = 0;

	if (v == 0)
	{
		write(STDOUT_FILENO, "0", 1);
		return;
	}
	while (v)
	{
		buf[i++] = (char)('0' + (v % 10));
		v /= 10;
	}
	while (i--)
		write(STDOUT_FILENO, &buf[i], 1);
}

/**
 * whex_u64 - write an unsigned long as lowercase hex to fd 1
 * @v: value
 */
static void whex_u64(unsigned long v)
{
	char map[] = "0123456789abcdef";
	char buf[16];
	int i = 0;

	if (v == 0)
	{
		write(STDOUT_FILENO, "0", 1);
		return;
	}
	while (v)
	{
		buf[i++] = map[v & 0xF];
		v >>= 4;
	}
	while (i--)
		write(STDOUT_FILENO, &buf[i], 1);
}

/**
 * print_header - read and print ELF header fields
 * @fd: file descriptor already opened O_RDONLY
 */
static void print_header(int fd)
{
	unsigned char ident[EI_NIDENT];
	ssize_t n;

	/* read e_ident first */
	n = read(fd, ident, EI_NIDENT);
	if (n != EI_NIDENT)
	{
		close(fd);
		die("Error: Can't read ELF header");
	}

	/* check magic */
	if (!(ident[0] == 0x7f && ident[1] == 'E' && ident[2] == 'L' && ident[3] == 'F'))
	{
		close(fd);
		die("Error: Not an ELF file");
	}

	/* common printing */
	write(STDOUT_FILENO, "ELF Header:\n", 12);

	/* Magic */
	{
		int i;
		write(STDOUT_FILENO, "  Magic:   ", 11);
		for (i = 0; i < EI_NIDENT; i++)
		{
			char hi = "0123456789abcdef"[ident[i] >> 4];
			char lo = "0123456789abcdef"[ident[i] & 0xF];
			write(STDOUT_FILENO, &hi, 1);
			write(STDOUT_FILENO, &lo, 1);
			if (i != EI_NIDENT - 1)
				write(STDOUT_FILENO, " ", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
	}

	/* Class */
	write(STDOUT_FILENO, "  Class:                             ", 36);
	if (ident[EI_CLASS] == ELFCLASS32)
		write(STDOUT_FILENO, "ELF32\n", 6);
	else if (ident[EI_CLASS] == ELFCLASS64)
		write(STDOUT_FILENO, "ELF64\n", 6);
	else
		write(STDOUT_FILENO, "Invalid class\n", 14);

	/* Data */
	write(STDOUT_FILENO, "  Data:                              ", 36);
	if (ident[EI_DATA] == ELFDATA2LSB)
		write(STDOUT_FILENO, "2's complement, little endian\n", 30);
	else if (ident[EI_DATA] == ELFDATA2MSB)
		write(STDOUT_FILENO, "2's complement, big endian\n", 27);
	else
		write(STDOUT_FILENO, "Invalid data encoding\n", 22);

	/* Version (print numeric value, add "(current)" only if equals EV_CURRENT) */
	write(STDOUT_FILENO, "  Version:                           ", 36);
	wdec((unsigned int)ident[EI_VERSION]);
	if (ident[EI_VERSION] == EV_CURRENT)
		write(STDOUT_FILENO, " (current)", 10);
	write(STDOUT_FILENO, "\n", 1);

	/* OS/ABI */
	write(STDOUT_FILENO, "  OS/ABI:                            ", 36);
	switch (ident[EI_OSABI])
	{
	case ELFOSABI_SYSV:      write(STDOUT_FILENO, "UNIX - System V\n", 16); break;
	case ELFOSABI_NETBSD:    write(STDOUT_FILENO, "UNIX - NetBSD\n", 14); break;
	case ELFOSABI_SOLARIS:   write(STDOUT_FILENO, "UNIX - Solaris\n", 15); break;
	case ELFOSABI_GNU:       write(STDOUT_FILENO, "UNIX - GNU\n", 11); break;
	case ELFOSABI_FREEBSD:   write(STDOUT_FILENO, "UNIX - FreeBSD\n", 15); break;
	case ELFOSABI_OPENBSD:   write(STDOUT_FILENO, "UNIX - OpenBSD\n", 15); break;
	case ELFOSABI_AIX:       write(STDOUT_FILENO, "UNIX - AIX\n", 11); break;
	case ELFOSABI_IRIX:      write(STDOUT_FILENO, "UNIX - IRIX\n", 12); break;
	case ELFOSABI_HPUX:      write(STDOUT_FILENO, "UNIX - HP-UX\n", 13); break;
	case ELFOSABI_TRU64:     write(STDOUT_FILENO, "UNIX - TRU64\n", 13); break;
	case ELFOSABI_ARM:       write(STDOUT_FILENO, "ARM\n", 4); break;
	case ELFOSABI_STANDALONE:write(STDOUT_FILENO, "Standalone App\n", 15); break;
	/* Sortix value (103) */
	case 103:                write(STDOUT_FILENO, "UNIX - Sortix\n", 14); break;
	default:
		write(STDOUT_FILENO, "<unknown: ", 10);
		wdec((unsigned int)ident[EI_OSABI]);
		write(STDOUT_FILENO, ">\n", 2);
		break;
	}

	/* ABI Version */
	write(STDOUT_FILENO, "  ABI Version:                       ", 36);
	wdec((unsigned int)ident[EI_ABIVERSION]);
	write(STDOUT_FILENO, "\n", 1);

	/* Now read full header based on class and respecting endianness */
	if (ident[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr h32;
		unsigned short type16;
		unsigned int entry32;

		if (lseek(fd, 0, SEEK_SET) == (off_t)-1 ||
		    read(fd, &h32, sizeof(h32)) != (ssize_t)sizeof(h32))
		{
			close(fd);
			die("Error: Can't read full ELF header");
		}

		type16 = h32.e_type;
		entry32 = h32.e_entry;

		if (ident[EI_DATA] == ELFDATA2MSB)
		{
#define BSWAP16(x) (unsigned short)(((x) >> 8) | ((x) << 8))
#define BSWAP32(x) (((x) << 24) | (((x) & 0x0000FF00U) << 8) | \
                   (((x) & 0x00FF0000U) >> 8) | ((x) >> 24))
			type16 = BSWAP16(type16);
			entry32 = BSWAP32(entry32);
		}

		/* Type */
		write(STDOUT_FILENO, "  Type:                              ", 36);
		switch (type16)
		{
		case ET_NONE: write(STDOUT_FILENO, "NONE (None)\n", 12); break;
		case ET_REL:  write(STDOUT_FILENO, "REL (Relocatable file)\n", 23); break;
		case ET_EXEC: write(STDOUT_FILENO, "EXEC (Executable file)\n", 23); break;
		case ET_DYN:  write(STDOUT_FILENO, "DYN (Shared object file)\n", 25); break;
		case ET_CORE: write(STDOUT_FILENO, "CORE (Core file)\n", 17); break;
		default:
			write(STDOUT_FILENO, "<unknown: 0x", 12);
			whex_u64((unsigned long)type16);
			write(STDOUT_FILENO, ">\n", 2);
			break;
		}

		/* Entry */
		write(STDOUT_FILENO,
		      "  Entry point address:               0x", 40);
		whex_u64((unsigned long)entry32);
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (ident[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr h64;
		unsigned short type16;
		unsigned long entry64;

		if (lseek(fd, 0, SEEK_SET) == (off_t)-1 ||
		    read(fd, &h64, sizeof(h64)) != (ssize_t)sizeof(h64))
		{
			close(fd);
			die("Error: Can't read full ELF header");
		}

		type16 = h64.e_type;
		entry64 = (unsigned long)h64.e_entry;

		if (ident[EI_DATA] == ELFDATA2MSB)
		{
#define BSWAP64(x) (((x) << 56) | (((x) & 0x000000000000FF00UL) << 40) | \
                   (((x) & 0x0000000000FF0000UL) << 24) | \
                   (((x) & 0x00000000FF000000UL) << 8)  | \
                   (((x) & 0x000000FF00000000UL) >> 8)  | \
                   (((x) & 0x0000FF0000000000UL) >> 24) | \
                   (((x) & 0x00FF000000000000UL) >> 40) | \
                   ((x) >> 56))
#define BSWAP16(x) (unsigned short)(((x) >> 8) | ((x) << 8))
			type16 = BSWAP16(type16);
			entry64 = BSWAP64(entry64);
		}

		/* Type */
		write(STDOUT_FILENO, "  Type:                              ", 36);
		switch (type16)
		{
		case ET_NONE: write(STDOUT_FILENO, "NONE (None)\n", 12); break;
		case ET_REL:  write(STDOUT_FILENO, "REL (Relocatable file)\n", 23); break;
		case ET_EXEC: write(STDOUT_FILENO, "EXEC (Executable file)\n", 23); break;
		case ET_DYN:  write(STDOUT_FILENO, "DYN (Shared object file)\n", 25); break;
		case ET_CORE: write(STDOUT_FILENO, "CORE (Core file)\n", 17); break;
		default:
			write(STDOUT_FILENO, "<unknown: 0x", 12);
			whex_u64((unsigned long)type16);
			write(STDOUT_FILENO, ">\n", 2);
			break;
		}

		/* Entry */
		write(STDOUT_FILENO,
		      "  Entry point address:               0x", 40);
		whex_u64(entry64);
		write(STDOUT_FILENO, "\n", 1);
	}
	else
	{
		close(fd);
		die("Error: Unknown ELF class");
	}
}

/**
 * main - entry point
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, exits 98 on error
 */
int main(int argc, char **argv)
{
	int fd;

	if (argc != 2)
		die("Usage: elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		die("Error: Can't open file");

	print_header(fd);

	if (close(fd) == -1)
		die("Error: Can't close file");

	return (0);
}

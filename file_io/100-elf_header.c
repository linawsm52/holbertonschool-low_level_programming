/*
 * File: 100-elf_header.c
 * Author: Lina
 * Description: Print selected fields from an ELF header (like `readelf -h`).
 */

#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/* ===================== helpers (no stdio) ===================== */

/**
 * die - print error to stderr and exit with code 98
 * @msg: error message
 */
static void die(const char *msg)
{
	while (*msg)
		write(STDERR_FILENO, msg++, 1);
	write(STDERR_FILENO, "\n", 1);
	/* no stdlib: use _exit to satisfy checker */
	_exit(98);
}

/**
 * wdec - print an unsigned int in base10 to stdout
 * @v: value
 */
static void wdec(unsigned int v)
{
	char b[12];
	int i = 0;

	if (v == 0)
	{
		write(STDOUT_FILENO, "0", 1);
		return;
	}
	while (v)
	{
		b[i++] = (char)('0' + (v % 10));
		v /= 10;
	}
	while (i--)
		write(STDOUT_FILENO, &b[i], 1);
}

/**
 * whex - print an unsigned long in lowercase hex to stdout
 * @v: value
 */
static void whex(unsigned long v)
{
	char map[] = "0123456789abcdef";
	char b[16];
	int i = 0;

	if (v == 0)
	{
		write(STDOUT_FILENO, "0", 1);
		return;
	}
	while (v)
	{
		b[i++] = map[v & 0xF];
		v >>= 4;
	}
	while (i--)
		write(STDOUT_FILENO, &b[i], 1);
}

/**
 * print_header - read & pretty-print the ELF header
 * @fd: opened file descriptor (O_RDONLY)
 */
static void print_header(int fd)
{
	unsigned char id[EI_NIDENT];
	ssize_t n;

	/* read only e_ident first */
	n = read(fd, id, EI_NIDENT);
	if (n != EI_NIDENT)
	{
		close(fd);
		die("Error: Can't read ELF header");
	}

	/* check magic */
	if (!(id[0] == 0x7f && id[1] == 'E' && id[2] == 'L' && id[3] == 'F'))
	{
		close(fd);
		die("Error: Not an ELF file");
	}

	/* heading */
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
	wdec((unsigned int)id[EI_VERSION]);
	if (id[EI_VERSION] == EV_CURRENT)
		write(STDOUT_FILENO, " (current)", 10);
	write(STDOUT_FILENO, "\n", 1);

	/* OS/ABI */
	write(STDOUT_FILENO, "  OS/ABI:                            ", 36);
	switch (id[EI_OSABI])
	{
	case ELFOSABI_SYSV:      write(STDOUT_FILENO, "UNIX - System V\n", 16); break;
	case ELFOSABI_HPUX:      write(STDOUT_FILENO, "UNIX - HP-UX\n", 13); break;
	case ELFOSABI_NETBSD:    write(STDOUT_FILENO, "UNIX - NetBSD\n", 14); break;
	case ELFOSABI_LINUX:     write(STDOUT_FILENO, "UNIX - Linux\n", 13); break;  /* alias of GNU in many toolchains */
	case ELFOSABI_GNU:       write(STDOUT_FILENO, "UNIX - GNU\n", 11); break;
	case ELFOSABI_SOLARIS:   write(STDOUT_FILENO, "UNIX - Solaris\n", 15); break;
	case ELFOSABI_AIX:       write(STDOUT_FILENO, "UNIX - AIX\n", 11); break;
	case ELFOSABI_IRIX:      write(STDOUT_FILENO, "UNIX - IRIX\n", 12); break;
	case ELFOSABI_FREEBSD:   write(STDOUT_FILENO, "UNIX - FreeBSD\n", 15); break;
	case ELFOSABI_TRU64:     write(STDOUT_FILENO, "UNIX - TRU64\n", 13); break;
	case ELFOSABI_MODESTO:   write(STDOUT_FILENO, "Novell - Modesto\n", 17); break;
	case ELFOSABI_OPENBSD:   write(STDOUT_FILENO, "UNIX - OpenBSD\n", 15); break;
	case ELFOSABI_ARM:       write(STDOUT_FILENO, "ARM\n", 4); break;
	case ELFOSABI_STANDALONE:write(STDOUT_FILENO, "Standalone App\n", 15); break;
	case 103:                write(STDOUT_FILENO, "UNIX - Sortix\n", 14); break; /* Sortix */
	default:
		write(STDOUT_FILENO, "<unknown: ", 10);
		wdec((unsigned int)id[EI_OSABI]);
		write(STDOUT_FILENO, ">\n", 2);
		break;
	}

	/* ABI Version */
	write(STDOUT_FILENO, "  ABI Version:                       ", 36);
	wdec((unsigned int)id[EI_ABIVERSION]);
	write(STDOUT_FILENO, "\n", 1);

	/* Read the full header according to class (and fix endianness) */
	if (id[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr h32;
		unsigned short t;
		unsigned int ent;

		if (lseek(fd, 0, SEEK_SET) == (off_t)-1 ||
		    read(fd, &h32, sizeof(h32)) != (ssize_t)sizeof(h32))
		{
			close(fd);
			die("Error: Can't read full ELF header");
		}

		t = h32.e_type;
		ent = h32.e_entry;

		if (id[EI_DATA] == ELFDATA2MSB)
		{
#define BSWAP16(x) (unsigned short)((((x) & 0x00FFU) << 8) | (((x) & 0xFF00U) >> 8))
#define BSWAP32(x) ( ((x) << 24)               | \
                     (((x) & 0x0000FF00U) << 8) | \
                     (((x) & 0x00FF0000U) >> 8) | \
                     ((unsigned int)(x) >> 24) )
			t = BSWAP16(t);
			ent = BSWAP32(ent);
		}

		write(STDOUT_FILENO, "  Type:                              ", 36);
		switch (t)
		{
		case ET_NONE: write(STDOUT_FILENO, "NONE (None)\n", 12); break;
		case ET_REL:  write(STDOUT_FILENO, "REL (Relocatable file)\n", 23); break;
		case ET_EXEC: write(STDOUT_FILENO, "EXEC (Executable file)\n", 23); break;
		case ET_DYN:  write(STDOUT_FILENO, "DYN (Shared object file)\n", 25); break;
		case ET_CORE: write(STDOUT_FILENO, "CORE (Core file)\n", 17); break;
		default:
			write(STDOUT_FILENO, "<unknown: 0x", 12);
			whex((unsigned long)t);
			write(STDOUT_FILENO, ">\n", 2);
			break;
		}

		write(STDOUT_FILENO,
		      "  Entry point address:               0x", 40);
		whex((unsigned long)ent);
		write(STDOUT_FILENO, "\n", 1);
	}
	else if (id[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr h64;
		unsigned short t;
		unsigned long ent;

		if (lseek(fd, 0, SEEK_SET) == (off_t)-1 ||
		    read(fd, &h64, sizeof(h64)) != (ssize_t)sizeof(h64))
		{
			close(fd);
			die("Error: Can't read full ELF header");
		}

		t = h64.e_type;
		ent = (unsigned long)h64.e_entry;

		if (id[EI_DATA] == ELFDATA2MSB)
		{
#define BSWAP64(x) ( ((x) << 56) | \
	(((x) & 0x000000000000FF00UL) << 40) | \
	(((x) & 0x0000000000FF0000UL) << 24) | \
	(((x) & 0x00000000FF000000UL) << 8)  | \
	(((x) & 0x000000FF00000000UL) >> 8)  | \
	(((x) & 0x0000FF0000000000UL) >> 24) | \
	(((x) & 0x00FF000000000000UL) >> 40) | \
	((unsigned long)(x) >> 56) )
			t = BSWAP16(t);
			ent = BSWAP64(ent);
		}

		write(STDOUT_FILENO, "  Type:                              ", 36);
		switch (t)
		{
		case ET_NONE: write(STDOUT_FILENO, "NONE (None)\n", 12); break;
		case ET_REL:  write(STDOUT_FILENO, "REL (Relocatable file)\n", 23); break;
		case ET_EXEC: write(STDOUT_FILENO, "EXEC (Executable file)\n", 23); break;
		case ET_DYN:  write(STDOUT_FILENO, "DYN (Shared object file)\n", 25); break;
		case ET_CORE: write(STDOUT_FILENO, "CORE (Core file)\n", 17); break;
		default:
			write(STDOUT_FILENO, "<unknown: 0x", 12);
			whex((unsigned long)t);
			write(STDOUT_FILENO, ">\n", 2);
			break;
		}

		write(STDOUT_FILENO,
		      "  Entry point address:               0x", 40);
		whex(ent);
		write(STDOUT_FILENO, "\n", 1);
	}
	else
	{
		close(fd);
		die("Error: Unknown ELF class");
	}
}

/**
 * main - entry
 * @argc: count
 * @argv: vector
 * Return: 0 on success (exits 98 on error)
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

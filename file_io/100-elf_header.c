/* 
 * File: 3-elf_header.c
 * Author: Lina & ChatGPT
 * Description: Print the information contained in the ELF header.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include <errno.h>
#include <string.h>

/* ---------- helpers ---------- */

static void die(const char *msg)
{
	dprintf(STDERR_FILENO, "Error: %s\n", msg);
	exit(98);
}

static void must_close(int fd)
{
	if (close(fd) == -1)
	{
		dprintf(STDERR_FILENO, "Error: Can't close fd %d\n", fd);
		exit(98);
	}
}

static void print_magic(const unsigned char *e)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", e[i]);
		if (i + 1 != EI_NIDENT)
			printf(" ");
	}
	printf("\n");
}

static void print_class(const unsigned char *e)
{
	printf("  Class:                             ");
	switch (e[EI_CLASS])
	{
	case ELFCLASS32: printf("ELF32\n"); break;
	case ELFCLASS64: printf("ELF64\n"); break;
	default:         printf("Invalid class\n"); break;
	}
}

static void print_data(const unsigned char *e)
{
	printf("  Data:                              ");
	switch (e[EI_DATA])
	{
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("Invalid data encoding\n");
		break;
	}
}

static void print_version(const unsigned char *e)
{
	printf("  Version:                           %d", e[EI_VERSION]);
	if (e[EI_VERSION] == EV_CURRENT)
		printf(" (current)");
	printf("\n");
}

static void print_osabi(const unsigned char *e)
{
	printf("  OS/ABI:                            ");
	switch (e[EI_OSABI])
	{
	case ELFOSABI_SYSV:     printf("UNIX - System V\n"); break;
	case ELFOSABI_HPUX:     printf("UNIX - HP-UX\n"); break;
	case ELFOSABI_NETBSD:   printf("UNIX - NetBSD\n"); break;
	case ELFOSABI_GNU:      printf("UNIX - GNU\n"); break;  /* aka Linux */
	case ELFOSABI_SOLARIS:  printf("UNIX - Solaris\n"); break;
	case ELFOSABI_AIX:      printf("UNIX - AIX\n"); break;
	case ELFOSABI_IRIX:     printf("UNIX - IRIX\n"); break;
	case ELFOSABI_FREEBSD:  printf("UNIX - FreeBSD\n"); break;
	case ELFOSABI_TRU64:    printf("UNIX - TRU64\n"); break;
	case ELFOSABI_MODESTO:  printf("Novell - Modesto\n"); break;
	case ELFOSABI_OPENBSD:  printf("UNIX - OpenBSD\n"); break;
	case ELFOSABI_ARM:      printf("ARM\n"); break;
	case ELFOSABI_STANDALONE: printf("Standalone App\n"); break;
	default:                printf("<unknown: %d>\n", e[EI_OSABI]); break;
	}
}

static void print_abiversion(const unsigned char *e)
{
	printf("  ABI Version:                       %d\n", e[EI_ABIVERSION]);
}

static void print_type(uint16_t type, int msb)
{
	if (msb) /* correct endianness if header is big-endian */
		type = (type >> 8) | (type << 8);

	printf("  Type:                              ");
	switch (type)
	{
	case ET_NONE: printf("NONE (None)\n"); break;
	case ET_REL:  printf("REL (Relocatable file)\n"); break;
	case ET_EXEC: printf("EXEC (Executable file)\n"); break;
	case ET_DYN:  printf("DYN (Shared object file)\n"); break;
	case ET_CORE: printf("CORE (Core file)\n"); break;
	default:      printf("<unknown: 0x%x>\n", type); break;
	}
}

static void print_entry32(uint32_t e, int msb)
{
	if (msb)
		e = ((e & 0x000000FFU) << 24) |
		    ((e & 0x0000FF00U) << 8)  |
		    ((e & 0x00FF0000U) >> 8)  |
		    ((e & 0xFF000000U) >> 24);
	printf("  Entry point address:               0x%x\n", e);
}

static void print_entry64(uint64_t e, int msb)
{
	if (msb)
		e = ((e & 0x00000000000000FFULL) << 56) |
		    ((e & 0x000000000000FF00ULL) << 40) |
		    ((e & 0x0000000000FF0000ULL) << 24) |
		    ((e & 0x00000000FF000000ULL) << 8)  |
		    ((e & 0x000000FF00000000ULL) >> 8)  |
		    ((e & 0x0000FF0000000000ULL) >> 24) |
		    ((e & 0x00FF000000000000ULL) >> 40) |
		    ((e & 0xFF00000000000000ULL) >> 56);

	printf("  Entry point address:               0x%lx\n", (unsigned long)e);
}

/* ---------- main ---------- */

int main(int argc, char **argv)
{
	int fd;
	unsigned char ident[EI_NIDENT];
	ssize_t n;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: %s elf_filename\n", argv[0]);
		return (98);
	}

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		die("Can't open file");

	/* read e_ident first to validate ELF and know class/data */
	n = read(fd, ident, EI_NIDENT);
	if (n != EI_NIDENT)
	{
		must_close(fd);
		die("Can't read ELF header");
	}

	if (!(ident[0] == 0x7f && ident[1] == 'E' && ident[2] == 'L' && ident[3] == 'F'))
	{
		must_close(fd);
		die("Not an ELF file");
	}

	printf("ELF Header:\n");
	print_magic(ident);
	print_class(ident);
	print_data(ident);
	print_version(ident);
	print_osabi(ident);
	print_abiversion(ident);

	/* read the rest of the header based on class */
	if (ident[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr h32;

		/* we already consumed EI_NIDENT; read remaining bytes of header */
		if (lseek(fd, 0, SEEK_SET) == (off_t)-1)
		{
			must_close(fd);
			die("Can't lseek");
		}
		n = read(fd, &h32, sizeof(h32));
		if (n != (ssize_t)sizeof(h32))
		{
			must_close(fd);
			die("Can't read full ELF header");
		}
		print_type(h32.e_type, ident[EI_DATA] == ELFDATA2MSB);
		print_entry32(h32.e_entry, ident[EI_DATA] == ELFDATA2MSB);
	}
	else if (ident[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr h64;

		if (lseek(fd, 0, SEEK_SET) == (off_t)-1)
		{
			must_close(fd);
			die("Can't lseek");
		}
		n = read(fd, &h64, sizeof(h64));
		if (n != (ssize_t)sizeof(h64))
		{
			must_close(fd);
			die("Can't read full ELF header");
		}
		print_type(h64.e_type, ident[EI_DATA] == ELFDATA2MSB);
		print_entry64(h64.e_entry, ident[EI_DATA] == ELFDATA2MSB);
	}
	else
	{
		must_close(fd);
		die("Unknown ELF class");
	}

	must_close(fd);
	return (0);
}

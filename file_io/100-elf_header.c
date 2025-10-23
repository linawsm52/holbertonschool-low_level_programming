/* 100-elf_header.c */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>

void print_error_and_exit(const char *msg)
{
	dprintf(STDERR_FILENO, "%s\n", msg);
	exit(98);
}

void print_magic(unsigned char *e_ident)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%2.2x", e_ident[i]);
		if (i < EI_NIDENT - 1)
			printf(" ");
	}
	printf("\n");
}

void print_class(unsigned char class)
{
	printf("  Class:                             ");
	switch (class)
	{
	case ELFCLASS32:
		printf("ELF32\n");
		break;
	case ELFCLASS64:
		printf("ELF64\n");
		break;
	default:
		printf("<unknown: %x>\n", class);
	}
}

void print_data(unsigned char data)
{
	printf("  Data:                              ");
	switch (data)
	{
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("<unknown: %x>\n", data);
	}
}

void print_version(unsigned char version)
{
	printf("  Version:                           %d", version);
	if (version == EV_CURRENT)
		printf(" (current)");
	printf("\n");
}

void print_osabi(unsigned char osabi)
{
	printf("  OS/ABI:                            ");
	switch (osabi)
	{
	case ELFOSABI_SYSV:
		printf("UNIX - System V\n");
		break;
	case ELFOSABI_HPUX:
		printf("UNIX - HP-UX\n");
		break;
	case ELFOSABI_NETBSD:
		printf("UNIX - NetBSD\n");
		break;
	case ELFOSABI_LINUX:
		printf("UNIX - Linux\n");
		break;
	case ELFOSABI_SOLARIS:
		printf("UNIX - Solaris\n");
		break;
	case ELFOSABI_IRIX:
		printf("UNIX - IRIX\n");
		break;
	case ELFOSABI_FREEBSD:
		printf("UNIX - FreeBSD\n");
		break;
	case ELFOSABI_OPENBSD:
		printf("UNIX - OpenBSD\n");
		break;
	default:
		printf("<unknown: %x>\n", osabi);
	}
}

void print_abi_version(unsigned char abi)
{
	printf("  ABI Version:                       %d\n", abi);
}

void print_type(uint16_t type, unsigned char *e_ident)
{
	/* If data encoding is big endian, flip bytes */
	if (e_ident[EI_DATA] == ELFDATA2MSB)
		type = (type >> 8) | (type << 8);

	printf("  Type:                              ");
	switch (type)
	{
	case ET_NONE:
		printf("NONE (None)\n");
		break;
	case ET_REL:
		printf("REL (Relocatable file)\n");
		break;
	case ET_EXEC:
		printf("EXEC (Executable file)\n");
		break;
	case ET_DYN:
		printf("DYN (Shared object file)\n");
		break;
	case ET_CORE:
		printf("CORE (Core file)\n");
		break;
	default:
		printf("<unknown>: %x\n", type);
	}
}

void print_entry(uint64_t entry, unsigned char *e_ident)
{
	/* If data encoding is big endian, convert 64-bit */
	if (e_ident[EI_DATA] == ELFDATA2MSB)
	{
		entry = ((uint64_t) (entry & 0x00000000000000FFULL) << 56) |
		        ((uint64_t) (entry & 0x000000000000FF00ULL) << 40) |
		        ((uint64_t) (entry & 0x0000000000FF0000ULL) << 24) |
		        ((uint64_t) (entry & 0x00000000FF000000ULL) << 8)  |
		        ((uint64_t) (entry & 0x000000FF00000000ULL) >> 8)  |
		        ((uint64_t) (entry & 0x0000FF0000000000ULL) >> 24) |
		        ((uint64_t) (entry & 0x00FF000000000000ULL) >> 40) |
		        ((uint64_t) (entry & 0xFF00000000000000ULL) >> 56);
	}

	printf("  Entry point address:               0x%lx\n", (unsigned long)entry);
}

/**
 * main - displays ELF header information
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 98 on error
 */
int main(int argc, char *argv[])
{
	int fd;
	ssize_t r;
	unsigned char e_ident[EI_NIDENT];

	if (argc != 2)
		print_error_and_exit("Usage: elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		print_error_and_exit("Error: Can't open file");

	/* Read the initial bytes (e_ident) to confirm ELF */
	r = read(fd, e_ident, EI_NIDENT);
	if (r < EI_NIDENT)
	{
		close(fd);
		print_error_and_exit("Error: Not an ELF file");
	}

	/* Check magic numbers */
	if (!(e_ident[EI_MAG0] == ELFMAG0 &&
	      e_ident[EI_MAG1] == ELFMAG1 &&
	      e_ident[EI_MAG2] == ELFMAG2 &&
	      e_ident[EI_MAG3] == ELFMAG3))
	{
		close(fd);
		print_error_and_exit("Error: Not an ELF file");
	}

	printf("ELF Header:\n");
	print_magic(e_ident);
	print_class(e_ident[EI_CLASS]);
	print_data(e_ident[EI_DATA]);
	print_version(e_ident[EI_VERSION]);
	print_osabi(e_ident[EI_OSABI]);
	print_abi_version(e_ident[EI_ABIVERSION]);

	/* Now read full header depending on class */
	if (e_ident[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr hdr32;
		/* we already read EI_NIDENT bytes, read the rest */
		lseek(fd, 0, SEEK_SET);
		r = read(fd, &hdr32, sizeof(Elf32_Ehdr));
		if (r < (ssize_t)sizeof(Elf32_Ehdr))
		{
			close(fd);
			print_error_and_exit("Error: Can't read ELF header");
		}
		print_type(hdr32.e_type, e_ident);
		print_entry((uint64_t)hdr32.e_entry, e_ident);
	}
	else if (e_ident[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr hdr64;
		lseek(fd, 0, SEEK_SET);
		r = read(fd, &hdr64, sizeof(Elf64_Ehdr));
		if (r < (ssize_t)sizeof(Elf64_Ehdr))
		{
			close(fd);
			print_error_and_exit("Error: Can't read ELF header");
		}
		print_type(hdr64.e_type, e_ident);
		print_entry((uint64_t)hdr64.e_entry, e_ident);
	}
	else
	{
		close(fd);
		print_error_and_exit("Error: Unknown ELF class");
	}

	if (close(fd) == -1)
		print_error_and_exit("Error: Can't close file descriptor");

	return (0);
}

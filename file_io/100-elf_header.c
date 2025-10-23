#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * print_error - prints an error message to stderr and exits with code 98
 * @msg: error message
 */
void print_error(char *msg)
{
	dprintf(STDERR_FILENO, "%s\n", msg);
	exit(98);
}

/**
 * print_magic - prints the ELF magic numbers
 * @e_ident: ELF identification bytes
 */
void print_magic(unsigned char *e_ident)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", e_ident[i]);
		if (i < EI_NIDENT - 1)
			printf(" ");
	}
	printf("\n");
}

/**
 * print_class - prints the ELF file class (32-bit or 64-bit)
 * @e_ident: ELF identification bytes
 */
void print_class(unsigned char *e_ident)
{
	printf("  Class:                             ");
	switch (e_ident[EI_CLASS])
	{
	case ELFCLASSNONE:
		printf("none\n");
		break;
	case ELFCLASS32:
		printf("ELF32\n");
		break;
	case ELFCLASS64:
		printf("ELF64\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_CLASS]);
	}
}

/**
 * print_data - prints the data encoding type
 * @e_ident: ELF identification bytes
 */
void print_data(unsigned char *e_ident)
{
	printf("  Data:                              ");
	switch (e_ident[EI_DATA])
	{
	case ELFDATANONE:
		printf("none\n");
		break;
	case ELFDATA2LSB:
		printf("2's complement, little endian\n");
		break;
	case ELFDATA2MSB:
		printf("2's complement, big endian\n");
		break;
	default:
		printf("<unknown: %x>\n", e_ident[EI_DATA]);
	}
}

/**
 * print_version - prints the ELF version
 * @e_ident: ELF identification bytes
 */
void print_version(unsigned char *e_ident)
{
	printf("  Version:                           %d", e_ident[EI_VERSION]);
	if (e_ident[EI_VERSION] == EV_CURRENT)
		printf(" (current)");
	printf("\n");
}

/**
 * print_osabi - prints the OS/ABI of the ELF file
 * @e_ident: ELF identification bytes
 */
void print_osabi(unsigned char *e_ident)
{
	printf("  OS/ABI:                            ");
	switch (e_ident[EI_OSABI])
	{
	case ELFOSABI_SYSV:
		printf("UNIX - System V\n");
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
	default:
		printf("<unknown: %x>\n", e_ident[EI_OSABI]);
	}
}

/**
 * print_abi_version - prints the ABI version
 * @e_ident: ELF identification bytes
 */
void print_abi_version(unsigned char *e_ident)
{
	printf("  ABI Version:                       %d\n", e_ident[EI_ABIVERSION]);
}

/**
 * print_type - prints ELF file type (EXEC, DYN, etc.)
 * @type: ELF type value
 * @e_ident: ELF identification bytes (for endianness)
 */
void print_type(unsigned int type, unsigned char *e_ident)
{
	if (e_ident[EI_DATA] == ELFDATA2MSB)
		type >>= 8;

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
		printf("<unknown: %x>\n", type);
	}
}

/**
 * print_entry - prints entry point address
 * @entry: entry point
 * @e_ident: ELF identification bytes (for endianness)
 */
void print_entry(unsigned long entry, unsigned char *e_ident)
{
	if (e_ident[EI_DATA] == ELFDATA2MSB)
		entry = ((entry << 8) & 0xFF00FF00) |
			((entry >> 8) & 0xFF00FF);
	printf("  Entry point address:               %#lx\n", entry);
}

/**
 * main - displays information from the ELF header of a file
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 98 on error
 */
int main(int argc, char *argv[])
{
	int fd;
	Elf64_Ehdr hdr;

	if (argc != 2)
		print_error("Usage: elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		print_error("Error: Can't open file");

	if (read(fd, &hdr, sizeof(hdr)) != sizeof(hdr))
	{
		close(fd);
		print_error("Error: Can't read file");
	}

	if (hdr.e_ident[EI_MAG0] != ELFMAG0 ||
	    hdr.e_ident[EI_MAG1] != ELFMAG1 ||
	    hdr.e_ident[EI_MAG2] != ELFMAG2 ||
	    hdr.e_ident[EI_MAG3] != ELFMAG3)
	{
		close(fd);
		print_error("Error: Not an ELF file");
	}

	printf("ELF Header:\n");
	print_magic(hdr.e_ident);
	print_class(hdr.e_ident);
	print_data(hdr.e_ident);
	print_version(hdr.e_ident);
	print_osabi(hdr.e_ident);
	print_abi_version(hdr.e_ident);
	print_type(hdr.e_type, hdr.e_ident);
	print_entry(hdr.e_entry, hdr.e_ident);

	if (close(fd))
		print_error("Error: Can't close fd");

	return (0);
}

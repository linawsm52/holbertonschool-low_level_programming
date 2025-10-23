#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * print_error - Prints an error message and exits with status 98
 * @msg: The error message to print
 */
void print_error(char *msg)
{
	int len = 0;

	while (msg[len])
		len++;
	write(STDERR_FILENO, msg, len);
	write(STDERR_FILENO, "\n", 1);
	exit(98);
}

/**
 * rev16 - Reverses a 16-bit value for big endian
 * @val: The value to reverse
 * Return: Reversed value
 */
unsigned short rev16(unsigned short val)
{
	return ((val >> 8) | (val << 8));
}

/**
 * rev64 - Reverses bytes of an unsigned long for big endian
 * @val: The value to reverse
 * Return: Reversed value
 */
unsigned long rev64(unsigned long val)
{
	unsigned long res = 0;
	int i;

	for (i = 0; i < 8; i++)
	{
		res <<= 8;
		res |= (val & 0xFF);
		val >>= 8;
	}
	return (res);
}

/**
 * print_magic - Prints ELF magic numbers
 * @e_ident: ELF identification array
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
 * print_class - Prints ELF class
 * @e_ident: ELF identification array
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
 * print_data - Prints ELF data encoding
 * @e_ident: ELF identification array
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
 * print_version - Prints ELF version
 * @e_ident: ELF identification array
 */
void print_version(unsigned char *e_ident)
{
	printf("  Version:                           %d", e_ident[EI_VERSION]);
	if (e_ident[EI_VERSION] == EV_CURRENT)
		printf(" (current)");
	printf("\n");
}

/**
 * print_osabi - Prints ELF OS/ABI
 * @e_ident: ELF identification array
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
 * print_abi_version - Prints ABI version
 * @e_ident: ELF identification array
 */
void print_abi_version(unsigned char *e_ident)
{
	printf("  ABI Version:                       %d\n", e_ident[EI_ABIVERSION]);
}

/**
 * print_type - Prints ELF type
 * @type: The ELF type
 * @e_ident: ELF identification array
 */
void print_type(unsigned short type, unsigned char *e_ident)
{
	if (e_ident[EI_DATA] == ELFDATA2MSB)
		type = rev16(type);

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
 * print_entry - Prints entry point address
 * @entry: The entry point address
 * @e_ident: ELF identification array
 */
void print_entry(unsigned long entry, unsigned char *e_ident)
{
	if (e_ident[EI_DATA] == ELFDATA2MSB)
		entry = rev64(entry);
	printf("  Entry point address:               %#lx\n", entry);
}

/**
 * main - Displays information from ELF header
 * @argc: Argument count
 * @argv: Argument vector
 * Return: 0 on success, 98 on failure
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

	close(fd);
	return (0);
}

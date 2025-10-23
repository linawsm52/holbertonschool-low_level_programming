#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * print_error - prints an error to stderr and exits with code 98
 * @msg: message to print
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
 * rev16 - reverses bytes of a 16-bit value
 * @val: value to reverse
 * Return: reversed value
 */
unsigned short rev16(unsigned short val)
{
	return ((val >> 8) | (val << 8));
}

/**
 * rev32 - reverses bytes of a 32-bit value
 * @val: value to reverse
 * Return: reversed value
 */
unsigned long rev32(unsigned long val)
{
	unsigned long res = 0;
	int i;

	for (i = 0; i < 4; i++)
	{
		res <<= 8;
		res |= (val & 0xFF);
		val >>= 8;
	}
	return (res);
}

/**
 * rev64 - reverses bytes of a 64-bit value
 * @val: value to reverse
 * Return: reversed value
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
 * print_magic - prints ELF magic bytes
 * @id: e_ident array
 */
void print_magic(unsigned char *id)
{
	int i;

	printf("  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		printf("%02x", id[i]);
		if (i < EI_NIDENT - 1)
			printf(" ");
	}
	printf("\n");
}

/**
 * print_class - prints ELF class
 * @id: e_ident array
 */
void print_class(unsigned char *id)
{
	printf("  Class:                             ");
	switch (id[EI_CLASS])
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
		printf("<unknown: %x>\n", id[EI_CLASS]);
	}
}

/**
 * print_data - prints ELF data encoding
 * @id: e_ident array
 */
void print_data(unsigned char *id)
{
	printf("  Data:                              ");
	switch (id[EI_DATA])
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
		printf("<unknown: %x>\n", id[EI_DATA]);
	}
}

/**
 * print_version - prints ELF version
 * @id: e_ident array
 */
void print_version(unsigned char *id)
{
	printf("  Version:                           %d", id[EI_VERSION]);
	if (id[EI_VERSION] == EV_CURRENT)
		printf(" (current)");
	printf("\n");
}

/**
 * print_osabi - prints OS/ABI
 * @id: e_ident array
 */
void print_osabi(unsigned char *id)
{
	printf("  OS/ABI:                            ");
	switch (id[EI_OSABI])
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
		printf("<unknown: %x>\n", id[EI_OSABI]);
	}
}

/**
 * print_abi_version - prints ABI version
 * @id: e_ident array
 */
void print_abi_version(unsigned char *id)
{
	printf("  ABI Version:                       %d\n", id[EI_ABIVERSION]);
}

/**
 * print_type - prints file type
 * @type: ELF file type
 * @id: e_ident array
 */
void print_type(unsigned short type, unsigned char *id)
{
	if (id[EI_DATA] == ELFDATA2MSB)
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
 * print_entry64 - prints 64-bit entry point
 * @entry: entry address
 * @id: e_ident array
 */
void print_entry64(unsigned long entry, unsigned char *id)
{
	if (id[EI_DATA] == ELFDATA2MSB)
		entry = rev64(entry);

	printf("  Entry point address:               %#lx\n", entry);
}

/**
 * print_entry32 - prints 32-bit entry point
 * @entry: entry address
 * @id: e_ident array
 */
void print_entry32(unsigned long entry, unsigned char *id)
{
	if (id[EI_DATA] == ELFDATA2MSB)
		entry = rev32(entry);

	printf("  Entry point address:               %#lx\n", entry);
}

/**
 * main - displays ELF header information
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success, 98 on failure
 */
int main(int argc, char *argv[])
{
	int fd;
	ssize_t n;
	unsigned char id[EI_NIDENT];

	if (argc != 2)
		print_error("Usage: elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		print_error("Error: Can't open file");

	n = read(fd, id, EI_NIDENT);
	if (n != EI_NIDENT)
	{
		close(fd);
		print_error("Error: Can't read file");
	}

	if (!(id[EI_MAG0] == ELFMAG0 && id[EI_MAG1] == ELFMAG1 &&
	      id[EI_MAG2] == ELFMAG2 && id[EI_MAG3] == ELFMAG3))
	{
		close(fd);
		print_error("Error: Not an ELF file");
	}

	printf("ELF Header:\n");
	print_magic(id);
	print_class(id);
	print_data(id);
	print_version(id);
	print_osabi(id);
	print_abi_version(id);

	lseek(fd, 0, SEEK_SET);

	if (id[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr h32;

		n = read(fd, &h32, sizeof(h32));
		if (n != (ssize_t)sizeof(h32))
		{
			close(fd);
			print_error("Error: Can't read ELF header");
		}

		print_type(h32.e_type, id);
		print_entry32((unsigned long)h32.e_entry, id);
	}
	else if (id[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr h64;

		n = read(fd, &h64, sizeof(h64));
		if (n != (ssize_t)sizeof(h64))
		{
			close(fd);
			print_error("Error: Can't read ELF header");
		}

		print_type(h64.e_type, id);
		print_entry64((unsigned long)h64.e_entry, id);
	}
	else
	{
		close(fd);
		print_error("Error: Unknown ELF class");
	}

	close(fd);
	return (0);
}

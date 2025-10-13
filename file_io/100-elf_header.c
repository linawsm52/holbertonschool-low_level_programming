/*
 * File: 100-elf_header.c
 * Author: Lina & ChatGPT
 * Description: Displays the information contained in the ELF header.
 */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <elf.h>

void print_error(char *msg)
{
	write(STDERR_FILENO, "Error: ", 7);
	while (*msg)
		write(STDERR_FILENO, msg++, 1);
	write(STDERR_FILENO, "\n", 1);
	exit(98);
}

void print_magic(unsigned char *e_ident)
{
	int i;

	write(STDOUT_FILENO, "  Magic:   ", 11);
	for (i = 0; i < EI_NIDENT; i++)
	{
		char hex[3];
		const char *space = (i == EI_NIDENT - 1) ? "\n" : " ";
		sprintf(hex, "%02x", e_ident[i]);
		write(STDOUT_FILENO, hex, 2);
		write(STDOUT_FILENO, space, 1);
	}
}

void print_class(unsigned char *e_ident)
{
	write(STDOUT_FILENO, "  Class:                             ", 36);
	if (e_ident[EI_CLASS] == ELFCLASS32)
		write(STDOUT_FILENO, "ELF32\n", 6);
	else if (e_ident[EI_CLASS] == ELFCLASS64)
		write(STDOUT_FILENO, "ELF64\n", 6);
	else
		write(STDOUT_FILENO, "Invalid class\n", 14);
}

void print_data(unsigned char *e_ident)
{
	write(STDOUT_FILENO, "  Data:                              ", 36);
	if (e_ident[EI_DATA] == ELFDATA2LSB)
		write(STDOUT_FILENO, "2's complement, little endian\n", 30);
	else if (e_ident[EI_DATA] == ELFDATA2MSB)
		write(STDOUT_FILENO, "2's complement, big endian\n", 27);
	else
		write(STDOUT_FILENO, "Invalid data encoding\n", 22);
}

void print_version(unsigned char *e_ident)
{
	char buffer[50];
	int len = 0;

	len += sprintf(buffer, "  Version:                           %d", e_ident[EI_VERSION]);
	if (e_ident[EI_VERSION] == EV_CURRENT)
		len += sprintf(buffer + len, " (current)\n");
	else
		len += sprintf(buffer + len, "\n");
	write(STDOUT_FILENO, buffer, len);
}

void print_osabi(unsigned char *e_ident)
{
	write(STDOUT_FILENO, "  OS/ABI:                            ", 36);
	switch (e_ident[EI_OSABI])
	{
	case ELFOSABI_SYSV: write(STDOUT_FILENO, "UNIX - System V\n", 16); break;
	case ELFOSABI_NETBSD: write(STDOUT_FILENO, "UNIX - NetBSD\n", 14); break;
	case ELFOSABI_SOLARIS: write(STDOUT_FILENO, "UNIX - Solaris\n", 15); break;
	default: write(STDOUT_FILENO, "<unknown: ", 10); break;
	}
}

void print_abiversion(unsigned char *e_ident)
{
	char buffer[50];
	int len = sprintf(buffer, "  ABI Version:                       %d\n",
					  e_ident[EI_ABIVERSION]);
	write(STDOUT_FILENO, buffer, len);
}

void print_type(unsigned int type)
{
	char *msg;

	write(STDOUT_FILENO, "  Type:                              ", 36);
	switch (type)
	{
	case ET_NONE: msg = "NONE (None)\n"; break;
	case ET_REL: msg = "REL (Relocatable file)\n"; break;
	case ET_EXEC: msg = "EXEC (Executable file)\n"; break;
	case ET_DYN: msg = "DYN (Shared object file)\n"; break;
	case ET_CORE: msg = "CORE (Core file)\n"; break;
	default: msg = "<unknown>\n"; break;
	}
	write(STDOUT_FILENO, msg, strlen(msg));
}

void print_entry(unsigned long int entry)
{
	char buffer[100];
	int len = sprintf(buffer, "  Entry point address:               0x%lx\n", entry);
	write(STDOUT_FILENO, buffer, len);
}

int main(int argc, char **argv)
{
	int fd;
	Elf64_Ehdr header;

	if (argc != 2)
		print_error("Usage: elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		print_error("Can't open file");

	if (read(fd, &header, sizeof(header)) != sizeof(header))
		print_error("Can't read ELF header");

	if (header.e_ident[0] != 0x7f || header.e_ident[1] != 'E' ||
		header.e_ident[2] != 'L' || header.e_ident[3] != 'F')
		print_error("Not an ELF file");

	write(STDOUT_FILENO, "ELF Header:\n", 12);
	print_magic(header.e_ident);
	print_class(header.e_ident);
	print_data(header.e_ident);
	print_version(header.e_ident);
	print_osabi(header.e_ident);
	print_abiversion(header.e_ident);
	print_type(header.e_type);
	print_entry(header.e_entry);

	close(fd);
	return (0);
}

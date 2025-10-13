/*
 * File: 100-elf_header.c
 * Author: Lina
 * Description: Displays the information contained in the ELF header.
 */

#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * print_error - prints an error message to stderr and exits
 * @msg: the message to print
 */
void print_error(char *msg)
{
	int i = 0;

	while (msg[i])
	{
		write(STDERR_FILENO, &msg[i], 1);
		i++;
	}
	write(STDERR_FILENO, "\n", 1);
	_exit(98);
}

/**
 * check_elf - checks if a file is an ELF file
 * @e_ident: pointer to the ELF magic numbers
 */
void check_elf(unsigned char *e_ident)
{
	if (e_ident[0] != 0x7f || e_ident[1] != 'E' ||
	    e_ident[2] != 'L' || e_ident[3] != 'F')
		print_error("Error: Not an ELF file");
}

/**
 * print_magic - prints ELF magic bytes
 * @e_ident: pointer to ELF header identification
 */
void print_magic(unsigned char *e_ident)
{
	int i;
	char hex[3];
	char *hexset = "0123456789abcdef";

	write(STDOUT_FILENO, "  Magic:   ", 11);
	for (i = 0; i < EI_NIDENT; i++)
	{
		hex[0] = hexset[e_ident[i] >> 4];
		hex[1] = hexset[e_ident[i] & 0x0F];
		write(STDOUT_FILENO, hex, 2);
		if (i < EI_NIDENT - 1)
			write(STDOUT_FILENO, " ", 1);
	}
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * main - displays ELF header info
 * @argc: argument count
 * @argv: argument vector
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
	int fd;
	Elf64_Ehdr header;
	ssize_t r;

	if (argc != 2)
		print_error("Usage: elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		print_error("Error: Can't open file");

	r = read(fd, &header, sizeof(header));
	if (r != sizeof(header))
		print_error("Error: Can't read ELF header");

	check_elf(header.e_ident);

	write(STDOUT_FILENO, "ELF Header:\n", 12);
	print_magic(header.e_ident);

	write(STDOUT_FILENO, "  Class:                             ", 36);
	if (header.e_ident[EI_CLASS] == ELFCLASS64)
		write(STDOUT_FILENO, "ELF64\n", 6);
	else if (header.e_ident[EI_CLASS] == ELFCLASS32)
		write(STDOUT_FILENO, "ELF32\n", 6);
	else
		write(STDOUT_FILENO, "Invalid class\n", 14);

	write(STDOUT_FILENO, "  Data:                              ", 36);
	if (header.e_ident[EI_DATA] == ELFDATA2LSB)
		write(STDOUT_FILENO, "2's complement, little endian\n", 30);
	else if (header.e_ident[EI_DATA] == ELFDATA2MSB)
		write(STDOUT_FILENO, "2's complement, big endian\n", 27);
	else
		write(STDOUT_FILENO, "Invalid data encoding\n", 22);

	close(fd);
	return (0);
}

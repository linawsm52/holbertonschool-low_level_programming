/*
 * File: 100-elf_header.c
 * Author: Lina
 * Description: Displays the information contained in the ELF header.
 */

#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * _putchar - writes a character to stdout
 * @c: character to print
 * Return: 1 on success, -1 on error
 */
int _putchar(char c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

/**
 * print_error - prints error message and exits
 * @msg: message to print
 */
void print_error(char *msg)
{
	int i;

	for (i = 0; msg[i]; i++)
		write(STDERR_FILENO, &msg[i], 1);
	write(STDERR_FILENO, "\n", 1);
	_exit(98);
}

/**
 * print_magic - prints ELF magic numbers
 * @e_ident: pointer to array
 */
void print_magic(unsigned char *e_ident)
{
	int i;
	char c;

	write(STDOUT_FILENO, "  Magic:   ", 11);
	for (i = 0; i < EI_NIDENT; i++)
	{
		c = "0123456789abcdef"[e_ident[i] >> 4];
		_putchar(c);
		c = "0123456789abcdef"[e_ident[i] & 0x0F];
		_putchar(c);
		if (i < EI_NIDENT - 1)
			_putchar(' ');
	}
	_putchar('\n');
}

/**
 * check_elf - verifies ELF file
 * @e_ident: pointer to magic numbers
 */
void check_elf(unsigned char *e_ident)
{
	if (e_ident[0] != 0x7f || e_ident[1] != 'E' ||
	    e_ident[2] != 'L' || e_ident[3] != 'F')
		print_error("Error: Not an ELF file");
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
	ssize_t r;
	Elf64_Ehdr header;

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

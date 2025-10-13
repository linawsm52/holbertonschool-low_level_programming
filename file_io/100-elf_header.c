/*
 * File: 100-elf_header.c
 * Author: Lina
 * Description: Print selected fields from the ELF header (like `readelf -h`).
 */

#include <elf.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

/* ---------- tiny I/O helpers (no stdio) ---------- */

static void wstr(int fd, const char *s)
{
	while (*s)
		write(fd, s++, 1);
}

static void wch(int fd, char c)
{
	write(fd, &c, 1);
}

static void wdec(int fd, unsigned int v)
{
	char buf[12];
	int i = 0;

	if (v == 0)
	{
		wch(fd, '0');
		return;
	}
	while (v)
	{
		buf[i++] = '0' + (v % 10);
		v /= 10;
	}
	while (i--)
		wch(fd, buf[i]);
}

static void whex_u64(int fd, unsigned long v)
{
	char map[] = "0123456789abcdef";
	char buf[16];
	int i = 0, j;

	if (v == 0)
	{
		wch(fd, '0');
		return;
	}
	while (v)
	{
		buf[i++] = map[v & 0xF];
		v >>= 4;
	}
	for (j = i - 1; j >= 0; j--)
		wch(fd, buf[j]);
}

static void die(const char *msg)
{
	wstr(STDERR_FILENO, "Error: ");
	wstr(STDERR_FILENO, (char *)msg);
	wch(STDERR_FILENO, '\n');
	/* لا نستخدم exit من stdlib؛ نضمن كود الخروج 98 */
	_exit(98);
}

/* ---------- endianness helpers ---------- */

static unsigned short bswap16(unsigned short x)
{
	return (unsigned short)((x >> 8) | (x << 8));
}

static unsigned int bswap32(unsigned int x)
{
	return ((x & 0x000000FFU) << 24) |
	       ((x & 0x0000FF00U) << 8)  |
	       ((x & 0x00FF0000U) >> 8)  |
	       ((x & 0xFF000000U) >> 24);
}

static unsigned long bswap64(unsigned long x)
{
	return ((x & 0x00000000000000FFUL) << 56) |
	       ((x & 0x000000000000FF00UL) << 40) |
	       ((x & 0x0000000000FF0000UL) << 24) |
	       ((x & 0x00000000FF000000UL) << 8)  |
	       ((x & 0x000000FF00000000UL) >> 8)  |
	       ((x & 0x0000FF0000000000UL) >> 24) |
	       ((x & 0x00FF000000000000UL) >> 40) |
	       ((x & 0xFF00000000000000UL) >> 56);
}

/* ---------- field printers ---------- */

static void print_magic(const unsigned char *e)
{
	int i;
	wstr(STDOUT_FILENO, "  Magic:   ");
	for (i = 0; i < EI_NIDENT; i++)
	{
		char hi = "0123456789abcdef"[e[i] >> 4];
		char lo = "0123456789abcdef"[e[i] & 0xF];
		wch(STDOUT_FILENO, hi);
		wch(STDOUT_FILENO, lo);
		if (i != EI_NIDENT - 1)
			wch(STDOUT_FILENO, ' ');
	}
	wch(STDOUT_FILENO, '\n');
}

static void print_class(const unsigned char *e)
{
	wstr(STDOUT_FILENO, "  Class:                             ");
	if (e[EI_CLASS] == ELFCLASS32)
		wstr(STDOUT_FILENO, "ELF32\n");
	else if (e[EI_CLASS] == ELFCLASS64)
		wstr(STDOUT_FILENO, "ELF64\n");
	else
		wstr(STDOUT_FILENO, "Invalid class\n");
}

static void print_data(const unsigned char *e)
{
	wstr(STDOUT_FILENO, "  Data:                              ");
	if (e[EI_DATA] == ELFDATA2LSB)
		wstr(STDOUT_FILENO, "2's complement, little endian\n");
	else if (e[EI_DATA] == ELFDATA2MSB)
		wstr(STDOUT_FILENO, "2's complement, big endian\n");
	else
		wstr(STDOUT_FILENO, "Invalid data encoding\n");
}

static void print_version(const unsigned char *e)
{
	wstr(STDOUT_FILENO, "  Version:                           ");
	wdec(STDOUT_FILENO, e[EI_VERSION]);
	if (e[EI_VERSION] == EV_CURRENT)
		wstr(STDOUT_FILENO, " (current)");
	wch(STDOUT_FILENO, '\n');
}

static void print_osabi(const unsigned char *e)
{
	wstr(STDOUT_FILENO, "  OS/ABI:                            ");
	switch (e[EI_OSABI])
	{
	case ELFOSABI_SYSV:      wstr(STDOUT_FILENO, "UNIX - System V\n"); break;
	case ELFOSABI_NETBSD:    wstr(STDOUT_FILENO, "UNIX - NetBSD\n"); break;
	case ELFOSABI_SOLARIS:   wstr(STDOUT_FILENO, "UNIX - Solaris\n"); break;
	case ELFOSABI_GNU:       wstr(STDOUT_FILENO, "UNIX - GNU\n"); break;
	case ELFOSABI_FREEBSD:   wstr(STDOUT_FILENO, "UNIX - FreeBSD\n"); break;
	case ELFOSABI_OPENBSD:   wstr(STDOUT_FILENO, "UNIX - OpenBSD\n"); break;
	case ELFOSABI_AIX:       wstr(STDOUT_FILENO, "UNIX - AIX\n"); break;
	case ELFOSABI_IRIX:      wstr(STDOUT_FILENO, "UNIX - IRIX\n"); break;
	case ELFOSABI_HPUX:      wstr(STDOUT_FILENO, "UNIX - HP-UX\n"); break;
	case ELFOSABI_TRU64:     wstr(STDOUT_FILENO, "UNIX - TRU64\n"); break;
	case ELFOSABI_ARM:       wstr(STDOUT_FILENO, "ARM\n"); break;
	default:
		wstr(STDOUT_FILENO, "<unknown: ");
		wdec(STDOUT_FILENO, (unsigned int)e[EI_OSABI]);
		wstr(STDOUT_FILENO, ">\n");
		break;
	}
}

static void print_abiversion(const unsigned char *e)
{
	wstr(STDOUT_FILENO, "  ABI Version:                       ");
	wdec(STDOUT_FILENO, (unsigned int)e[EI_ABIVERSION]);
	wch(STDOUT_FILENO, '\n');
}

static void print_type(unsigned short t, int msb)
{
	if (msb)
		t = bswap16(t);

	wstr(STDOUT_FILENO, "  Type:                              ");
	switch (t)
	{
	case ET_NONE: wstr(STDOUT_FILENO, "NONE (None)\n"); break;
	case ET_REL:  wstr(STDOUT_FILENO, "REL (Relocatable file)\n"); break;
	case ET_EXEC: wstr(STDOUT_FILENO, "EXEC (Executable file)\n"); break;
	case ET_DYN:  wstr(STDOUT_FILENO, "DYN (Shared object file)\n"); break;
	case ET_CORE: wstr(STDOUT_FILENO, "CORE (Core file)\n"); break;
	default:
		wstr(STDOUT_FILENO, "<unknown: 0x");
		whex_u64(STDOUT_FILENO, (unsigned long)t);
		wstr(STDOUT_FILENO, ">\n");
		break;
	}
}

static void print_entry_u32(unsigned int e, int msb)
{
	if (msb)
		e = bswap32(e);

	wstr(STDOUT_FILENO, "  Entry point address:               0x");
	whex_u64(STDOUT_FILENO, (unsigned long)e);
	wch(STDOUT_FILENO, '\n');
}

static void print_entry_u64(unsigned long e, int msb)
{
	if (msb)
		e = bswap64(e);

	wstr(STDOUT_FILENO, "  Entry point address:               0x");
	whex_u64(STDOUT_FILENO, e);
	wch(STDOUT_FILENO, '\n');
}

/* ---------- main ---------- */

int main(int argc, char **argv)
{
	int fd, msb;
	unsigned char ident[EI_NIDENT];
	ssize_t n;

	if (argc != 2)
		die("Usage: elf_header elf_filename");

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		die("Can't open file");

	/* اقرأ e_ident أولاً للتحقق وتحديد الصنف/الاندينس */
	n = read(fd, ident, EI_NIDENT);
	if (n != EI_NIDENT)
	{
		close(fd);
		die("Can't read ELF header");
	}

	if (!(ident[0] == 0x7f && ident[1] == 'E' && ident[2] == 'L' && ident[3] == 'F'))
	{
		close(fd);
		die("Not an ELF file");
	}

	msb = (ident[EI_DATA] == ELFDATA2MSB);

	wstr(STDOUT_FILENO, "ELF Header:\n");
	print_magic(ident);
	print_class(ident);
	print_data(ident);
	print_version(ident);
	print_osabi(ident);
	print_abiversion(ident);

	/* اقرأ بقية الترويسة حسب الـ class */
	if (ident[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr h32;

		if (lseek(fd, 0, SEEK_SET) == (off_t)-1 ||
		    read(fd, &h32, sizeof(h32)) != (ssize_t)sizeof(h32))
		{
			close(fd);
			die("Can't read full ELF header");
		}
		print_type(h32.e_type, msb);
		print_entry_u32(h32.e_entry, msb);
	}
	else if (ident[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr h64;

		if (lseek(fd, 0, SEEK_SET) == (off_t)-1 ||
		    read(fd, &h64, sizeof(h64)) != (ssize_t)sizeof(h64))
		{
			close(fd);
			die("Can't read full ELF header");
		}
		print_type(h64.e_type, msb);
		print_entry_u64((unsigned long)h64.e_entry, msb);
	}
	else
	{
		close(fd);
		die("Unknown ELF class");
	}

	close(fd);
	return (0);
}

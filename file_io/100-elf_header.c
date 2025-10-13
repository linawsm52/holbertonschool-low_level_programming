static void print_osabi(const unsigned char *e)
{
	wstr(STDOUT_FILENO, "  OS/ABI:                            ");
	switch (e[EI_OSABI])
	{
	case ELFOSABI_SYSV:
		wstr(STDOUT_FILENO, "UNIX - System V\n");
		break;
	case ELFOSABI_NETBSD:
		wstr(STDOUT_FILENO, "UNIX - NetBSD\n");
		break;
	case ELFOSABI_SOLARIS:
		wstr(STDOUT_FILENO, "UNIX - Solaris\n");
		break;
	case ELFOSABI_GNU:
		wstr(STDOUT_FILENO, "UNIX - GNU\n");
		break;
	case ELFOSABI_FREEBSD:
		wstr(STDOUT_FILENO, "UNIX - FreeBSD\n");
		break;
	case ELFOSABI_OPENBSD:
		wstr(STDOUT_FILENO, "UNIX - OpenBSD\n");
		break;
	case ELFOSABI_AIX:
		wstr(STDOUT_FILENO, "UNIX - AIX\n");
		break;
	case ELFOSABI_IRIX:
		wstr(STDOUT_FILENO, "UNIX - IRIX\n");
		break;
	case ELFOSABI_HPUX:
		wstr(STDOUT_FILENO, "UNIX - HP-UX\n");
		break;
	case ELFOSABI_TRU64:
		wstr(STDOUT_FILENO, "UNIX - TRU64\n");
		break;
	case ELFOSABI_ARM:
		wstr(STDOUT_FILENO, "ARM\n");
		break;
	case 103:
		wstr(STDOUT_FILENO, "UNIX - Sortix\n");
		break;
	default:
		wstr(STDOUT_FILENO, "<unknown: ");
		/* ✅ cast to unsigned char fixes the wrong value */
		wdec(STDOUT_FILENO, (unsigned int)((unsigned char)e[EI_OSABI]));
		wstr(STDOUT_FILENO, ">\n");
		break;
	}
}

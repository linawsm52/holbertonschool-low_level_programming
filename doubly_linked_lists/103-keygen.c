#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * main - generates a key depending on a username for crackme5
 * @argc: number of arguments
 * @argv: array of arguments
 * Return: 0 on success
 */
int main(int argc, char *argv[])
{
    unsigned int i, len;
    unsigned long long ch;
    char *username;
    char key[7];
    unsigned int ops[] = {
        0x3877445248432d41,
        0x42394530534e6c37,
        0x4d6e706762695432,
        0x74767a5835737956,
        0x2b554c59634a474f,
        0x71786636576a6d34,
        0x723161513346655a,
        0x6b756f494b646850
    };

    if (argc != 2)
    {
        printf("Usage: %s username\n", argv[0]);
        return (1);
    }

    username = argv[1];
    len = strlen(username);

    key[0] = ((len ^ 59) & 63);
    key[1] = (0);
    ch = 0;
    for (i = 0; i < len; i++)
        ch += username[i];
    key[1] = ((ch ^ 79) & 63);
    ch = 1;
    for (i = 0; i < len; i++)
        ch *= username[i];
    key[2] = ((ch ^ 85) & 63);
    ch = 0;
    for (i = 0; i < len; i++)
        if (username[i] > ch)
            ch = username[i];
    srand(ch ^ 14);
    key[3] = (rand() & 63);
    ch = 0;
    for (i = 0; i < len; i++)
        ch += (username[i] * username[i]);
    key[4] = ((ch ^ 239) & 63);
    for (i = 0; i < username[0]; i++)
        ch = rand();
    key[5] = ((ch ^ 229) & 63);

    for (i = 0; i < 6; i++)
        putchar(((char *)ops)[key[i]]);
    putchar('\n');

    return (0);
}

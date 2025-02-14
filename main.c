#include <string.h>
#include <unistd.h>
#include "ft_malloc.h"

void print(char *s)
{
    write(1, s, strlen(s));
}

int     main()
{
    int     i;
    int     alignment;
    char    *addr;

    i = 1;
    alignment = 2 * sizeof(size_t);
    while (i <= 100)
    {
        addr = (char*)malloc(i);
        if (addr == NULL)
        {
            print("Failed to allocate memory\n");
        }
        if ((((unsigned long) (addr)) % alignment) != 0)
        {
            print("malloc returned a non aligned boundary\n");
        }
        i++;
        free(addr);
    }
}

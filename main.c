#include "ft_malloc.h"

int	main()
{
    char *addr;
    int i;

    i = 0;
    while (i < 1024)
    {
        addr = (char *)malloc(1024);
        addr[0] = 42;
        // free(addr);
        i++;
    }
    return 0;
}

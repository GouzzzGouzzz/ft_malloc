#include "ft_malloc.h"

int	main()
{
    char *addr;
    int i;

    i = 0;
    while (i < 1024)
    {
        addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        addr[1] = 42;
        munmap(addr, 4096);
        // addr = (char *)malloc(1024);
        // addr[0] = 42;
        // free(addr);
        i++;
    }
    return 0;
}

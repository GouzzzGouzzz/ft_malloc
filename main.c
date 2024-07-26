#include "ft_malloc.h"

#define M (1024 * 1024)

void print(char *s)
{
    write(1, s, ft_strlen(s));
}

int     main()
{
    char *addr1;
    char *addr2;
    char *addr3;

    addr1 = (char*)malloc(16*M);
    if (addr1 == NULL)
    {
        print("Failed to allocate memory\n");
    }
    ft_strlcpy(addr1, "Hello world!\n", ft_strlen("Hello world!\n"));
    print(addr1);
    print("toto");
    addr2 = (char*)malloc(16*M);
    if (addr2 == NULL)
    {
        print("Failed to allocate memory\n");
    }
    print("tutu");
    addr3 = (char*)realloc(addr1, 128*M);
    if (addr3 == NULL)
    {
        print("Failed to reallocate memory\n");
    }
    addr3[127*M] = 42;
    print(addr3);
    return (0);
}


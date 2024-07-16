#include "ft_malloc.h"

int main()
{
    char *addr;

    addr = NULL;
    addr = realloc(addr, 11);
}

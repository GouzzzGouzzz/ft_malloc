#include <stdlib.h>
#include <stdio.h>
int main()
{
    char *addr = malloc(122);
    addr = realloc(addr, -1);
    printf("%p\n", addr);
}

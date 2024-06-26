#include "ft_malloc.h"

// #include <stdlib.h>
// #include <stdio.h>
// # include "./libft/libft.h"
int main(){
    char *test = malloc(20);
    for (int i = 0; i < 20; i++)
    {
        malloc(244);
    }
    if (test == NULL)
    {
        printf("CRASHED\n");
        return 1;
    }
    show_alloc_mem();
    free(test);
    show_alloc_mem();
}

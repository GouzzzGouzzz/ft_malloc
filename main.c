#include "ft_malloc.h"

// #include <stdlib.h>
// #include <stdio.h>
// # include "./libft/libft.h"
int main(){
    // char pool[200];
    // ft_bzero(pool, 200);
    // SET_CHUNK_SIZE(pool + sizeof(int), 10);
    // SET_CHUNK_FREE(pool + sizeof(int) + 10);
    // SET_CHUNK_FREE(pool + sizeof(int) + sizeof(int));
    // printf("%p\n", find_chunk(pool, pool + 200, 20));
    // printf("%d\n", calc_free_area(pool + sizeof(int) + 10, pool + 200));
    // printf("%d\n", GET_CHUNK_SIZE(pool + sizeof(int)));
    // printf("%d\n", GET_CHUNK_SIZE(pool + sizeof(int) + 10));


    char *s[2000];
    for (int i = 0; i < 5; i++)
    {
        s[i] = malloc(64);
        if (s[i] == NULL)
            printf("null returned\n");
        else
            printf("BLOCK ALLOCATED: %d\n",i + 1);
    }
    for (int i = 0; i < 5; i++)
        free(s[i]);
    printf("\n\n\nStarting to alloc again on top of previous alloc\n\n\n");
    for (int i = 0; i < 19; i++)
    {
        s[i] = malloc(7);
        if (s[i] == NULL)
            printf("null returned\n");
        else
            printf("BLOCK ALLOCATED: %d\n",i + 1);
        printf("\n\n");
    }
}

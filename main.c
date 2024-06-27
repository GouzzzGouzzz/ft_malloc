#include "ft_malloc.h"

// #include <stdlib.h>
// #include <stdio.h>
// # include "./libft/libft.h"
int main(){
    int i = 0;
    // char mem[40];
    // ft_bzero(mem, 40);
    // SET_CHUNK_FREE(mem + sizeof(int));
    // SET_CHUNK_FREE(mem + 14 + sizeof(int));
    // for (int i = 0; i < 4; i++)
    //     printf("%c\n", mem[i]);
    // if (GET_CHUNK_SIZE(mem + 10 + sizeof(int)) == 0)
    //     printf("DONE\n");

    char *s[2000];
    int total = 0;
    int size = 2;
    while (1)
    {
        s[i] = malloc(size);
        if (s[i] == NULL){
            printf("NO MORE ROOM IN SMALL POOL after %d alloc, total of %d bytes\n", i, (i * size) + (i * 4));
            total = i;
            break;
        }
        i++;
    }
    for (int i = 0; i < total; i++)
        free(s[i]);
    i = 0;
    printf("REALLOCATING ALL : EXPECTING 1638 allocs\n");
    while (1)
    {
        s[i] = malloc(1);
        if (s[i] == NULL){
            printf("NO MORE ROOM IN SMALL POOL after %d alloc\n", i);
            break;
        }
        i++;
    }
}

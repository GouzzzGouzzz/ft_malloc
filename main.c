#include "ft_malloc.h"

// #include <stdlib.h>
// #include <stdio.h>
// # include "./libft/libft.h"
int main(){
    int i = 0;
    char *s[128];
    while (i < 200)
    {
        s[i] = malloc(60);
        if (s[i] == NULL){
            printf("NO MORE ROOM IN SMALL POOL\n");
            break;
        }
        i++;
        printf("%d == i\n",i);
    }
    show_alloc_mem();
    for (int i = 0; i < 128; i++)
        free(s[i]);
    show_alloc_mem();
    i = 0;
    printf("REALLOCATING ALL\n");
    while (i < 200)
    {
        s[i] = malloc(30);
        if (s[i] == NULL){
            printf("NO MORE ROOM IN SMALL POOL\n");
            break;
        }
        i++;
        printf("%d == i\n",i);
    }
}

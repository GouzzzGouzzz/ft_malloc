#include "ft_malloc.h"

// #include <stdlib.h>
// #include <stdio.h>
// # include "./libft/libft.h"
int main(){
    char pool[300];
    ft_bzero(pool, 300);
    SET_CHUNK_FREE(pool + sizeof(int));
    SET_CHUNK_FREE(pool + sizeof(int) + 20);
    SET_CHUNK_FREE(pool + sizeof(int) + 42);
    SET_CHUNK_FREE(pool + sizeof(int) + 192);

    SET_CHUNK_SIZE(pool + 40 + sizeof(int), 10);

    SET_CHUNK_FREE(pool + sizeof(int) + 143);
    SET_CHUNK_FREE(pool + sizeof(int) + 120);
    SET_CHUNK_FREE(pool + sizeof(int) + 230);
    SET_CHUNK_FREE(pool + sizeof(int) + 250);

    //TEST THE FREE FUNCTION TO SEE IF IT IS NOT OVERWRITNG OR ANYTHING

    char *test = malloc(10);
    for (int i = 0;i < 10; i++)
        test[i] = 's';
    for (int i = 0;i < 20; i++)
        printf("%c ", test[i]);
    printf("\n");
    free(test);
    test = malloc(10);
    for (int i = 0;i < 20; i++)
        printf("%c ", test[i]);
    printf("\n");
    // char *s[2000];
    // for (int i = 0; i < 2000; i++)
    //     s[i] = NULL;
    // int size = 60;
    // for (int i = 0; i < 1638; i++)
    // {
    //     s[i] = malloc(60);
    //     if (s[i] == NULL){
    //         printf("NO MORE ROOM IN SMALL POOL after %d alloc of %d size, total of %d bytes\n", i,size, (i * size) + (i * 4));
    //         break;
    //     }
    // }
    // int i = 0;
    // while (s[i] != NULL){
    //     free(s[i]);
    //     i++;
    // }
    // printf("Freed %d allocs\n", i);
    // for (int i = 0; i < 2000; i++)
    //     s[i] = NULL;
    // for (int i = 0; i < 1364; i++)
    // {
    //     // show_alloc_mem();
    //     s[i] = malloc(2);
    //     if (s[i] == NULL){
    //         break;
    //     }
    // }
    // i = 0;
    // while (s[i] != NULL){
    //     free(s[i]);
    //     i++;
    // }
    // printf("Freed %d allocs\n", i);
    // i = 0;
    // printf("REALLOCATING ALL : EXPECTING 1638 allocs\n");
    // while (i < 10)
    // {
    //     s[i] = malloc(1);
    //     if (s[i] == NULL){
    //         printf("NO MORE ROOM IN SMALL POOL after %d alloc\n", i);
    //         break;
    //     }
    //     i++;
    // }

}

#include "ft_malloc.h"

// #include <stdlib.h>
// #include <stdio.h>
// # include "./libft/libft.h"
int main(){
    char *test = malloc(20);
    for (int i = 0; i < 20; i++)
    {
        test[i] = '1';
    }
    if (test == NULL)
    {
        printf("CRASHED\n");
        return 1;
    }
    printf("%s\n", test);
    free(test);
}

#include "ft_malloc.h"

// le pointeur - 1 == size du chunk a free
// le size du chunk == -1 si il est free

void free(void *ptr)
{
    if (ptr == NULL)
        return ;
    int *size_ptr = (int *)ptr - 1;
    int size = *size_ptr;
    *size_ptr = -1;
    for (int i = 0; i < size; i++)
        ((char *)ptr)[i] = '\0';
}

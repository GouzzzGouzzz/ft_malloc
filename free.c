#include "ft_malloc.h"

// le pointeur - 1 == size du chunk a free
// le size du chunk == -1 si il est free

void free(void *ptr)
{
    if (ptr == NULL || ptr == memory_pool + SIZE_MAX_POOL)
        return ;
    int size = GET_CHUNK_SIZE(ptr);
    SET_CHUNK_FREE(ptr);
    for (int i = 0; i < size; i++)
        ((char *)ptr)[i] = '\0';
    printf("Freed %d bytes from : %p\n", size, ptr);
}

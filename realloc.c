#include "ft_malloc.h"

void *realloc(void *ptr, size_t size)
{
    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    (void) ptr;
    (void) size;
    return NULL;
}

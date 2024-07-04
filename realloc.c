#include "ft_malloc.h"

void *realloc(void *ptr, size_t size)
{
    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    if (!ptr)
        return my_malloc(size);
    if (ptr && size == 0)
    {
        my_free(ptr);
        return NULL;
    }
    //start realloc functions logic
}

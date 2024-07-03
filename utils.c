#include "ft_malloc.h"

void init_malloc()
{
    ft_bzero(memory_pool, SIZE_MAX_POOL + ZERO_SIZE_BLOCK + sizeof(void *));
    SET_CHUNK_FREE(memory_pool + sizeof(size_t));
    SET_CHUNK_FREE(memory_pool + sizeof(size_t) + SIZE_SMALL_POOL);
}

int round_up_align(size_t size, int align_to)
{
    int remainder;

    remainder = size % align_to;
    if (remainder == 0)
        return size;
    return size + align_to - remainder;
}

int calc_free_area(char *start, char *end)
{
    char *start_ptr = start;

    while (GET_CHUNK_SIZE(start_ptr) == -1)
    {
        while (start_ptr < end && *start_ptr == '\0')
            start_ptr++;
        start_ptr += sizeof(size_t);
    }
    return start_ptr - start;
}

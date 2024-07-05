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

    while (start_ptr < end && GET_CHUNK_SIZE(start_ptr) == -1)
    {
        while (start_ptr < end && *start_ptr == '\0')
            start_ptr++;
        start_ptr += sizeof(size_t);
    }
    return start_ptr - start;
}

char* find_start(char* to_find)
{
    char *ptr;
    char *next_ptr;
    char *end_ptr;

    ptr = *(void**)(memory_pool + START_LARGE);
    if (!ptr)
        return NULL;
    next_ptr = *(void**)(ptr);
    end_ptr = (ptr + GET_ALLOC_SIZE(ptr));
    ptr += START_MMAP_ALLOC;
    while (ptr != NULL)
    {
        if (ptr <= to_find && to_find <= end_ptr)
            return ptr - START_MMAP_ALLOC;
        ptr = next_ptr;
        next_ptr = *(void**)(ptr);
        end_ptr = (ptr + GET_ALLOC_SIZE(ptr));
        ptr += START_MMAP_ALLOC;
    }
    return NULL;
}

void* find_chunk(char* start, char* end, size_t size_needed)
{
    int size;

    size_needed = round_up_align(size_needed, sizeof(size_t));
    start += sizeof(size_t);
    while (start < end)
    {
        size = GET_CHUNK_SIZE(start);
        if (size == -1)
        {
            int chunk_size;
            chunk_size = calc_free_area(start, end);
            if (chunk_size >= (int)size_needed)
            {
                SET_CHUNK_SIZE(start, size_needed);
                if (start + size_needed + sizeof(size_t) < end && GET_CHUNK_SIZE(start + size_needed + sizeof(size_t)) == 0)
                    SET_CHUNK_FREE(start + size_needed + sizeof(size_t));
                return start;
            }
            start += chunk_size;
        }
        else
            start += size + sizeof(size_t);
    }
    return NULL;
}

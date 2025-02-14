#include "ft_malloc.h"

static void* inplace_extend(char* start, char*end, size_t size, char* ptr)
{
    size_t expandable_size;

    expandable_size = calc_free_area(start, end);
    if (expandable_size + GET_CHUNK_SIZE(ptr) >= size)
    {
        SET_CHUNK_SIZE(ptr, size);
        if (ptr + GET_CHUNK_SIZE(ptr) + ALIGNMENT < end)
            SET_CHUNK_FREE(ptr + GET_CHUNK_SIZE(ptr) + ALIGNMENT);
        return ptr;
    }
    return NULL;
}

static char *extend(size_t size, char* curr_alloc)
{
    char *new_ptr;
    char *start_alloc;
    char *end;

    start_alloc = find_start(curr_alloc);
    end = (start_alloc + GET_ALLOC_SIZE(start_alloc));
    new_ptr = inplace_extend(curr_alloc + GET_CHUNK_SIZE(curr_alloc) + ALIGNMENT, end, size, curr_alloc);
    if (new_ptr != NULL)
        return new_ptr;
    pthread_mutex_unlock(&alloc_acces);
    new_ptr = malloc(size);
    if (!new_ptr)
        return NULL;
    pthread_mutex_lock(&alloc_acces);
    ft_memcpy(new_ptr, curr_alloc, GET_CHUNK_SIZE(curr_alloc));
    pthread_mutex_unlock(&alloc_acces);
    free(curr_alloc);
    return new_ptr;

}

static void shrink(size_t size, size_t curr_size, void *ptr)
{
    if (size < ALIGNMENT)
        size = ALIGNMENT;
    for (size_t i = 0; i < curr_size - size ; i++)
        *(char *)(ptr + curr_size - i) = '\0';
    SET_CHUNK_SIZE(ptr, size);
    SET_CHUNK_FREE(ptr + size + ALIGNMENT);
}

void *realloc(void *ptr, size_t size)
{
    if (!ptr)
        return malloc(size);
    if (!find_start(ptr))
    {
        write(1, "realloc(): invalid pointer\n", 27);
        return NULL;
    }
    if (size > 9223372036854775807)
        return NULL;
    if (ptr && size == 0)
    {
        free(ptr);
        return NULL;
    }
    pthread_mutex_lock(&alloc_acces);
    if (mem_pool.small == NULL)
        init_malloc();
    size = round_up_align(size, ALIGNMENT);
    size_t curr_size = GET_CHUNK_SIZE(ptr);
    if (curr_size == size)
    {
        pthread_mutex_unlock(&alloc_acces);
        return ptr;
    }
    if (size > curr_size)
        return extend(size, ptr);
    shrink(size, curr_size, ptr);
    pthread_mutex_unlock(&alloc_acces);
    return ptr;
}

#include "ft_malloc.h"

static void* inplace_extend(char* start, char*end, size_t size, char* ptr)
{
    int expandable_size;

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

static char *move_alloc_to(char* start, char* end, size_t size_needed, char* curr_alloc)
{
	char* new_ptr;

    new_ptr = inplace_extend(curr_alloc + GET_CHUNK_SIZE(curr_alloc) + ALIGNMENT, end, size_needed, curr_alloc);
    if (new_ptr != NULL)
        return new_ptr;
    new_ptr = malloc(size_needed);
    if (!new_ptr)
        return NULL;
    ft_memcpy(new_ptr, curr_alloc, GET_CHUNK_SIZE(curr_alloc));
    pthread_mutex_unlock(&alloc_acces);
    free(curr_alloc);
    return new_ptr;
}

static char *move_alloc_mmap(size_t size, char* curr_alloc)
{
    char *new_ptr;
    char *start_alloc;
    char *end;

    start_alloc = find_start(curr_alloc);
    end = (start_alloc + GET_ALLOC_SIZE(start_alloc));
    new_ptr = inplace_extend(curr_alloc + GET_CHUNK_SIZE(curr_alloc) + ALIGNMENT, end, size, curr_alloc);
    if (new_ptr != NULL)
        return new_ptr;
    else
    {
        new_ptr = malloc(size);
        if (!new_ptr)
            return NULL;
        ft_memcpy(new_ptr, curr_alloc, GET_CHUNK_SIZE(curr_alloc));
        pthread_mutex_unlock(&alloc_acces);
        free(curr_alloc);
        return new_ptr;
    }
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

static void* extend(size_t size, void *ptr)
{
    void *ret_ptr = NULL;

    if (size < SMALL_VALUE)
        ret_ptr = move_alloc_to(memory_pool, memory_pool + SIZE_SMALL_POOL, size, ptr);
    else if (size < MEDIUM_VALUE)
        ret_ptr =  move_alloc_to(memory_pool + SIZE_SMALL_POOL, memory_pool + SIZE_MAX_POOL, size, ptr);
    else
        ret_ptr =  move_alloc_mmap(size, ptr);
    return ret_ptr;
}

void *realloc(void *ptr, size_t size)
{
    if (!ptr || (((char*)ptr < memory_pool || (char*)ptr > memory_pool + SIZE_MAX_POOL) && !find_start(ptr)))
        return malloc(size);
    if (ptr && size == 0)
    {
        free(ptr);
        return NULL;
    }
    if (size > 9223372036854775807)
        return NULL;
    pthread_mutex_lock(&alloc_acces);
    if (GET_CHUNK_SIZE(memory_pool + ALIGNMENT) == 0)
        init_malloc();

    //All test passed
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

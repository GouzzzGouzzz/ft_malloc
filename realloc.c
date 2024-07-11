#include "ft_malloc.h"

// char *end;
// int prev_size;

// prev_size = GET_CHUNK_SIZE(ptr);
// SET_CHUNK_SIZE(ptr, size);
// end = find_start(ptr);
// end = ptr + GET_ALLOC_SIZE(end - START_MMAP_ALLOC);
// if (ptr + size + sizeof(size_t) < end)
// 	SET_CHUNK_FREE(start + size_needed + sizeof(size_t));
// for (int i = 0; i < prev_size - size - sizeof(size_t); i++)
// 	(char *)(ptr + size + sizeof(size_t))[i] == '\0';


static char *move_alloc_to(char* start, char* end, size_t size_needed, char* curr_alloc)
{
	char* new_ptr;

	new_ptr = find_chunk(start, end, size_needed, curr_alloc);
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
    new_ptr = malloc_mmap(size, curr_alloc);
    if (!new_ptr)
        return NULL;
    ft_memcpy(new_ptr, curr_alloc, GET_CHUNK_SIZE(curr_alloc));
    pthread_mutex_unlock(&alloc_acces);
	free(curr_alloc);

	return new_ptr;
}

void *realloc(void *ptr, size_t size)
{
    if (!ptr || (((char*)ptr < memory_pool || (char*)ptr > memory_pool + SIZE_MAX_POOL) && !find_start(ptr)))
        return malloc(size);
    pthread_mutex_lock(&alloc_acces);
    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    if (ptr && size == 0)
    {
        pthread_mutex_unlock(&alloc_acces);
        free(ptr);
        return NULL;
    }
	size = round_up_align(size, sizeof(size_t));
    size_t curr_size = GET_CHUNK_SIZE(ptr);
	if (curr_size == size)
    {
        pthread_mutex_unlock(&alloc_acces);
		return ptr;
    }
	if (size > curr_size) // need extend
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
    for (size_t i = 0; i < GET_CHUNK_SIZE(ptr) - size - sizeof(size_t); i++)
        ((char *)(ptr + size + sizeof(size_t)))[i] = '\0';
    SET_CHUNK_SIZE(ptr, size);
    SET_CHUNK_FREE(ptr + size + sizeof(size_t));
    pthread_mutex_unlock(&alloc_acces);
    return ptr;
}

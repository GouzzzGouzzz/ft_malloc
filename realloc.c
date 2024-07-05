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

	new_ptr = find_chunk(start, end, size_needed);
	if (!new_ptr)
		return NULL;
	free(ptr)
	ft_memcpy(new_ptr, curr_alloc, GET_CHUNK_SIZE(curr_alloc));
	return new_ptr;
}

static char* handle_small_extend(size_t size, char* curr_ptr, int ptr_location)
{
	char *new_ptr;

	if (ptr_location == 1) // already in
	{
		//try to extend the current alloc considering this one as free
		//null if nothing can be done
		//return new_ptr;
	}
	else// from other to small
	{
		new_ptr = move_alloc_to(memory_pool, memory_pool + SIZE_SMALL_POOL, size, curr_ptr);
		return new_ptr;
	}
	return NULL;
}

static char* handle_medium_extend(size_t size, char* curr_ptr, int ptr_location)
{
	if (ptr_location == 2)// medium to medium
	{
		//try to extend the current alloc
		//else find a new chunk (considering this one as free)
		//null if nothing can be done
		//return new_ptr;
	}
	else
	{
		new_ptr = move_alloc_to(memory_pool + SIZE_SMALL_POOL, memory_pool + SIZE_MAX_POOL, size, ptr);
		return new_ptr;
	}
}

static char* handle_mmap_extend(size_t size, char* curr_ptr, int ptr_location)
{

}

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
	size = round_up_align(size, sizeof(size_t))
	int curr_size = GET_CHUNK_SIZE(ptr);
	int ptr_location = 0;

	if (curr_size == size)
		return ptr;
	if (memory_pool <= ptr && ptr < memory_pool + SIZE_SMALL_POOL)
		ptr_location = 1;
	if (memory_pool + SIZE_SMALL_POOL <= ptr && ptr < memory_pool + SIZE_MAX_POOL)
		ptr_location = 2;

	if (size > curr_size) // need extend
	{
		if (size < SMALL_VALUE)
			return handle_small_extend(size, ptr, ptr_location);
		else if (size < MEDIUM_VALUE)
			return handle_medium_extend(size, ptr, ptr_location);
		else
			return handle_mmap_extend(size, ptr, ptr_location);
	}
}

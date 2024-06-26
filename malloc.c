#include "ft_malloc.h"

//return NULL si la pool small / tiny est full
char memory_pool[SIZE_MAX_POOL];

static void* find_chunk(char* start, char* end, int chunk_size)
{
    char *ptr_chunk;
    int curr_size = 0;
    while (start < end)
    {
        //if -1 ou 0 == free
        //donmc calcul
    }

}

static void* malloc_small(size_t size){
    char *ptr = find_chunk(memory_pool, SIZE_SMALL_POOL, size) + sizeof(int);
    if (ptr == NULL)
        return NULL;
    SET_CHUNK_SIZE(ptr, size);
    return ptr;
}

static void* malloc_medium(size_t size){
    char *ptr = find_chunk(memory_pool + SIZE_SMALL_POOL, SIZE_MAX_POOL, size) + sizeof(int);
    if (ptr == NULL)
        return NULL;
    SET_CHUNK_SIZE(ptr, size);
    return ptr;
}

static void* malloc_mmap(size_t size){
    return NULL;
}

void *malloc(size_t size)
{
    static bool init = true;
    if (init)
    {
        ft_memset(memory_pool, 0, sizeof(memory_pool));
        init = false;
    }
    char *ptr;
    if (size > 9223372036854775807)
        return NULL;
    if (size < SMALL_VALUE)
        ptr = malloc_small(size);
    else if (size < MEDIUM_VALUE)
        ptr = malloc_medium(size);
    else
        ptr = malloc_mmap(size);
    return ptr;
}


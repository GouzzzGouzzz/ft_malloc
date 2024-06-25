#include "ft_malloc.h"

char memory_pool[SIZE_MAX_POOL];
//return NULL si la pool small / tiny est full

static void* malloc_small(size_t size, short int *tiny_size){
    if ((size_t)(*tiny_size) + size > SIZE_SMALL_POOL)
        return NULL;
    printf("pool occupied : %ld\n",(size_t)(*tiny_size) + size);
    memory_pool[*tiny_size] = size;
    char *ptr = &memory_pool[*tiny_size+1];
    *tiny_size += size;
    return ptr;
}

static void* malloc_medium(size_t size, short int *small_size){
    if ((size_t)(*small_size) + size + SIZE_SMALL_POOL > SIZE_MAX_POOL)
        return NULL;
    printf("pool occupied : %ld\n",(size_t)(*small_size) + size);
    memory_pool[*small_size] = size;
    char *ptr = &memory_pool[SIZE_SMALL_POOL + *small_size];
    *small_size += size;
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
        ft_memset(&memory_pool[0], -1, SIZE_MAX_POOL);
        init = false;
    }
    static short int tiny_size = 0;
    static short int small_size = 0;
    char *ptr;
    if (size > 9223372036854775807)
        return NULL;
    if (size < SMALL_VALUE)
        ptr = malloc_small(size, &tiny_size);
    else if (size < MEDIUM_VALUE)
        ptr = malloc_medium(size, &small_size);
    else
        ptr = malloc_mmap(size);
    return ptr;
}


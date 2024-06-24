#include "ft_malloc.h"
# include <stdint.h>

char memory_pool[12288];
//return NULL si la pool small / tiny est full

static void* malloc_tiny(size_t size, short int *tiny_size){
    if ((size_t)(*tiny_size) + size > MAX_POOL)
        return NULL;
    char *ptr = &memory_pool[*tiny_size];
    *tiny_size += size;
    return ptr;
}

static void* malloc_small(size_t size, short int *small_size){
    if ((size_t)small_size + size > MAX_POOL)
        return NULL;
    char *ptr = &memory_pool[*small_size];
    *small_size += size;
    return ptr;
}

static void* malloc_mmap(size_t size){
    return NULL;
}

void *malloc(size_t size)
{
    static short int tiny_size = 0;
    static short int small_size = 0;
    char *ptr;
    if (size > 9223372036854775807)
        return NULL;
    if (size < TINY)
        ptr = malloc_tiny(size, &tiny_size);
    else if (size < SMALL)
        ptr = malloc_small(size, &small_size);
    else
        ptr = malloc_mmap(size);
    printf("returning :%p", ptr);
    return ptr;
}


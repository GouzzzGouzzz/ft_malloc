#include "ft_malloc.h"

static void unlink_area(void* ptr, size_t size)
{
    char *prev;
    char *next;

    next = *(void**)ptr;
    prev = *(void**)(ptr + sizeof(void *));
    if (!prev)//first of the chain (only for large)
    {
        if (!next)//only one
        {
            munmap(ptr, size);
            mem_pool.large = NULL;
        }
        SET_PREV_ADRR(next, NULL);
        mem_pool.large = next;
        munmap(ptr, size);
    }
    if (!next) //the last mmap called
    {
        SET_NEXT_ADDR(prev, NULL);
        munmap(ptr, size);
        return ;
    }
    //between 2 mmap area
    SET_NEXT_ADDR(prev, next);
    SET_PREV_ADRR(next, prev);
    munmap(ptr, size);
    return ;
}

void free(void *ptr)
{
    int size;
    size_t area_size;
    char *start_alloc;

    if (!find_start(ptr))
    {
        write(1, "free(): invalid pointer\n", 25);
        return ;
    }
    pthread_mutex_lock(&alloc_acces);
    size = GET_CHUNK_SIZE(ptr);
    size = round_up_align(size, ALIGNMENT);
    start_alloc = find_start(ptr);
    SET_CHUNK_FREE(ptr);
    if (!start_alloc)
    {
        pthread_mutex_unlock(&alloc_acces);
        return ;
    }
    area_size = GET_ALLOC_SIZE(start_alloc);
    if (GET_ALLOC_NUMBER(start_alloc) == 1 && start_alloc == mem_pool.large + START_MMAP_ALLOC)
        unlink_area(start_alloc, area_size);
    if (GET_ALLOC_NUMBER(start_alloc) == 1 && *(void **)(start_alloc + sizeof(void *)) != NULL) //We keep the first call to mmap //
        unlink_area(start_alloc, area_size);
    else // We can't free the area since more block are still in
    {
        SET_ALLOC_NUMBER(start_alloc, GET_ALLOC_NUMBER(start_alloc) - 1);
        for (int i = 0; i < size; i++)
            ((char *)ptr)[i] = '\0';
    }
    pthread_mutex_unlock(&alloc_acces);
}

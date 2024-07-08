#include "ft_malloc.h"

static void unmap_link(char* ptr, size_t size)
{
    char *prev;
    char *next;

    next = *(void**)(ptr);
    prev = *(void**)(ptr + sizeof(void *));
    if (!prev) //first mmap called
    {
        if (!next) //only one mmap called
        {
            *(void**)(memory_pool + START_LARGE) = NULL;
            munmap(ptr, size);
            return ;
        }
        //another mmap, now it is the first
        *(void**)(memory_pool + START_LARGE) = next;
        SET_PREV_ADRR(next, NULL);
        munmap(ptr, size);
        return;
    }
    else //not the first mmap called
    {
        if (!next) //the last mmap called
        {
            SET_NEXT_ADDR(prev, NULL);
            munmap(ptr, size);
            return ;
        }
        SET_NEXT_ADDR(prev, next);
        SET_PREV_ADRR(next, prev);
        munmap(ptr, size);
        return ;
    }
    return ;
}

void free(void *ptr)
{
    int size;

    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    if (ptr == NULL || ptr == memory_pool + SIZE_MAX_POOL)
        return ;
    size = GET_CHUNK_SIZE(ptr);
    SET_CHUNK_FREE(ptr);
    for (int i = 0; i < size; i++)
        ((char *)ptr)[i] = '\0';
    if ((char *)ptr >= memory_pool && (char *)ptr <= memory_pool + SIZE_MAX_POOL)
        return ;
    else
    {
        int area_size;
        char *start_alloc;

        start_alloc = find_start(ptr);
        if (!start_alloc)
            return ;
        area_size = GET_ALLOC_SIZE(start_alloc);
        if (calc_free_area(start_alloc + START_MMAP_ALLOC, start_alloc + area_size, NULL) == (int)(area_size - START_MMAP_ALLOC + sizeof(size_t)))
            unmap_link(start_alloc, area_size);
    }
}

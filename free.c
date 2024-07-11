#include "ft_malloc.h"

static void unmap_link(void* ptr, size_t size)
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

    if (ptr == NULL || (((char*)ptr < memory_pool || (char*)ptr > memory_pool + SIZE_MAX_POOL) && !find_start(ptr)))
        return ;
    pthread_mutex_lock(&alloc_acces);
    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    size = GET_CHUNK_SIZE(ptr);
    SET_CHUNK_FREE(ptr);
    if ((char *)ptr >= memory_pool && (char *)ptr <= memory_pool + SIZE_MAX_POOL)
    {
        for (int i = 0; i < size; i++)
            ((char *)ptr)[i] = '\0';
    }
    else
    {
        size_t area_size;
        char *start_alloc;
        start_alloc = find_start(ptr);
        if (!start_alloc)
        {
            pthread_mutex_unlock(&alloc_acces);
            return ;
        }
        area_size = GET_ALLOC_SIZE(start_alloc);
        if (GET_ALLOC_NUMBER(start_alloc) == 1)
            unmap_link(&start_alloc, area_size);
        else
        {
            SET_ALLOC_NUMBER(start_alloc, GET_ALLOC_NUMBER(start_alloc) - 1);
            for (int i = 0; i < size; i++)
                ((char *)ptr)[i] = '\0';
        }
    }
    pthread_mutex_unlock(&alloc_acces);
}

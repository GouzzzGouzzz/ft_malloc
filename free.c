#include "ft_malloc.h"

// le pointeur - 1 == size du chunk a free
// le size du chunk == -1 si il est free

void free(void *ptr)
{
    int size;

    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    if (ptr == NULL || ptr == memory_pool + SIZE_MAX_POOL)
        return ;
    size = GET_CHUNK_SIZE(ptr);
    if ((char *)ptr >= memory_pool && (char *)ptr <= memory_pool + SIZE_MAX_POOL)
    {
        SET_CHUNK_FREE(ptr);
        for (int i = 0; i < size; i++)
            ((char *)ptr)[i] = '\0';
    }
    else
    {

        //check if this part of the memory doesn t store any chunk
        //IF PREV_ADDR == NULL --> need to change inside memory_pool
        //then modify the previous "NEXT_ADDR" to point to the next current ptr
        //modify the next chunk "PREV_ADDR" to the addres of the current ptr
        //munmap(ptr, size);
    }
}

#include "ft_malloc.h"

//return NULL si la pool small / tiny est full
char memory_pool[SIZE_MAX_POOL];

static void* find_chunk(char* start, char* end, int size_needed)
{
    int curr_size = 0;
    start += sizeof(int);
    while (start < end)
    {
        int size = GET_CHUNK_SIZE(start);
        if (size  == -1) // found a free chunk
        {
            int chunk_size = 0;
            //
            //NEED TO CHECK IF I FOUND A -1 SIZE WHICH MEANS 2 OR MORE FREED CHUNK
            //CAN BE 1 BIGGER BLOCK
            //
            while (start[chunk_size] == '\0' && (start + chunk_size < end)){
                chunk_size++;
            }
            if (chunk_size >= size_needed) // if size of free chunk is enough allocate it
            {
                //Set the chunk size, and set the next one to free
                SET_CHUNK_SIZE(start, size_needed);
                if (start + size_needed + sizeof(int) < end)
                    if (GET_CHUNK_SIZE(start + size_needed + sizeof(int)) == 0)
                        SET_CHUNK_FREE(start + size_needed + sizeof(int));
                return start;
            }
            start += chunk_size + sizeof(int);
        }
        else
            start += size + sizeof(int);
    }
    return NULL;
}

static void* malloc_small(size_t size){
    char *ptr = find_chunk(memory_pool, memory_pool + SIZE_SMALL_POOL, size);

    if (ptr == NULL)
        return NULL;
    SET_CHUNK_SIZE(ptr, size);
    return ptr;
}

static void* malloc_medium(size_t size){
    char *ptr = find_chunk(memory_pool + SIZE_SMALL_POOL, memory_pool + SIZE_MAX_POOL, size) + sizeof(int);

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
    char *ptr;

    if (init)
    {
        ft_memset(memory_pool, 0, sizeof(memory_pool));
        init = false;
        //MARK BOTH POOL AS FREE
        SET_CHUNK_FREE(memory_pool + sizeof(int));
        SET_CHUNK_FREE(memory_pool + sizeof(int) + SIZE_SMALL_POOL);
    }
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


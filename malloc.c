#include "ft_malloc.h"

//return NULL si la pool small / tiny est full
char memory_pool[SIZE_MAX_POOL];

int calc_free_area(char *start, char* end)
{
    char *start_ptr = start;
    while (start_ptr < end )
    {
        if (GET_CHUNK_SIZE(start_ptr) == -1)
        {
            while (start_ptr < end && *start_ptr == '\0')
                start_ptr++;
            start_ptr += sizeof(int);
        }
        else
            break;
    }
    if (start_ptr - start == 0)
        return 0;
    return start_ptr - start - sizeof(int);
}

void* find_chunk(char* start, char* end, int size_needed)
{
    int size;

    start += sizeof(int); // we start after the chunk headers
    while (start < end)
    {
        size = GET_CHUNK_SIZE(start); // this give -256 it should not be possible
        if (size == -1) // Found the start of a free area / chunk
        {
            char *chunk_start = start; //chunk_start is always after the header
            int chunk_size;
            chunk_size = calc_free_area(chunk_start, end);
            printf("chunk size =%d\n", chunk_size);
            // printf("chunk size = %d,chunk_start = %p, end = %p\n", chunk_size, chunk_start, end);
            if (chunk_size >= size_needed)//allocate
            {
                SET_CHUNK_SIZE(start, size_needed);
                if (start + size_needed + sizeof(int) < end && GET_CHUNK_SIZE(start + size_needed + sizeof(int)) == 0)
                    SET_CHUNK_FREE(start + size_needed + sizeof(int));
                return start;
            }
            start = chunk_start;
        }
        else
        {
            start += size + sizeof(int);
            printf("Added size : %d\n",size);
        }
    }
    return NULL;
}

static void* malloc_small(size_t size){
    char *ptr = find_chunk(memory_pool, memory_pool + SIZE_SMALL_POOL, size);
    if (ptr == NULL){
        // printf("SMALL POOL IS FULL\n");
        return NULL;
    }
    printf("Returning :%p\n",ptr);
    return ptr;
}

static void* malloc_medium(size_t size){
    char *ptr = find_chunk(memory_pool + SIZE_SMALL_POOL, memory_pool + SIZE_MAX_POOL, size);

    if (ptr == NULL)
        return NULL;
    return ptr;
}

static void* malloc_mmap(size_t size){
    (void) size;
    return NULL;
}

void *malloc(size_t size)
{
    static bool init = true;
    char *ptr = NULL;

    if (init)
    {
        ft_memset(memory_pool, 0, SIZE_MAX_POOL);
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


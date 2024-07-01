#include "ft_malloc.h"

//return NULL si la pool small / tiny est full
char memory_pool[SIZE_MAX_POOL + ZERO_SIZE_BLOCK];

static int round_up_align(size_t size)
{
    int remainder;
    remainder = size % sizeof(size_t);
    if (remainder == 0)
        return size;

    return size + sizeof(size_t) - remainder;
}

int calc_free_area(char *start, char *end)
{
    char *start_ptr = start;
    while (GET_CHUNK_SIZE(start_ptr) == -1)
    {
        while (start_ptr < end && *start_ptr == '\0')
        {
            start_ptr++;
        }
        start_ptr += sizeof(size_t);
    }
    return start_ptr - start;
}

void* find_chunk(char* start, char* end, size_t size_needed)
{
    int size;
    size_needed = round_up_align(size_needed);
    start += sizeof(size_t); // we start after the chunk headers
    while (start < end)
    {
        size = GET_CHUNK_SIZE(start);
        // printf("start == %p, end = %p / size = %d\n", start, end, size);
        if (size == -1) // Found the start of a free area / chunk
        {
            int chunk_size;
            chunk_size = calc_free_area(start, end);
            printf("chunk_szie = %d, size_needed = %d\n", chunk_size, size_needed);
            if (chunk_size >= size_needed)
            {
                printf("allocated a block of %ld inside a chunk size =%d\n", size_needed, chunk_size);
                SET_CHUNK_SIZE(start, size_needed);
                if (start + size_needed + sizeof(size_t) < end && GET_CHUNK_SIZE(start + size_needed + sizeof(size_t)) == 0)
                    SET_CHUNK_FREE(start + size_needed + sizeof(size_t));
                return start;
            }
            start += chunk_size;
        }
        else
        {
            start += size + sizeof(size_t);
            // printf("Added size : %d\n",size);
        }
    }
    printf("NOT OFUDN\n");
    return NULL;
}

static void* malloc_small(size_t size){
    char *ptr = find_chunk(memory_pool, memory_pool + SIZE_SMALL_POOL, size);
    if (ptr == NULL){
        // printf("SMALL POOL IS FULL\n");
        return NULL;
    }
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

void init_malloc()
{
    ft_bzero(memory_pool, SIZE_MAX_POOL);
    //MARK BOTH POOL AS FREE
    SET_CHUNK_FREE(memory_pool + sizeof(size_t));
    SET_CHUNK_FREE(memory_pool + sizeof(size_t) + SIZE_SMALL_POOL);
}

void *malloc(size_t size)
{
    char *ptr = NULL;
    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    if (size == 0)
        return memory_pool + SIZE_MAX_POOL;
    if (size > 9223372036854775807)
        return NULL;
    if (size <= SMALL_VALUE)
        ptr = malloc_small(size);
    else if (size <= MEDIUM_VALUE)
        ptr = malloc_medium(size);
    else
        ptr = malloc_mmap(size);
    return ptr;
}


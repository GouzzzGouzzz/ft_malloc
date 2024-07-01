#include "ft_malloc.h"

//return NULL si la pool small / tiny est full
char memory_pool[SIZE_MAX_POOL + ZERO_SIZE_BLOCK];

static int round_up_align(size_t size, int align_to)
{
    int remainder;

    remainder = size % align_to;
    if (remainder == 0)
        return size;
    return size + align_to - remainder;
}

static int calc_free_area(char *start, char *end)
{
    char *start_ptr = start;

    while (GET_CHUNK_SIZE(start_ptr) == -1)
    {
        while (start_ptr < end && *start_ptr == '\0')
            start_ptr++;
        start_ptr += sizeof(size_t);
    }
    return start_ptr - start;
}

static void* find_chunk(char* start, char* end, size_t size_needed)
{
    int size;

    size_needed = round_up_align(size_needed, sizeof(size_t));
    start += sizeof(size_t); //Start after the chunk headers
    while (start < end)
    {
        size = GET_CHUNK_SIZE(start);
        if (size == -1) // Found the start of a free area / chunk
        {
            int chunk_size;
            chunk_size = calc_free_area(start, end);
            if (chunk_size >= size_needed)
            {
                SET_CHUNK_SIZE(start, size_needed);
                if (start + size_needed + sizeof(size_t) < end && GET_CHUNK_SIZE(start + size_needed + sizeof(size_t)) == 0)
                    SET_CHUNK_FREE(start + size_needed + sizeof(size_t));
                return start;
            }
            start += chunk_size;
        }
        else
            start += size + sizeof(size_t);
    }
    return NULL;
}

static void* malloc_small(size_t size){
    char *ptr = find_chunk(memory_pool, memory_pool + SIZE_SMALL_POOL, size);

    if (ptr == NULL)
        return NULL;
    return ptr;
}

static void* malloc_medium(size_t size){
    char *ptr = find_chunk(memory_pool + SIZE_SMALL_POOL, memory_pool + SIZE_MAX_POOL, size);

    if (ptr == NULL)
        return NULL;
    return ptr;
}

static void* malloc_mmap(size_t size){
    void* ptr;
    void *curr_ptr;

    size = round_up_align(size + sizeof(size_t) + 2 * sizeof(void *), sysconf(_SC_PAGESIZE));
    curr_ptr = (void *)memory_pool + SIZE_MAX_POOL;
    if (!curr_ptr)//no area allocated yet
    {
        ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (ptr == MAP_FAILED)
            return NULL;
        SET_CHUNK_SIZE(ptr + sizeof(size_t), size);
        //Set the address of the first large area alocated at the end of memory_pool
        *(void**)(memory_pool + SIZE_MAX_POOL) = ptr;
        SET_PREV_ADRR(ptr, NULL);
    }
    else
    {
        //check in all area already allocated if there is enough space somewhere
        ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
        if (ptr == MAP_FAILED)
            return NULL;
        SET_CHUNK_SIZE(ptr + sizeof(size_t), size);
        //if not new mmap and place prev and next
        SET_PREV_ADRR(ptr, curr_ptr);
        SET_NEXT_ADDR(curr_ptr, ptr);
    }
    return ptr + sizeof(size_t);
}

void init_malloc()
{
    ft_bzero(memory_pool, SIZE_MAX_POOL);
    SET_CHUNK_FREE(memory_pool + sizeof(size_t));
    SET_CHUNK_FREE(memory_pool + sizeof(size_t) + SIZE_SMALL_POOL);
}

void *malloc(size_t size)
{
    char *ptr = NULL;

    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    if (size == 0)
        return memory_pool + SIZE_MAX_POOL + ZERO_SIZE_BLOCK;
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


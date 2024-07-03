#include "ft_malloc.h"

char memory_pool[SIZE_MAX_POOL + ZERO_SIZE_BLOCK + sizeof(void *)];

static void* find_chunk(char* start, char* end, size_t size_needed)
{
    int size;

    size_needed = round_up_align(size_needed, sizeof(size_t));
    start += sizeof(size_t);
    while (start < end)
    {
        size = GET_CHUNK_SIZE(start);
        if (size == -1)
        {
            int chunk_size;
            chunk_size = calc_free_area(start, end);
            if (chunk_size >= (int)size_needed)
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

static char* new_mmap_alloc(int size_to_map, int size_needed)
{
    char *ptr;

    ptr = mmap(NULL, size_to_map, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;
    ft_bzero(ptr, size_to_map);
    SET_CHUNK_SIZE(ptr + START_MMAP_ALLOC, size_needed);
    SET_ALLOC_SIZE(ptr, size_to_map);
    if (ptr + START_MMAP_ALLOC + size_needed < ptr + size_to_map)
        SET_CHUNK_FREE(ptr + START_MMAP_ALLOC + size_needed + sizeof(size_t));
    return ptr;
}

static char* init_mmap(int size_to_map, int size_needed)
{
    char *ptr;

    ptr = new_mmap_alloc(size_to_map, size_needed);
    if (ptr == NULL)
        return NULL;
    *(void**)(memory_pool + START_LARGE) = ptr;
    SET_PREV_ADRR(ptr, NULL);
    SET_NEXT_ADDR(ptr, NULL);
    printf("FIRST ALLOCATION AT:%p with size of %d\n", ptr, size_to_map);
    return ptr + START_MMAP_ALLOC;
}

static char* add_mmap_alloc(int size_to_map, int size_needed, char *curr_ptr)
{
    char *ptr;

    ptr = new_mmap_alloc(size_to_map, size_needed);
    if (ptr == NULL)
        return NULL;
    SET_NEXT_ADDR(ptr, NULL);
    SET_PREV_ADRR(ptr, curr_ptr);
    SET_NEXT_ADDR(curr_ptr, ptr);
    printf("Allocated a new map block at : %p sizeof %d\n", ptr, size_to_map);
    return ptr + START_MMAP_ALLOC;
}

static void* malloc_mmap(size_t size_needed){
    void* ptr = NULL;
    char *curr_ptr;
    char *end_ptr;
    int size_to_map;

    size_needed = round_up_align(size_needed, sizeof(size_t));
    size_to_map = round_up_align(size_needed + START_MMAP_ALLOC, sysconf(_SC_PAGESIZE));
    curr_ptr = *(void**)(memory_pool + START_LARGE);
    if (!curr_ptr)
    {
        return init_mmap(size_to_map, size_needed);
    }
    else
    {
        while (curr_ptr != NULL)
        {
            end_ptr = (curr_ptr + GET_ALLOC_SIZE(curr_ptr));
            ptr = find_chunk(curr_ptr + sizeof(size_t) + (2 * sizeof(void *)), end_ptr, size_needed);
            if (ptr != NULL)
                return ptr;
            if (!*(void**)(curr_ptr))
                break;
            curr_ptr = *(void**)(curr_ptr);
        }
        return add_mmap_alloc(size_to_map, size_needed, curr_ptr);
    }
}

static void* malloc_prealloc(char *start, char *end, size_t size)
{
    char *ptr = find_chunk(start, end, size);

    if (ptr == NULL)
        return NULL;
    return ptr;
}

void *my_malloc(size_t size)
{
    char *ptr = NULL;

    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    if (size == 0)
        return memory_pool + SIZE_MAX_POOL;
    if (size > 9223372036854775807)
        return NULL;
    if (size <= SMALL_VALUE)
        ptr = malloc_prealloc(memory_pool, memory_pool + SIZE_SMALL_POOL, size);
    else if (size <= MEDIUM_VALUE)
        ptr = malloc_prealloc(memory_pool + SIZE_SMALL_POOL, memory_pool + SIZE_MAX_POOL, size);
    else
        ptr = malloc_mmap(size);
    return ptr;
}


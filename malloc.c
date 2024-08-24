#include "ft_malloc.h"

t_alloc mem_pool;
pthread_mutex_t	alloc_acces;

static char* new_mmap_alloc(size_t size_to_map, size_t size_needed)
{
    void *ptr;

    ptr = mmap(NULL, size_to_map, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;
    SET_CHUNK_SIZE(ptr + START_MMAP_ALLOC, size_needed);
    SET_ALLOC_SIZE(ptr, size_to_map);
    SET_ALLOC_NUMBER(ptr, 1);
    if (ptr + START_MMAP_ALLOC + round_up_align(size_needed, ALIGNMENT) < ptr + size_to_map)
        SET_CHUNK_FREE(ptr + START_MMAP_ALLOC + round_up_align(size_needed, ALIGNMENT) + ALIGNMENT);
    return ptr;
}

static char* init_mmap(size_t size_to_map, size_t size_needed)
{
    void *ptr;

    ptr = new_mmap_alloc(size_to_map, size_needed);
    if (ptr == NULL)
        return NULL;
    SET_PREV_ADRR(ptr, NULL);
    SET_NEXT_ADDR(ptr, NULL);
    return ptr + START_MMAP_ALLOC;
}

static char* add_mmap_alloc(size_t size_to_map, size_t size_needed, char *curr_ptr)
{
    char *ptr;
    write(1, "MMAP CREATED\n", 14);
    ptr = new_mmap_alloc(size_to_map, size_needed);
    if (ptr == NULL)
        return NULL;
    SET_NEXT_ADDR(ptr, NULL);
    SET_PREV_ADRR(ptr, curr_ptr);
    SET_NEXT_ADDR(curr_ptr, ptr);
    return ptr + START_MMAP_ALLOC;
}

void* malloc_mmap(size_t size_needed, void *curr_ptr)
{
    void* ptr = NULL;
    char *end_ptr;
    int size_to_map;

    size_to_map = round_up_align(size_needed + START_MMAP_ALLOC, 8 * sysconf(_SC_PAGESIZE));
    if (!curr_ptr) //first large case
        return init_mmap(size_to_map, size_needed);
    else
    {
        while (curr_ptr != NULL)
        {
            end_ptr = (curr_ptr + GET_ALLOC_SIZE(curr_ptr));
            ptr = find_chunk(curr_ptr + (2 * sizeof(size_t)) + (2 * sizeof(void *)), end_ptr, size_needed);
            if (ptr != NULL)
            {
                SET_ALLOC_NUMBER(curr_ptr, GET_ALLOC_NUMBER(curr_ptr) + 1);
                return ptr;
            }
            if (!*(void**)(curr_ptr))
                break;
            curr_ptr = *(void**)(curr_ptr);
        }
        return add_mmap_alloc(size_to_map, size_needed, curr_ptr);
    }
}

void *malloc(size_t size)
{
    char *ptr = NULL;


    if (size > 9223372036854775807)
        return NULL;
    pthread_mutex_lock(&alloc_acces);
    if (mem_pool.small == NULL) //not initalized
        init_malloc();
    if (size <= SMALL_VALUE)
        ptr = malloc_mmap(size, mem_pool.small);
    else if (size <= MEDIUM_VALUE)
        ptr = malloc_mmap(size, mem_pool.medium);
    else
        ptr = malloc_mmap(size, mem_pool.large);
    pthread_mutex_unlock(&alloc_acces);
    return ptr;
}


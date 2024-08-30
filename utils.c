#include "ft_malloc.h"

void init_malloc()
{
    mem_pool.small = mmap(NULL, sysconf(_SC_PAGESIZE) * 4, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    SET_CHUNK_FREE(mem_pool.small + START_MMAP_ALLOC);
    SET_ALLOC_SIZE(mem_pool.small, sysconf(_SC_PAGESIZE) * 4);
    SET_ALLOC_NUMBER(mem_pool.small, 1);
    SET_PREV_ADRR(mem_pool.small, NULL);
    SET_NEXT_ADDR(mem_pool.small, NULL);
    mem_pool.medium = mmap(NULL, sysconf(_SC_PAGESIZE) * 8, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    SET_CHUNK_FREE(mem_pool.medium + START_MMAP_ALLOC);
    SET_ALLOC_SIZE(mem_pool.medium, sysconf(_SC_PAGESIZE) * 8);
    SET_ALLOC_NUMBER(mem_pool.medium, 1);
    SET_PREV_ADRR(mem_pool.medium, NULL);
    SET_NEXT_ADDR(mem_pool.medium, NULL);
}

int round_up_align(size_t size, int align_to)
{
    int remainder;

    remainder = size % align_to;
    if (remainder == 0)
        return size;
    return size + align_to - remainder;
}

int calc_free_area(char *start, char *end)
{
    char *start_ptr = start;

    while (start_ptr < end && GET_CHUNK_SIZE(start_ptr) == -1)
    {
        while (start_ptr < end && *start_ptr == '\0')
            start_ptr++;
        start_ptr += ALIGNMENT;
    }
    return start_ptr - start;
}

static char* search_in(char* to_find, char *ptr)
{
    char *next_ptr;
    char *end_ptr;

    if (!ptr)
        return NULL;
    next_ptr = *(void**)(ptr);
    end_ptr = (ptr + GET_ALLOC_SIZE(ptr));
    ptr += START_MMAP_ALLOC;
    while (ptr != NULL)
    {
        if (ptr <= to_find && to_find <= end_ptr)
            return ptr - START_MMAP_ALLOC;
        ptr = next_ptr;
        if (ptr == NULL)
            break ;
        next_ptr = *(void**)(ptr);
        end_ptr = (ptr + GET_ALLOC_SIZE(ptr));
        ptr += START_MMAP_ALLOC;
    }
    return NULL;
}

void *find_start(char *to_find)
{
    void *start_alloc;

    start_alloc = search_in(to_find, mem_pool.small);
    if (!start_alloc)
        start_alloc = search_in(to_find, mem_pool.medium);
    if (!start_alloc)
        start_alloc = search_in(to_find, mem_pool.large);
    return start_alloc;
}

void* find_chunk(char* start, char* end, size_t size_needed)
{
    int size;

    start += ALIGNMENT;
    while (start < end)
    {
        size = GET_CHUNK_SIZE(start);
        if (size == -1)
        {
            size_t chunk_size;
            chunk_size = calc_free_area(start, end);
            if (chunk_size >= size_needed)
            {
                SET_CHUNK_SIZE(start, size_needed);
                if (start + round_up_align(size_needed, ALIGNMENT) + ALIGNMENT < end && round_up_align(GET_CHUNK_SIZE(start + size_needed + ALIGNMENT), ALIGNMENT) == 0)
                    SET_CHUNK_FREE(start + round_up_align(size_needed, ALIGNMENT) + ALIGNMENT);
                return start;
            }
            start += chunk_size;
        }
        else
            start += round_up_align(size, ALIGNMENT) + ALIGNMENT;
    }
    return NULL;
}

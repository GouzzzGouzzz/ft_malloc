#ifndef FT_MALLOC
# define FT_MALLOC
# include <sys/mman.h>
# include <sys/resource.h>
# include "./libft/libft.h"
# include <pthread.h>

//Various value to simplify memory access for my array / ptr
# define SMALL_VALUE 64
# define MEDIUM_VALUE 256
# define ALIGNMENT (2 * sizeof(size_t))

//2 size_t for the allocated size, and the first chunk size, 2 void * for the prev and next allocated address
//1 more size_t to be aligned in 16 bytes (so there is 48 bytes of metadata, only 40 are used)
# define START_MMAP_ALLOC ((4 * sizeof(size_t)) + (2 * sizeof(void *)))

//Macro
//Setter and getter for the chunk allocations
# define SET_CHUNK_SIZE(ptr, size) (*(int *)((char *)(ptr) - sizeof(size_t)) = (int) size)
# define GET_CHUNK_SIZE(ptr) (*(int *)((char *)(ptr) - sizeof(size_t)))
# define SET_CHUNK_FREE(ptr) (*(int *)((char *)(ptr) - sizeof(size_t)) = (int)-1)
//Setter and getter for the mmap allocations
# define SET_NEXT_ADDR(ptr, next_ptr) *(void **)(char *)(ptr) = (void *)next_ptr
# define SET_PREV_ADRR(ptr, prev_ptr) *(void **)(char *)(ptr + sizeof(void *)) = (void *)prev_ptr
# define GET_ALLOC_SIZE(ptr) (*(int *)((char *)(ptr) + (2 * sizeof(void *))))
# define SET_ALLOC_SIZE(ptr, size) (*(int *)((char *)(ptr) + (2 * sizeof(void *))) = (int) size)
# define SET_ALLOC_NUMBER(ptr, nb) (*(int *)((char *)(ptr) + (2 * sizeof(void *) + sizeof(size_t)))) = (int) nb
# define GET_ALLOC_NUMBER(ptr) (*(int *)((char *)(ptr) + (2 * sizeof(void *) + sizeof(size_t))))

//Global var to manage all allocations from my malloc
//void * at the end pointing to the first mmap alloc (default : NULL)
typedef struct alloc_ptr
{
    void *small;
    void *medium;
    void *large;
}   t_alloc;

extern t_alloc mem_pool;
extern pthread_mutex_t	alloc_acces;

//Main functions
void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();

//Utils
void    init_malloc();
int     round_up_align(size_t size, int align_to);
int     calc_free_area(char *start, char *end);
void*   find_start(char* to_find);
void*   find_chunk(char* start, char* end, size_t size_needed);
void*   malloc_mmap(size_t size_needed, void *curr_ptr);

#endif

#ifndef FT_MALLOC
# define FT_MALLOC
# include <sys/mman.h>
# include <sys/resource.h>
# include "./libft/libft.h"

// #include <stdio.h>

//Various value to simplify memory access for my array / ptr
# define ZERO_SIZE_BLOCK 16
# define SMALL_VALUE 64
# define MEDIUM_VALUE 256
# define SIZE_SMALL_POOL 8192
# define SIZE_MEDIUM_POOL 28672
# define SIZE_MAX_POOL SIZE_SMALL_POOL + SIZE_MEDIUM_POOL
# define START_LARGE SIZE_MAX_POOL + ZERO_SIZE_BLOCK

//2 size_t for the allocated size, and the first chunk size, 2 void * for the prev and next allocated address
# define START_MMAP_ALLOC ((2 * sizeof(size_t)) + (2 * sizeof(void *)))

//Macro
//Setter and getter for the chunk allocations
# define SET_CHUNK_SIZE(ptr, size) (*(int *)((char *)(ptr) - sizeof(size_t)) = (int) size)
# define GET_CHUNK_SIZE(ptr) (*(int *)((char *)(ptr) - sizeof(size_t)))
# define SET_CHUNK_FREE(ptr) (*(int *)((char *)(ptr) - sizeof(size_t)) = (int)-1)
//Setter and getter for the mmap allocations
# define SET_NEXT_ADDR(ptr, next_ptr) *(void **)(char *)(ptr) = (void *)next_ptr;
# define SET_PREV_ADRR(ptr, prev_ptr) *(void **)(char *)(ptr + sizeof(void *)) = (void *)prev_ptr
# define GET_ALLOC_SIZE(ptr) (*(int *)((char *)(ptr) + (2 * sizeof(void *))))
# define SET_ALLOC_SIZE(ptr, size) (*(int *)((char *)(ptr) + (2 * sizeof(void *))) = (int) size)

//Global var to manage all allocations from my malloc
//ZERO_SIZE_BLOCK for the malloc(0) case
//void * at the end pointing to the first mmap alloc (default : NULL)
extern char memory_pool[SIZE_MAX_POOL + ZERO_SIZE_BLOCK + sizeof(void *)];

//Main functions
// void    free(void *ptr);
// void    *malloc(size_t size);
void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();

//Utils
void    init_malloc();
int     round_up_align(size_t size, int align_to);
int     calc_free_area(char *start, char *end, char *curr_chunk);
char*   find_start(char* to_find);
void*   find_chunk(char* start, char* end, size_t size_needed, char* curr_alloc);
void*   malloc_mmap(size_t size_needed, char *curr_alloc);

#endif

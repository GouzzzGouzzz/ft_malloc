#ifndef FT_MALLOC
# define FT_MALLOC
# include <sys/mman.h>
# include <sys/resource.h>
# include "./libft/libft.h"
# include <stdio.h>

//Value
# define ZERO_SIZE_BLOCK 16
# define SMALL_VALUE 64
# define MEDIUM_VALUE 256
# define SIZE_SMALL_POOL 8192
# define SIZE_MEDIUM_POOL 28672
# define SIZE_MAX_POOL SIZE_SMALL_POOL + SIZE_MEDIUM_POOL

//Macro
# define SET_CHUNK_SIZE(ptr, size) (*(int *)((char *)(ptr) - sizeof(size_t)) = (int) size)
# define GET_CHUNK_SIZE(ptr) (*(int *)((char *)(ptr) - sizeof(size_t)))
# define SET_CHUNK_FREE(ptr) (*(int *)((char *)(ptr) - sizeof(size_t)) = (int)-1)

# define SET_NEXT_ADDR(ptr, next_ptr) *(void **)(char *)(ptr + GET_CHUNK_SIZE(ptr) - (2 * sizeof(void *))) = (void *)next_ptr;
# define SET_PREV_ADRR(ptr, prev_ptr) *(void **)(char *)(ptr + size - sizeof(void *)) = (void *)prev_ptr
# define GET_NEXT_ADDR(ptr) *(void **)(char *)(ptr + GET_CHUNK_SIZE(ptr) - (2 * sizeof(void *)))

//Global var for the allocation contains the small and the medium pool
//first SIZE_SMALL_POOL bytes correspond to the small pool, the rest is the medium
extern char memory_pool[SIZE_MAX_POOL + ZERO_SIZE_BLOCK + sizeof(void *)];

//Main functions
void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();

//Utils
void    init_malloc();

#endif

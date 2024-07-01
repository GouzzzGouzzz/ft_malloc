#ifndef FT_MALLOC
# define FT_MALLOC
# include <sys/mman.h>
# include <sys/resource.h>
# include "./libft/libft.h"
# include <stdio.h>

//REVOIR LES VALEURS PLUS TARD (need +1 pour la size des chunks + moins grandre valeur ?)
# define SMALL_VALUE 64
# define MEDIUM_VALUE 256
//MAYBE -1 sur les valeurs
# define SIZE_SMALL_POOL 8192 // 118 alloc of 64 (for int)
# define SIZE_MEDIUM_POOL 28672 // 108 alloc of 256
# define SIZE_MAX_POOL SIZE_SMALL_POOL + SIZE_MEDIUM_POOL

//Some Macro will be useful
#define SET_CHUNK_SIZE(ptr, size) (*(int *)((char *)(ptr) - sizeof(size_t)) = (int) size)
#define GET_CHUNK_SIZE(ptr) (*(int *)((char *)(ptr) - sizeof(size_t)))
#define SET_CHUNK_FREE(ptr) (*(int *)((char *)(ptr) - sizeof(size_t)) = (int)-1)

//Added memory for the zero size block, can be passed to free safely, it points to a unique pointer value
#define ZERO_SIZE_BLOCK 16

//global var for the allocation
//contains the small and the medium pool
//first SIZE_SMALL_POOL bytes correspond to the small pool, the rest is the medium
extern char memory_pool[SIZE_MAX_POOL + ZERO_SIZE_BLOCK];

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();
void    init_malloc();
//mmap(2)
//munmap(2)
//sysconf(_SC_PAGESIZE) under linux
//getrlimit(2)
void* find_chunk(char* start, char* end, size_t size_needed);
int calc_free_area(char *start, char* end);

#endif

    // TINY : 0xA0000
    // 0xA0020 - 0xA004A : 42 bytes
    // 0xA006A - 0xA00BE : 84 bytes
    // SMALL : 0xAD000
    // 0xAD020 - 0xADEAD : 3725 bytes
    // LARGE : 0xB0000
    // 0xB0020 - 0xBBEEF : 48847 bytes
    // Total : 52698 bytes

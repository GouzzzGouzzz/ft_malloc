#ifndef FT_MALLOC
# define FT_MALLOC
# include <sys/mman.h>
# include <sys/resource.h>
# include "./libft/libft.h"
# include <stdio.h>
# include <stdbool.h>

//REVOIR LES VALEURS PLUS TARD (need +1 pour la size des chunks + moins grandre valeur ?)
# define SMALL_VALUE 64
# define MEDIUM_VALUE 256
//MAYBE -1 sur les valeurs
# define SIZE_SMALL_POOL 8192 // 128 alloc of 60 + 4(for int)
# define SIZE_MAX_POOL 28672 //112 alloc of 252 + 4 (for int)

//Some Macro will be useful
#define SET_CHUNK_SIZE(ptr, size) (*(int *)((char *)(ptr) - sizeof(int)) = (int) size)
#define GET_CHUNK_SIZE(ptr) (*(int *)((char *)(ptr) - sizeof(int)))
#define SET_CHUNK_FREE(ptr) (*(int *)((char *)(ptr) - sizeof(int)) = (-1))

//global var for the allocation
//contains the small and the medium pool
//first SIZE_SMALL_POOL bytes correspond to the small pool, the rest is the medium
extern char memory_pool[SIZE_MAX_POOL];

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();

//mmap(2)
//munmap(2)
//sysconf(_SC_PAGESIZE) under linux
//getrlimit(2)
void* find_chunk(char* start, char* end, int size_needed);
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

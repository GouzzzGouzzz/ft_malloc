#ifndef FT_MALLOC
# define FT_MALLOC
# include <sys/mman.h>
# include <sys/resource.h>
# include "./libft/libft.h"

# include <stdio.h>


# define TINY 64
# define SMALL 256
//MAYBE -1 sur les valeurs
# define SMALL_POOL 4096
# define MAX_POOL 12288

//global var for the allocation
//contains the tiny and the small pool
//first 4096 bytes correspond to the tiny pool, the rest is the small
extern char memory_pool[12288];

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);

//mmap(2)
//munmap(2)
//sysconf(_SC_PAGESIZE) under linux
//getrlimit(2)

#endif

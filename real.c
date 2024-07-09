#include <stdlib.h>

int	main()
{
    char *addr ,*t, *p, *x;
    addr = malloc(1024);
    t = malloc(8000);
    p = malloc(16000);
    x = malloc(81920);
    x[0] = 10;
    // munmap((x - START_MMAP_ALLOC),86016);
    free(addr);
    free(t);
    free(p);
    free(x);
	return (0);
}


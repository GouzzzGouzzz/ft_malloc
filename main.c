#include "ft_malloc.h"


#define M (1024 * 1024)

void print(char *s)
{
	write(1, s, strlen(s));
}

int main()
{
	char *addr;

	addr = malloc(16);
	// if (realloc((void *)addr + 5, 10) == NULL)
	// 	print("Bonjours\n");
	free(addr);
}

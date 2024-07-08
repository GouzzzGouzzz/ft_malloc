#include "ft_malloc.h"


int	main()
{
	int i;
	char *addr;

	i = 0;
	while (i < 4)
	{
		addr = (char*)my_malloc(1024);
		addr[0] = 42;
		my_free(addr);
		i++;
	}
	return (0);
}

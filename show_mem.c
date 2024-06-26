#include "ft_malloc.h"

    // TINY : 0xA0000
    // 0xA0020 - 0xA004A : 42 bytes
    // 0xA006A - 0xA00BE : 84 bytes
    // SMALL : 0xAD000
    // 0xAD020 - 0xADEAD : 3725 bytes
    // LARGE : 0xB0000
    // 0xB0020 - 0xBBEEF : 48847 bytes
    // Total : 52698 bytes

static void	ft_print_arr_rev(int size, char *tab)
{
	while (size > 1)
	{
		size--;
		write(1, &tab[size], 1);
	}
}

static void	ft_print_address(unsigned long address)
{
	int					i;
	char				*hexa_set;
	char				address_arr[16];

	hexa_set = "0123456789abcdef";
	i = 0;
	write(1, "0x", 2);
	while (address > 0)
	{
		address_arr[i] = hexa_set[(address % 16)];
		address = address / 16;
		i++;
	}
	ft_print_arr_rev(i, address_arr);
}

static void calc_prealloc_small()
{
    char *ptr = memory_pool;
    printf("%p - %p\n", (void *)ptr, memory_pool + SIZE_SMALL_POOL);
    ft_putstr_fd("TINY : ", 1);
    ft_print_address((unsigned long)&memory_pool[0]);
    while (ptr < memory_pool + SIZE_SMALL_POOL)
    {
        int size = GET_CHUNK_SIZE(ptr);
        printf("%d\n", size);
        if (size <= 0)
            break;
        ft_print_address((unsigned long)&ptr);
        ptr += size + sizeof(int);
        write(1, " - ", 3);
        ft_print_address((unsigned long)&ptr);
        write(1, " : ", 3);
        ft_putnbr_fd(size, 1);
        write(1, "\n", 1);
    }
}

void show_alloc_mem()
{
    calc_prealloc_small();
}

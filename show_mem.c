#include "ft_malloc.h"

static void	ft_print_arr_rev(int size, char *tab)
{
	while (size > 0)
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

static int calc_prealloc_small()
{
    char *ptr = memory_pool + sizeof(size_t);
    int total = 0;

    write(1, "SMALL : ", 9);
    ft_print_address((unsigned long)memory_pool);
    write(1, "\n", 1);
    while (ptr < memory_pool + SIZE_SMALL_POOL)
    {
        int size = GET_CHUNK_SIZE(ptr);
        if (size <= -1) //Skipping freed chunk
        {
            while (*ptr == '\0')
            {
                ptr++;
                if (ptr + sizeof(size_t) == memory_pool + SIZE_SMALL_POOL)
                    return total;
            }
            ptr += sizeof(size_t);
            continue ;
        }
        total += size;
        ft_print_address((unsigned long)ptr);
        ptr += size;
        write(1, " - ", 3);
        ft_print_address((unsigned long)ptr);
        ptr += sizeof(size_t);
        write(1, " : ", 3);
        ft_putnbr_fd(size, 1);
        write(1, " bytes\n", 8);
    }
    return total;
}

static int calc_prealloc_medium()
{
    char *ptr = memory_pool + sizeof(size_t) + SIZE_SMALL_POOL;
    int total = 0;

    write(1, "MEDIUM : ", 10);
    ft_print_address((unsigned long)memory_pool + SIZE_SMALL_POOL);
    write(1, "\n", 1);
    while (ptr < memory_pool + SIZE_MAX_POOL)
    {
        int size = GET_CHUNK_SIZE(ptr);
        if (size <= -1) //Skipping freed chunk
        {
            while (*ptr == '\0')
            {
                ptr++;
                if (ptr + sizeof(size_t) == memory_pool + SIZE_MAX_POOL)
                    return total;
            }
            ptr += sizeof(size_t);
            continue ;
        }
        total += size;
        ft_print_address((unsigned long)ptr);
        ptr += size;
        write(1, " - ", 3);
        ft_print_address((unsigned long)ptr);
        ptr += sizeof(size_t);
        write(1, " : ", 3);
        ft_putnbr_fd(size, 1);
        write(1, " bytes\n", 8);
    }
    return total;
}

void show_alloc_mem()
{
    int total = 0;
    int medium_total = 0;
    if (GET_CHUNK_SIZE(memory_pool + sizeof(size_t)) == 0)
        init_malloc();
    total += calc_prealloc_small();
    if (total == 0)
        write(1, "None\n",6);
    medium_total += calc_prealloc_medium();
    if (medium_total == 0)
        write(1, "None\n",6);
    total += medium_total;
    write(1, "Total : ", 9);
    ft_putnbr_fd(total, 1);
    write(1, " bytes\n", 8);
}

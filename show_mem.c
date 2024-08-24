#include "ft_malloc.h"

void print_address(unsigned long address)
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
    while (i > 0)
	{
		i--;
		write(1, &address_arr[i], 1);
	}
}

static void print_bytes(char *str1, int nb)
{
    ft_putstr_fd(str1, 1);
    ft_putnbr_fd(nb, 1);
    ft_putendl_fd(" bytes", 1);
}

static void print_addr_diff(char *ptr, int size)
{
    print_address((unsigned long)ptr);
    write(1, " - ", 3);
    print_address((unsigned long)ptr + size);
}

static void print_start_addr(char *str, char* start)
{
    ft_putstr_fd(str, 1);
    if (!start)
        write(1, "NULL", 4);
    else
        print_address((unsigned long)start);
    write(1, "\n", 1);
}

static size_t calc_mmap_alloc(char *ptr)
{
    size_t total = 0;
    char *next_ptr;
    char *end_ptr;

    if (!ptr)
        return 0;
    next_ptr = *(void**)(ptr);
    end_ptr = (ptr + GET_ALLOC_SIZE(ptr));
    ptr += START_MMAP_ALLOC;
    while (1)
    {
        while (ptr < end_ptr)
        {
            int size = GET_CHUNK_SIZE(ptr);
            if (size == -1)
            {
                while (ptr < end_ptr && *ptr == '\0')
                    ptr++;
            }
            if (ptr == end_ptr)
                break;
            total += size;
            print_addr_diff(ptr, size);
            ptr += ALIGNMENT + round_up_align(size, ALIGNMENT);
            print_bytes(" : ", size);
        }
        if (!next_ptr)
            return total;
        ptr = next_ptr;
        next_ptr = *(void**)(ptr);
        end_ptr = (ptr + GET_ALLOC_SIZE(ptr));
        ptr += START_MMAP_ALLOC;
    }
    return total;
}

void show_alloc_mem()
{
    size_t small_total, medium_total, large_total = 0;

    pthread_mutex_lock(&alloc_acces);
    print_start_addr("SMALL  : ", mem_pool.small);
    small_total = calc_mmap_alloc(mem_pool.small);
    if (small_total == 0)
        write(1, "None\n",6);

    print_start_addr("MEDIUM  : ", mem_pool.medium);
    medium_total = calc_mmap_alloc(mem_pool.medium);
    if (medium_total == 0)
        write(1, "None\n",6);

    print_start_addr("LARGE  : ", mem_pool.large);
    large_total = calc_mmap_alloc(mem_pool.large);
    if (large_total == 0)
        write(1, "None\n",6);

    print_bytes("Total : ", small_total + medium_total + large_total);
    pthread_mutex_unlock(&alloc_acces);
}

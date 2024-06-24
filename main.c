#include "ft_malloc.h"
#include <stdio.h>


int main(){
    printf("%ld\n",sysconf(_SC_PAGESIZE));
}


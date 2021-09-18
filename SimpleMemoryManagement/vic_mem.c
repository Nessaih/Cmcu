#include <stdio.h>
#include "vic_mem.h"

#define MEM_MAX_SIZE		(0x4000)


static mem_unit_t mem_pool[MEM_MAX_SIZE];
static mem_size_t mem_tabl[MEM_MAX_SIZE];



void *vic_malloc(mem_size_t sz)
{
    long long i,j;
    void *ptr = NULL;

    for (i = MEM_MAX_SIZE; i - sz >= 0; --i) {
        if(0 == mem_tabl[i-1] && 0 == mem_tabl[i-sz]) {
            for(j = i - 1; j >= i-sz; --j)
                mem_tabl[j] = sz;
            ptr = (void *)&mem_pool[i-sz];
            break;
        }
    }
    return ptr;
}


void vic_free(void *ptr)
{
    long long offset,i;
    mem_size_t sz;

    offset = ptr - (void *)mem_pool;

    if(offset >= 0 && offset < MEM_MAX_SIZE){
        sz = mem_tabl[offset];
        for(i = 0; i < sz; ++i)
            mem_tabl[offset+i] = 0;
    }
}
void vic_show(void)
{
    long long i;
    mem_size_t used,free;

    for (i = MEM_MAX_SIZE; i > 0; --i) {
        if(mem_tabl[i-1] == 0)
        {
            break;
        }
    }
    used = MEM_MAX_SIZE - i;
    free = i;
    printf("vic memory \t total:%-10u\tused:%-10u\tfree:%-10u\t\r\n",MEM_MAX_SIZE,used,free);
}

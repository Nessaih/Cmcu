#ifndef __VIC_MEM_H__
#define __VIC_MEM_H__

#ifdef _cplusplus
extern "C"{
#endif


typedef unsigned char mem_unit_t;
typedef unsigned int  mem_size_t;

extern void *vic_malloc(mem_size_t sz);
extern void vic_free(void *ptr);
extern void vic_show(void);

#ifdef _cplusplus
}
#endif

#endif

#ifndef _RAM_MANAGE_H
#define _RAM_MANAGE_H
#include "cdv_include.h" 

//mem内存参数设定,mem完全处于外部SRAM里面
#define MEM_MNG_START       SRAM_START
#define MEM_MNG_MAX_SIZE		SRAM_SIZE/8*6
#define MEM_MNG_BASE        __attribute__((at(MEM_MNG_START)))
#define MEM_MNG_SIZE        MEM_MNG_MAX_SIZE         //内存块大小
#define MEM_MNG_MAP_SIZE    MEM_MNG_MAX_SIZE/8       //内存块map大小
#define MEM_MNG_MAPBASE     __attribute__((at(MEM_MNG_START + MEM_MNG_SIZE)))


void memmng_memcpy(void *des,void *src,u32 n);
void *memmng_malloc(size_t size );
void memmng_free(void *p );
void memmng_memset(void *p, u8 c, u32 count);
//外扩sram内存管理初始化，必须在程序一开始使用
void memmng_init(void);
//外扩sram内存块使用率，返回0-100
u8 memmng_perused(void);
void *memmng_realloc(void *p,size_t size);

#endif



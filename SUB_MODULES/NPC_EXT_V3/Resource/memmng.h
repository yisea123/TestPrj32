#ifndef _RAM_MANAGE_H
#define _RAM_MANAGE_H
#include "cdv_include.h" 

//mem�ڴ�����趨,mem��ȫ�����ⲿSRAM����
#define MEM_MNG_START       SRAM_START
#define MEM_MNG_MAX_SIZE		SRAM_SIZE/8*6
#define MEM_MNG_BASE        __attribute__((at(MEM_MNG_START)))
#define MEM_MNG_SIZE        MEM_MNG_MAX_SIZE         //�ڴ���С
#define MEM_MNG_MAP_SIZE    MEM_MNG_MAX_SIZE/8       //�ڴ��map��С
#define MEM_MNG_MAPBASE     __attribute__((at(MEM_MNG_START + MEM_MNG_SIZE)))


void memmng_memcpy(void *des,void *src,u32 n);
void *memmng_malloc(size_t size );
void memmng_free(void *p );
void memmng_memset(void *p, u8 c, u32 count);
//����sram�ڴ�����ʼ���������ڳ���һ��ʼʹ��
void memmng_init(void);
//����sram�ڴ��ʹ���ʣ�����0-100
u8 memmng_perused(void);
void *memmng_realloc(void *p,size_t size);

void memmng_test(void)  ;
#endif



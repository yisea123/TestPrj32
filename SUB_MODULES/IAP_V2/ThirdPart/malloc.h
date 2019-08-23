#ifndef _MALLOC_H
#define _MALLOC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//�ڴ���� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/7/8
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#ifndef NULL
#define NULL 0
#endif
//����CCM�ķ����ַ//__attribute__((at(0X10000000 + 0x10000 - 0x1000/*59*1024*/))) 
#define RAM_ADDR(X) __attribute__((at(X)))
#define CCM_BASE 0x10000000
#define CCM_SIZE 0x10000
#define CCM_END   CCM_BASE + CCM_SIZE
//MEM3_MAX_SIZE 32K
#define CCM_MEM3BASE    RAM_ADDR(CCM_BASE)
//MEM3_ALLOC_TABLE_SIZE MEM3_MAX_SIZE/32 = 1K
#define CCM_MEM3MAPBASE    RAM_ADDR(CCM_BASE + MEM3_MAX_SIZE)
//3kԤ��
//0x7000 = 28K
#define CCM_USERMEMSIZE 0x7000
#define CCM_USERMEMBASE    RAM_ADDR(CCM_END - CCM_USERMEMSIZE)
//usermem�ļ���offset����
//MAX4096�ֽ� 4K
#define CCM_FLASH_WRITE_BUF    CCM_USERMEMSIZE - 0x1000
//MAX4096�ֽ� 4K
#define CCM_SEND_BUF    CCM_USERMEMSIZE - 0x2000
//MAX 8K
#define CCM_RECV_BUF    CCM_USERMEMSIZE - 0x4000
//MAX 4K
#define CCM_TCP_SERVER    CCM_USERMEMSIZE - 0x5000
//MAX 4K
#define CCM_TCP_IP    CCM_USERMEMSIZE - 0x6000

//���������ڴ��
#define SRAMIN 	0  //�ڲ��ڴ��
#define SRAMEX 	1  //�ⲿ�ڴ��
#define SRAMCCM 2  //CCM�ڴ��(�˲���SRAM����CPU���Է��ʣ�����)

#define SRAMBANK  3 //����֧�ֵ�SRAM����


//mem1�ڴ�����趨,mem1��ȫ�����ڲ�SRAM����
#define MEM1_BLOCK_SIZE	32  			//�ڴ���СΪ32�ֽ�
#define MEM1_MAX_SIZE		100*1024 	//�������ڴ� 110k
#define MEM1_ALLOC_TABLE_SIZE MEM1_MAX_SIZE/MEM1_BLOCK_SIZE  //�ڴ���С

//mem2�ڴ�����趨,mem2�����ⲿSRAM����
#define MEM2_BLOCK_SIZE	32  			//�ڴ���СΪ32�ֽ�
#define MEM2_MAX_SIZE		200*1024 	//�������ڴ� 200k
#define MEM2_ALLOC_TABLE_SIZE MEM2_MAX_SIZE/MEM2_BLOCK_SIZE  //�ڴ���С

//mem3�ڴ�����趨,mem3����CCM,���ڹ���CCM(�ر�ע��,�ⲿ��SRAM,��CPU���Է���)
#define MEM3_BLOCK_SIZE	32  			//�ڴ���СΪ32�ֽ�
#define MEM3_MAX_SIZE		32*1024 	//�������ڴ� 32k
#define MEM3_ALLOC_TABLE_SIZE MEM3_MAX_SIZE/MEM3_BLOCK_SIZE  //�ڴ���С1024

//�ڴ���������
struct _m_mallco_dev
{
	void (*init)(u8);  		//��ʼ��
	u8 (*perused)(u8); 		//�ڴ�ʹ����
	u8 *membase[SRAMBANK]; //�ڴ��,����SRAMBANK��������ڴ�
	u16 *memmap[SRAMBANK];  //�ڴ�״̬��
	u8 memrdy[SRAMBANK];   //�ڴ�����Ƿ����
};
extern struct _m_mallco_dev mallco_dev;  //��malloc.c���涨��
//extern u8 usermem[CCM_USERMEMSIZE];

void mymemset(void *s,u8 c,u32 count);	 //�����ڴ�
void mymemcpy(void *des,void *src,u32 n);//�����ڴ�     
void mymem_init(u8 memx);					 //�ڴ�����ʼ������(��/�ڲ�����)
u32 mymem_malloc(u8 memx,u32 size);		 //�ڴ����(�ڲ�����)
u8 mymem_free(u8 memx,u32 offset);		 //�ڴ��ͷ�(�ڲ�����)
u8 mem_perused(u8 memx);				 //����ڴ�ʹ����(��/�ڲ�����) 
////////////////////////////////////////////////////////////////////////////////
//�û����ú���
void myfree(u8 memx,void *ptr);  			//�ڴ��ͷ�(�ⲿ����)
void *mymalloc(u8 memx,u32 size);			//�ڴ����(�ⲿ����)
void *myrealloc(u8 memx,void *ptr,u32 size);//���·����ڴ�(�ⲿ����)
void *UserMemPtr(u32 offset);
#endif



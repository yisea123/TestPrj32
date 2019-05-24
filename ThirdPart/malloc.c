#include "malloc.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//内存管理 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//内存池(4字节对齐)
//__align(4) u8 mem1base[0];
//__align(4) u8 mem2base[0] __attribute__((at(0x68000000))); //外部SRAM内存池
__align(4) u8 mem3base[MEM3_MAX_SIZE] CCM_MEM3BASE;//__attribute__((at(0x10000000))); //内部CMM内存池

#if USE_MEMMNG == 1u
void* user_mem_ptr_base = NULL;
#else
__align(4) u8 usermem[CCM_USERMEMSIZE] CCM_USERMEMBASE;//CCM_MEM3MAPBASE;//__attribute__((at(0x10000000))); //内部CMM内存池
#endif

//内存管理表
//u16 mem1mapbase[0];													//内部SRAM内存池MAP
//u16 mem2mapbase[0] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));	//外部SRAM内存池MAP
u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] CCM_MEM3MAPBASE;//__attribute__((at(0X10000000+MEM3_MAX_SIZE)));	//内部CCM内存池MAP
//内存管理参数	   
const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};	//内存表大小
const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//内存分块大小
const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};							//内存总大小

//内存管理控制器
struct _m_mallco_dev mallco_dev=
{
	mymem_init,							//内存初始化
	mem_perused,						//内存使用率
	NULL,NULL,mem3base,			//内存池
	NULL,NULL,mem3mapbase,//内存管理状态表
	0,0,0,  		 					//内存管理未就绪
};

//复制内存
//*des:目的地址
//*src:源地址
//n:需要复制的内存长度(字节为单位)
void mymemcpy(void *des,void *src,u32 n)
{
	u8 *xdes = des;
	u8 *xsrc = src;
	while(n--) *xdes++ = *xsrc++;
}

//设置内存
//*s:内存首地址
//c :要设置的值
//count:需要设置的内存大小(字节为单位)
void mymemset(void*s,u8 c,u32 count)
{
	u8 *xs = s;
	while(count--) *xs++=c;
}

//内存管理初始化  
//memx:所属内存块
//u8 *ppppptr = NULL;
void mymem_init(u8 memx)
{
#if USE_MEMMNG == 1u
	if(user_mem_ptr_base == NULL) {
		NEWCH(user_mem_ptr_base, CCM_USERMEMSIZE);
	}
#endif
	
	//NEWCH(ppppptr, 307200);
	
	mymemset(mallco_dev.memmap[memx], 0, memtblsize[memx]*2); //内存状态表清零
	mymemset(mallco_dev.membase[memx], 0, memsize[memx]);	//内存池所有数据清零  
	mallco_dev.memrdy[memx]=1;								//内存管理初始化OK  
}

//获取内存使用率
//memx:所属内存块
//返回值:使用率(0~100)
u8 mem_perused(u8 memx)  
{  
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(mallco_dev.memmap[memx][i])used++; 
    } 
    return (used*100)/(memtblsize[memx]);  
} 

//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 mymem_malloc(u8 memx,u32 size)  
{  
    signed long offset=0;  
    u16 nmemb;	//需要的内存块数  
		u16 cmemb=0;//连续空内存块数
    u32 i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//未初始化,先执行初始化 
    if(size==0)return 0XFFFFFFFF;//不需要分配
    nmemb=size/memblksize[memx];  	//获取需要分配的连续内存块数
    if(size%memblksize[memx])nmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!mallco_dev.memmap[memx][offset])cmemb++;//连续空内存块数增加
		else cmemb=0;								//连续内存块清零
		if(cmemb==nmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  					//标注内存块非空 
            {  
                mallco_dev.memmap[memx][offset+i]=nmemb;  
            }  
            return (offset*memblksize[memx]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  

//释放内存(内部调用) 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 mymem_free(u8 memx,u32 offset)  
{  
	int i;  
  if(!mallco_dev.memrdy[memx])//未初始化,先执行初始化
	{
		mallco_dev.init(memx);    
    return 1;//未初始化  
  }  
  if(offset<memsize[memx])//偏移在内存池内. 
  {  
		int index=offset/memblksize[memx];			//偏移所在内存块号码  
    int nmemb=mallco_dev.memmap[memx][index];	//内存块数量
    for(i=0;i<nmemb;i++)  						//内存块清零
    {  
			mallco_dev.memmap[memx][index+i]=0;  
    }  
    return 0;  
  }else return 2;//偏移超区了.  
}  

//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void myfree(u8 memx,void *ptr)  
{  
	u32 offset;  
    if(ptr==NULL)return;//地址为0.  
 	offset=(u32)ptr-(u32)mallco_dev.membase[memx];  
    mymem_free(memx,offset);//释放内存     
}  

//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u8 memx,u32 size)  
{  
  u32 offset;  									      
	offset=mymem_malloc(memx,size);  	   				   
  if(offset==0XFFFFFFFF)return NULL;  
  else return (void*)((u32)mallco_dev.membase[memx]+offset);  
}  

//重新分配内存(外部调用)
//memx:所属内存块
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(u8 memx,void *ptr,u32 size)  
{  
    u32 offset;  
    offset=mymem_malloc(memx,size);  
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);	//拷贝旧内存内容到新内存   
        myfree(memx,ptr);  											  		//释放旧内存
        return (void*)((u32)mallco_dev.membase[memx]+offset);  				//返回新内存首地址
    }  
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//使用usermem数组
//大小需自己控制，使用时要非常小心
//适用于不变地址，大小的指针类型
//框架专用+


void *UserMemPtr(u32 offset)
{   				   
  if(offset >= CCM_USERMEMSIZE) {
		return NULL;  
	}
  else {
#if USE_MEMMNG == 1u
		return (void*)((u32)user_mem_ptr_base + offset);  
#else
		return (void*)((u32)usermem + offset);  
#endif
	}
}  
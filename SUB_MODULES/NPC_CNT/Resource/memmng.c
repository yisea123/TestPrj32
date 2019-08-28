#include "memmng.h"
#include "sram.h"


__align(32) u8 mem_base[MEM_MNG_SIZE] MEM_MNG_BASE;
u32 map_start_cache = 0;//此变量在系统的内存中

enum MAP_MARK_TYPE{
	MARK_START = 0,
	MARK_OCCUPANCY,
};
//1bit对应1个内存单元8位
//8bit对于1个内存块64位
//起始位标记bit,mem_mapbase[MARK_START],每块至多有一个，且标记都在第一bit,方便检索
//占用标记bit,mem_mapbase[MARK_OCCUPANCY]，1占用
u8 mem_mapbase[2][MEM_MNG_MAP_SIZE] MEM_MNG_MAPBASE;

/** @brief  
  * @param  
  * @retval 
  * @note   
	*	@example
  */
void memmng_memcpy(void *des,void *src,u32 n)
{
	u8 *xdes = des;
	u8 *xsrc = src;
	ASSERT(MEM_MNG_START + MEM_MNG_SIZE - (u32)des >= n);
	ASSERT(MEM_MNG_START + MEM_MNG_SIZE - (u32)src >= n);
	while(n--) *xdes++ = *xsrc++;
}

/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void *memmng_malloc(size_t size ) {
	int i;
	int start = -1;
	size_t num = 0;
	
	for (i = map_start_cache; i < MEM_MNG_MAP_SIZE; i++) {
		if(!mem_mapbase[MARK_OCCUPANCY][i]) { // 为0、说明未分配
			if(-1 == start)
			  start = i;
			
			if((++num<<3) >= size) {
				int j;
				//刷新起始标记
				mem_mapbase[MARK_START][start] = 1;
				//刷新占用标记
				for(j = start; j < i; j++) {
					mem_mapbase[MARK_OCCUPANCY][j] = 0xFF;
				}
				mem_mapbase[MARK_OCCUPANCY][j] = 0xFF>>((num<<3) - size);
				
				if(map_start_cache == start)//保存起始块
				  map_start_cache = j+1;
				
				return (void *)&mem_base[start<<3];
			}
		} else {
			start = -1;
			num = 0;
		}
	}
	ASSERT(1);
	return (void *)0;
}

/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void memmng_free(void *p ) {
	
	u32 map_start = ((u32)p - (u32)mem_base)>>3;
	int i;
	
	if((u32)p < (u32)mem_base)
		return ;
	
	if(!mem_mapbase[MARK_START][map_start])//无起始标记，不必释放
		return;
	
	mem_mapbase[MARK_START][map_start] = 0;//清起始标记
	
	if(map_start_cache > map_start)//保存起始块
		map_start_cache = map_start;
	
	//清除占用标记
	for (i = map_start; i < MEM_MNG_MAP_SIZE; i++) {
		if(mem_mapbase[MARK_OCCUPANCY][i] && !mem_mapbase[MARK_START][i]) {
			mem_mapbase[MARK_OCCUPANCY][i] = 0;
		} else {//清除完毕
			return;
		}
	}
	return;//爆表了
}

/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void memmng_memset(void *p, u8 c, u32 count)
{
	u8 *xs = p;
	while(count--) *xs++=c;
}

/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void memmng_init(void)
{
	int size;
	FSMC_SRAM_Init();
	size = sizeof(mem_mapbase);
	memmng_memset(mem_mapbase, 0, size);
	map_start_cache = 0;
}

/** @brief  获取内存块使用量(字节为单位)
  * @param  
  * @retval 
  * @note   块的使用量
  */
u32 memmng_used(void)  
{  
	size_t used = 0;
  int i;
	for(i = 0; i < MEM_MNG_MAP_SIZE; i++)  
	{
		if(mem_mapbase[MARK_OCCUPANCY][i]) { // 非0、说明已分配
			used++;
		}
	} 
  return (used<<3);  
} 

/** @brief  获取内存块使用率(0~100)
  * @param  
  * @retval 
  * @note   非字节使用率，而是块的使用率，一个块8字节
  */
u8 memmng_perused(void)  
{  
  return (memmng_used()*100)/(MEM_MNG_SIZE);  
} 

/** @brief  获取内存申请数量(0~100)
  * @param  
  * @retval 
  * @note   查询起始标记
  */
u32 memmng_allocd(void)  
{  
  size_t used = 0;
  int i;
	for(i = 0; i < MEM_MNG_MAP_SIZE; i++)  
	{
		if(mem_mapbase[MARK_START][i]) { // 非0、说明已分配
			used++;
		}
	} 
  return (used);  
} 
/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void *memmng_realloc(void *p,size_t size)  
{  
	void *ptr = NULL;
  ptr = memmng_malloc(size);  
  if(!ptr) {
		return NULL;     
	}	else {  									   
		memmng_memcpy(ptr, p, size);	//拷贝旧内存内容到新内存   
		memmng_free(p);  											  		//释放旧内存
		return (ptr);  				//返回新内存首地址
	}  
}

/** @brief  
  * @param  
  * @retval 
  * @note   
  */
void memmng_test(void)  
{  
	#define LEN 0xFF
	u8* ptr = NULL;
	static u32 allocd;
	static u32 used;
	static u8 perused;
	static char onon2 = 0;
//	memmng_init();
	if(onon2) {
		ptr = memmng_malloc(LEN);
		allocd = memmng_allocd();
		used = memmng_used();
		perused = memmng_perused();
		//onon2 = 0;
		memmng_free(ptr);
	}
}


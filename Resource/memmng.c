#include "memmng.h"
#include "sram.h"


__align(32) u8 mem_base[MEM_MNG_SIZE] MEM_MNG_BASE;
u32 map_start_cache = 0;//�˱�����ϵͳ���ڴ���

enum MAP_MARK_TYPE{
	MARK_START = 0,
	MARK_OCCUPANCY,
};
//1bit��Ӧ1���ڴ浥Ԫ8λ
//8bit����1���ڴ��64λ
//��ʼλ���bit,mem_mapbase[MARK_START],ÿ��������һ�����ұ�Ƕ��ڵ�һbit,�������
//ռ�ñ��bit,mem_mapbase[MARK_OCCUPANCY]��1ռ��
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
		if(!mem_mapbase[MARK_OCCUPANCY][i]) { // Ϊ0��˵��δ����
			if(-1 == start)
			  start = i;
			
			if((++num<<3) >= size) {
				int j;
				//ˢ����ʼ���
				mem_mapbase[MARK_START][start] = 1;
				//ˢ��ռ�ñ��
				for(j = start; j < i; j++) {
					mem_mapbase[MARK_OCCUPANCY][j] = 0xFF;
				}
				mem_mapbase[MARK_OCCUPANCY][j] = 0xFF>>((num<<3) - size);
				
				if(map_start_cache == start)//������ʼ��
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
	
	if(!mem_mapbase[MARK_START][map_start])//����ʼ��ǣ������ͷ�
		return;
	
	mem_mapbase[MARK_START][map_start] = 0;//����ʼ���
	
	if(map_start_cache > map_start)//������ʼ��
		map_start_cache = map_start;
	
	//���ռ�ñ��
	for (i = map_start; i < MEM_MNG_MAP_SIZE; i++) {
		if(mem_mapbase[MARK_OCCUPANCY][i] && !mem_mapbase[MARK_START][i]) {
			mem_mapbase[MARK_OCCUPANCY][i] = 0;
		} else {//������
			return;
		}
	}
	return;//������
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

/** @brief  ��ȡ�ڴ��ʹ����(�ֽ�Ϊ��λ)
  * @param  
  * @retval 
  * @note   ���ʹ����
  */
u32 memmng_used(void)  
{  
	size_t used = 0;
  int i;
	for(i = 0; i < MEM_MNG_MAP_SIZE; i++)  
	{
		if(mem_mapbase[MARK_OCCUPANCY][i]) { // ��0��˵���ѷ���
			used++;
		}
	} 
  return (used<<3);  
} 

/** @brief  ��ȡ�ڴ��ʹ����(0~100)
  * @param  
  * @retval 
  * @note   ���ֽ�ʹ���ʣ����ǿ��ʹ���ʣ�һ����8�ֽ�
  */
u8 memmng_perused(void)  
{  
  return (memmng_used()*100)/(MEM_MNG_SIZE);  
} 

/** @brief  ��ȡ�ڴ���������(0~100)
  * @param  
  * @retval 
  * @note   ��ѯ��ʼ���
  */
u32 memmng_allocd(void)  
{  
  size_t used = 0;
  int i;
	for(i = 0; i < MEM_MNG_MAP_SIZE; i++)  
	{
		if(mem_mapbase[MARK_START][i]) { // ��0��˵���ѷ���
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
		memmng_memcpy(ptr, p, size);	//�������ڴ����ݵ����ڴ�   
		memmng_free(p);  											  		//�ͷž��ڴ�
		return (ptr);  				//�������ڴ��׵�ַ
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


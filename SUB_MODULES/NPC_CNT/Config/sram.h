
#ifndef  _SRAM_
#define  _SRAM_
#include "cdv_include.h" 

#define SRAM_START    ((u32)(0x68000000))
#define SRAM_SIZE     ((u32)(0x100000))

void FSMC_SRAM_Init(void);

void fsmc_sram_test(void);
#endif
#ifndef  _CDV_DG_UART_
#define  _CDV_DG_UART_	 
//#include "sys.h"	 								  
#include "cdv_include.h"	  	

//如果想串口中断接收，设置EN_USART2_RX为1，否则设置为0
#define EN_UART5_RX 	1			//0,不接收;1,接收.
extern u8 DG_UART5_RX_OVER;
extern u32 DG_UART5_RX_CNT;														 
void DG_UART5_Init(u32 bound);
void DMA_usart5Send(CDV_INT32U mar,CDV_INT16U ndtr);
void DG_UART5_Send_Data(u8 *buf,u32 len);
void DG_UART5_Receive_Data(u8 *buf,u32 *len); 
   
#endif

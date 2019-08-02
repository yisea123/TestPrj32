
#include "_include.h"



__INLINE static void ConfigInit(void) {
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	GpioInit();
	
	Init74HC595();
	
	USART_Configuration();
}


u8 send_buf[] = {0x02, 0x02, 0x00, 0x00, 0x00, 0x0E, 0xF9, 0xFD};

int main (void) {
	static u32 led_ticks = 0;
	//static u8* send_buf = NULL;
	//static u16 send_len = 0;
	static u8* recv_buf = NULL;
	static u16 recv_len = 0;
	
  if (SysTick_Config (SystemCoreClock / 1000)) { /* Setup SysTick for 1 msec interrupts */
    ;                                            /* Handle Error */
    while (1);
  }
	
  ConfigInit();                                  /* configure the LEDs */                            
 
  while(1) {
		// RUN LED
		if(CalcCount(sys_ticks, led_ticks)>1000)
		{
			//LED1 =~LED1;
			LED2 =~LED2;
			//LED3 =~LED3;
			led_ticks = sys_ticks;
		}
		
		// USART2
		USARTTR(send_buf,8,recv_buf,&recv_len,2);
		
  }
}

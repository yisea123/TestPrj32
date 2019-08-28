
#include "_include.h"



__INLINE static void ConfigInit(void) {
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	InitGpio();
	
	Init74HC595();
	
	USART_Configuration();
	
	TIM4_Count_Init(5-1,1-1);
	
	TIM8_PWM_Init(2800-1, 60000-1);
	
}


u8 send_buf[] = {0x02, 0x02, 0x00, 0x00, 0x00, 0x0E, 0xF9, 0xFD};

	
int main (void) {
	static u32 led_ticks = 0;
	//static u8* send_buf = NULL;
	//static u16 send_len = 0;
	//static u8* recv_buf = NULL;
	//static u16 recv_len = 0;
	int ret = 0;
	
  if (SysTick_Config (SystemCoreClock / 10000)) { /* Setup SysTick for 0.1 msec interrupts */
    ;                                            /* Handle Error */
    while (1);
  }
	
  ConfigInit();                                  /* configure the LEDs */                            
 
  while(1) {
		// RUN LED
		if(CalcCount(sys_ticks, led_ticks)>10000)
		{
			led_ticks = sys_ticks;
//			LED2 =~LED2;
//			LED3 =~LED3;
		}

    USARTRT(CmdParse , 4);
		// host
//		Cascade_Host_Stat();
		// slave
		//if(0 != CascadeModbus_Map_Stat())
//		ret = CascadeModbus_Map_Stat2();
//		if(-1 ==ret)
//			LED3 = LED_ON;
//		else if(1==ret)
//			LED3 = LED_OFF;
		
		
  }
}


#include "_include.h"

extern uint16_t dma_buf[13];

__INLINE static void ConfigInit(void) {
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	
	InitGpio();
	
	Init74HC595();
	
	USART_Configuration();
	
	//TIM8_PWM_Init(2800-1, 60000-1);
	TIM8_PWM_Init(1-1, 60000-1);// z置为0，不会有更新中断，
	
	TIM4_Count_Init(2-1,1-1);
	
	TIM1_Init(28000-1, 60000-1);
	
	
//	DMA_Enable(DMA2_Stream1,(u32)dma_buf,sizeof(dma_buf)/2);    //开始一次DMA传输！	  
//	TIM8->EGR = 1;//产生更新事件
//	TIM_Cmd(TIM8, ENABLE);
  StartPWM();
//	TIM8->ARR = 2800;
	TIM8->EGR = 1;
  
	TIM_Cmd(TIM8, ENABLE);
}


u8 send_buf[] = {0x02, 0x02, 0x00, 0x00, 0x00, 0x0E, 0xF9, 0xFD};

	int on = 0;
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
if(on){
	StartPWM();
	on = 0;
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

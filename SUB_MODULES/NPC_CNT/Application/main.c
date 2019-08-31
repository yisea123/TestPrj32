
#include "_include.h"

extern uint16_t dma_buf[13];

__INLINE static void ConfigInit(void) {
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	InitGpio();
	
	Init74HC595();
	
	USART_Configuration();
	
	//TIM8_PWM_Init(2800-1, 60000-1);
	TIM8_PWM_Init(1-1, 60000-1);
	
	TIM4_Count_Init(2-1,1-1);
	
	TIM1_Init(28000-1, 60000-1);
	
//	TIM_Cmd(TIM8, DISABLE);
//	TIM_SetCounter(TIM8, 0);
//	DMA_Enable(DMA2_Stream1,(u32)dma_buf,sizeof(dma_buf)/2);    //��ʼһ��DMA���䣡	  
//	TIM8->EGR = 1;//���������¼�
//	TIM_Cmd(TIM8, ENABLE);
//	 TIM_CtrlPWMOutputs(TIM8, ENABLE); 
StartPWM();
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
	TIM_Cmd(TIM8, DISABLE);
	DMA_Enable(DMA2_Stream1,(u32)dma_buf,sizeof(dma_buf)/2);    //��ʼһ��DMA���䣡	��dma�м�������ndtr - 1��
  TIM8->ARR = 2800;//���ڴ���Ӱ�ӼĴ�����������Ҫ��ǰ��ֵ��Ӧ���������һ���������飬�����һ���ᱻ����
  TIM8->EGR = 1;//���������¼� ��dma�м�����-- ����arr��ֵ���䵽Ӱ�ӼĴ��������������ã��������arr��0�����µ�Ӱ�ӼĴ�������0���������á�ͬʱ����һ��dma���䵽arr��ͬʱ�����ظ��Ĵ���������pscҲ����µ�Ӱ�ӼĴ��������м�������0��ʼ����Щ���Ǹ����¼��Ĺ���
	on = 0;
	TIM_Cmd(TIM8, ENABLE);
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

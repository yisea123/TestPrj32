
#include "_include.h"

extern uint16_t dma_buf[200];
//�൱������
#define MIN_ARR 1
#define MAX_ARR 2800
//�൱���ٶ�
#define MIN_HZ 1/MAX_ARR
#define MAX_HZ 1/MIN_ARR
// st start arr
// et end arr
// n  num
void CURVE_K(u16* arr, u16 n, u16 st, u16 et) {
	u16 i;
	
	for(i = 0; i < n; i++) {
		arr[i]= (st*et*n) / (et*n + i*st - i*et);
	}
}











__INLINE static void ConfigInit(void) {
	CURVE_K(dma_buf, 199, 2800, 200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	
	InitGpio();
	
	Init74HC595();
	
	USART_Configuration();
	
	//TIM8_PWM_Init(2800-1, 60000-1);
	TIM8_PWM_Init(1-1, 60000-1);// ��Ϊ0��ֹͣpwm���䣬���Ҳ����и����жϣ�
	
	TIM4_Count_Init(2-1,1-1);
	
	//���updateʱ�䣺168000000HZ / 60000(��Ƶ) / 28000 ���Ƚϣ� = 1 / 10 HZ -��10s
	//���벶����С�ֱ���ʱ�䣺168000000HZ / 60000(��Ƶ) / 1 ���Ƚϣ� = 2800 HZ -��0.357ms
	TIM1_Init(28000-1, 60000-1);
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

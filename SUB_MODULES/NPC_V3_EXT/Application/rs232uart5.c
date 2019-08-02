// #include "sys.h"		    
 #include "rs232uart5.h"	 
// #include "delay.h"
// #include "w25qxx.h"
// #include "key.h"
 	 
//extern u32 FLASH_SIZE;
//#if EN_USART4_RX   		//���ʹ���˽���   	  
//���ջ����� 	
u8 DG_UART5_RX_BUF[32];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u32 DG_UART5_RX_CNT=0; 
u8 DG_UART5_RX_OVER;
u8 DG_Receive_Data[16];
u32 DG_Receive_Len;
//**********************************************
//
//**********************************************
void UART5_IRQHandler(void)
{                                    /*�˳��ж�*/	
	CDV_INT08U Res;//, len = 0;
//	OS_ERR err;
   // OSTmrStart(&tmr2,&err);                                 /*������ʱ�� 1*/
	OSIntEnter();                                           /*�����ж�*/
  tm2Re = 0;
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)   /*�����ж�()*/
	{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);
		Res =USART_ReceiveData(UART5);                       /*(USART2->DR);	//��ȡ���յ�������*/
		switch(g_cdvStat){
			default:
				WIFI_RX_BUF_ADD_CHAR(Res);
//					if('\n' == Res)//��ipdָ���л���0d0a
//						if(g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos]>2) {
//						if( '\r' == g_wifiRx.QUEUE.rxBuf[g_wifiRx.rxPos][g_wifiRx.QUEUE.rxLen[g_wifiRx.rxPos] -2]) {
//							WIFI_RX_QUEUE_SELF_ADD;
//							if(WIFI_RX_HAD) {
//								OSTaskResume((OS_TCB*)&UsartRecvTaskTCB,&err);
//							}
//						}
//					}
				break;
		}
  }
	
	OSIntExit();    	                                      /*�˳��ж�*/

} 
//#endif										 
//��ʼ��IO ����5
//bound:������	  
void DG_UART5_Init(u32 bound)
{  	 
	
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIODʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);//ʹ��USART2ʱ��
	
  //����2���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource12,GPIO_AF_UART5); //GPIOD5����ΪUART2
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5); //GPIOD6����ΪUART2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOD5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
  //RX
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD6
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
//	//GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; //���츴�����
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //
	
   //UART2��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART5, &USART_InitStructure); //��ʼ������2
	
  USART_Cmd(UART5, ENABLE);  //ʹ�ܴ��� 2
	
	USART_ClearFlag(UART5, USART_FLAG_TC);
	
#if EN_UART5_RX	
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//���������ж�

	//Usart2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//�����ȼ�4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif	
  DMA_Config(DMA1_Stream7,DMA_Channel_4,(CDV_INT32U)&UART5->DR,(CDV_INT32U)0,0);
	USART_DMACmd(UART5,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����  
}

void DMA_usart5Send(CDV_INT32U mar,CDV_INT16U ndtr){
	DMA_MemoryTargetConfig(DMA1_Stream7, mar,DMA_Memory_0);	
	DMA_Enable(DMA1_Stream7,ndtr);    //��ʼһ��DMA���䣡	  
  
  while(DMA_GetFlagStatus(DMA1_Stream7,DMA_FLAG_TCIF7)==RESET);	
	
	DMA_ClearFlag(DMA1_Stream7,DMA_FLAG_TCIF7);
}
//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void DG_UART5_Send_Data(u8 *buf,u32 len)
{
	DMA_usart5Send((CDV_INT32U)buf, len);
//	u16 t;
//  for(t=0;t<len;t++)		//ѭ����������
//	{
//	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
//    USART_SendData(UART5,buf[t]); //��������
//	}	 
//	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		 
}
//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void DG_UART5_Send_Data11111111(u8 *buf,u32 len)
{
	u16 t;
  for(t=0;t<len;t++)		//ѭ����������
	{
	  while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
    USART_SendData(UART5,buf[t]); //��������
	}	 
	while(USART_GetFlagStatus(UART5,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		 
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void DG_UART5_Receive_Data(u8 *buf,u32 *len)
{
	u32 rxlen=DG_UART5_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(20);		//�ȴ�20ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==DG_UART5_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=DG_UART5_RX_BUF[i];	
			DG_UART5_RX_BUF[i]=0;
		}		
		*len=DG_UART5_RX_CNT;	//��¼�������ݳ���
		DG_UART5_RX_CNT=0;		//����
		DG_UART5_RX_OVER=1;   //�������
	}
}


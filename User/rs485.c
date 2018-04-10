// #include "sys.h"		    
 #include "rs485.h"	 
// #include "delay.h"
// #include "w25qxx.h"
// #include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//RS485���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/7
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
extern u32 FLASH_SIZE;
#if EN_UART4_RX   		//���ʹ���˽���   	  
//���ջ����� 	
u8 RS485_RX_BUF[16];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
u32 RS485_RX_CNT=0; 
u32 FPGA_CODE_SIZE=0;//��¼����ĳ���
u8 Rec_Over=0;
const u32 FPGA1_CODE_ADD=0x700000;
const u32 FPGA2_CODE_ADD=0x755000;//FPGA�ڶ�������洢��ַ��340k��ʼ
//u16 i=0;	
void UART4_IRQHandler(void)
{
	u8 res;
	OSIntEnter();
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)//���յ�����
	{	 	
		   Rec_Over=0;
	    res =USART_ReceiveData(UART4);//;��ȡ���յ�������UART4->DR
		RS485_RX_BUF[RS485_RX_CNT]=res;
        RS485_RX_CNT=RS485_RX_CNT+1;
        if(RS485_RX_CNT > 15)
		   RS485_RX_CNT = 15;			
	}  				
  OSIntExit();    	
} 
#endif										 
//��ʼ��IO ����4
//bound:������	  
void RS485_Init(u32 bound)
{  	 
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOH,ENABLE); //ʹ��GPIOH  GPIOCʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��USART4ʱ��
	
  //����4���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOC10����ΪUART4
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOC11����ΪUART4
	
	//UART4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOC10��GPIOC11
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��
	
	//PH15���������485ģʽ����  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15; //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOH,&GPIO_InitStructure); //��ʼ��PH15
	

   //UART4��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b  ;//�ֳ�Ϊ8λ���ݸ�ʽ()
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART4, &USART_InitStructure); //��ʼ������4
	
  USART_Cmd(UART4, ENABLE);  //ʹ�ܴ��� 4
	
	USART_ClearFlag(UART4, USART_FLAG_TC);
	
#if EN_UART4_RX	
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//���������ж�

	//Usart6 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�4
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����

#endif	
	
	RS485_TX_ENABLE;				//Ĭ��Ϊ����ģʽ	
	DMA_Config(DMA1_Stream4,DMA_Channel_4,(u32)&UART4->DR,(u32)0,0);
	USART_DMACmd(UART4,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA����  
}
void DMA_usart4Send(u32 mar,u16 ndtr){
//	OS_ERR err;
//	DMA_MemoryTargetConfig(DMA1_Stream4, mar,DMA_Memory_0);	
//	DMA_Enable(DMA1_Stream4,ndtr);    //��ʼһ��DMA���䣡	  
//  
//  while(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)==RESET);	
//	OSTimeDlyHMSM(0,0,0,1,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ	
//	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
	
	/////////////
	DMA_MemoryTargetConfig(DMA1_Stream4, mar,DMA_Memory_0);	
	
	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
	USART_ClearFlag(UART4, USART_FLAG_TC);
//	USART_ClearFlag(UART4, USART_FLAG_RXNE);
	
	DMA_Enable(DMA1_Stream4,ndtr);    //��ʼһ��DMA���䣡	  
  
  
  while(DMA_GetFlagStatus(DMA1_Stream4,DMA_FLAG_TCIF4)==RESET);	
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET);
//	DMA_ClearFlag(DMA1_Stream4,DMA_FLAG_TCIF4);
//	USART_ClearFlag(UART4, USART_FLAG_TC);
}

void USART4_Send_Data(u8 *buf,u32 len)
{
//	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);//���������ж�
	RS485_TX_ENABLE;	
	RS485_RX_CNT=0;		//����
	
	DMA_usart4Send((u32)buf, len);	
//	USART_ClearFlag(UART4, USART_FLAG_RXNE);
  RS485_TX_DISABLE;				//����Ϊ����ģʽ		
	
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//���������ж�
//	USART_ClearFlag(UART4, USART_FLAG_RXNE);
}
//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u16 len)
{
	u16 t;
	RS485_TX_ENABLE;			//����Ϊ����ģʽ
	RS485_RX_CNT=0;	  
  for(t=0;t<len;t++)		//ѭ����������
	{
	  while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET){}; //�ȴ����ͽ���		
    USART_SendData(UART4,buf[t]); //��������
		//buf[t]=0;
		//if(RS485_RX_CNT>0)
		//	RS485_RX_CNT=RS485_RX_CNT-1;
	}	 
	while(USART_GetFlagStatus(UART4,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
	
	RS485_TX_DISABLE;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	
	u32 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	//delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
			RS485_RX_BUF[i]=0;
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
		Rec_Over=1;
	}
}

u32 uart4len=0;
u8 UART4_Receive_Data2(u8 *buf,u32 *len)
{

	  u8 i=0;
		*len=0;				//Ĭ��Ϊ0
		if(RS485_RX_CNT!=0)
		{
			 if(RS485_RX_CNT!=uart4len)
			 {	 
					uart4len=RS485_RX_CNT;
				  return 0;
			 }
       else
       {
					for(i=0;i<uart4len;i++)
					{
						buf[i]=RS485_RX_BUF[i];	
						RS485_RX_BUF[i]=0;
					}		
					*len=uart4len;	//��¼�������ݳ���
					RS485_RX_CNT=0;		//����
					uart4len=0;
					Rec_Over=1;
					return 1;
			 }				 
		}	
		else
		{
			return 0;
		}	
}

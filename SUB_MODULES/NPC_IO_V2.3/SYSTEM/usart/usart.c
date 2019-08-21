#include "usart.h"	
//#include "user.h"
//#include "sys.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"					//ucos ʹ��	  
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//��ʢ����
//�޸�����:2014/1/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ���˺�ʢ�������޹�˾
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮������������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
//V1.5�޸�˵��
//1,�����˶�UCOSII��֧��
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); 
  USART_SendData(USART2,(uint8_t)ch);   
	return ch;
}
#endif 

 
#if EN_USART2_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u8 USART2_RX_BUF[200];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART2_RX_CNT =0;
u8 USART2_REC_OVER;
u16 USART2_DMA_TX_Buf[adc3_value_lens]={0}; //ADC3 DMA��������
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u8 USART_RX_STA=0;       //����״̬���	  
u8  USART_RX_NUM=0;//���ռ���ֵ
u16 times=0;
u8 len;	
u8 bufbuf[8]={0xA5,'C','4',4,0,0,0,'C'+'4'+4};
u8 buf[8]={0xA5,'C','4',4,0,0,0,'C'+'4'+4};	//Ĭ���л�2״̬
u8 USART_RX_DATA1;
//void USART2_Send(u8 *buf,u16 len);
//��ʼ��IO ����2  bound:������

//DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ���/�洢������ģʽ
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
//chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
//par:�����ַ
//mar:�洢����ַ
//ndtr:���ݴ�����  

//DMA_Config(DMA2_Stream7,DMA_Channel_4,(u32)&USART1->DR,(u32)SendBuff,SEND_BUF_SIZE);//DMA2,STEAM7,CH4,����Ϊ����1,�洢��ΪSendBuff,����Ϊ:SEND_BUF_SIZE.
void DMA_Config1(DMA_Stream_TypeDef *DMA_Streamx,CDV_INT32U chx,CDV_INT32U par,CDV_INT32U mar,CDV_INT16U ndtr)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	USART_DMACmd(USART2 , USART_DMAReq_Tx, ENABLE);
  /* Enable USART2 DMA TX request */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  DMA_DeInit(DMA1_Channel7);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&USART2->DR);//DMA�����ַ
  DMA_InitStructure.DMA_MemoryBaseAddr =  (uint32_t)USART2_DMA_TX_Buf;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = 0;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel7, &DMA_InitStructure);
  DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);
} 
 void DMA_ConfigDir(DMA_Channel_TypeDef *DMA_Channelx,u32 par,u32 mar,u16 ndtr,u32 dir)
{ 
 
	DMA_InitTypeDef  DMA_InitStructure;
	
	if((u32)DMA_Channelx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
		
	}else 
	{
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
  DMA_DeInit(DMA_Channelx);
	
	//while (DMA_GetCmdStatus(DMA_Channelx) != DISABLE){}//�ȴ�DMA������ 
	
  /* ���� DMA Stream */
  //DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
  //DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_MemoryBaseAddr = mar;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = dir;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�е����ȼ�
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA_Channelx, &DMA_InitStructure);//��ʼ��DMA Stream
	

} 
//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void DMA_Enable(DMA_Channel_TypeDef *DMA_Channelx,u32 mar,u16 ndtr)
{
 
	DMA_Cmd(DMA_Channelx, DISABLE);                      //�ر�DMA���� 
	DMA_Channelx->CNDTR = ndtr;                          // �����ڴ泤��
  DMA_Channelx->CMAR = mar;                            // �����ڴ��ַ
	DMA_SetCurrDataCounter(DMA_Channelx,ndtr);          //���ݴ�����  
	DMA_Cmd(DMA_Channelx, ENABLE); 
}

/**
  *USART2����
  */
int DMA_usart2Send(u32 mar,u16 ndtr){
	DMA_ClearFlag(DMA1_FLAG_TC7);
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	
	DMA_Enable(DMA1_Channel7,mar,ndtr);    //��ʼһ��DMA���䣡	  
  
	while(DMA_GetFlagStatus(DMA1_FLAG_TC7)==RESET) {};
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET){};
} 

void uart2_init(u32 bound)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//�ڶ������򿪴���ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);   //GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_AFIO,ENABLE);  	//ʹ��USART2ʱ��
	USART_DeInit(USART2);  //��λ����2
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				 //PC5�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//����������������   ����2 PA2	  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2

	//����2  PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��PA3

  RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//��λ����2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//ֹͣ��λ
   //��һ����USART ��ʼ������
	
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	USART_ClearFlag(USART2, USART_FLAG_TC);
#if EN_USART2_RX		  //���ʹ���˽���  
   	//���岽���ж����ȼ�����
    //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   	//���Ĳ��������ж�ʹ��
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
#endif
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
  RS485_TX_EN=0;			//Ĭ��Ϊ����ģʽ
//	DMA_Config1(DMA1_Stream6,DMA_Channel_4,(u32)&USART2->DR,(u32)Modbus_TX_buf,Modbus_TX_len);
//	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���2��DMA����
//DMA
	DMA_ConfigDir(DMA1_Channel7,(u32)&USART2->DR,(u32)0,0, DMA_DIR_PeripheralDST);//����DMA����

	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  //ʹ�ܴ���1��DMA���� 
	

}

//��ʼ��IO ����1 
//bound:������
void uart1_init(u32 bound){
    
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	//ʹ��USART1��GPIOAʱ��
 //	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1)
	USART_DeInit(USART1);  //��λ����1
 //USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9
 
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

  //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������
	USART_ClearFlag(USART1, USART_FLAG_TC);
#if EN_USART1_RX		  //���ʹ���˽���  
   //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
   
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	
#endif
    USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);//(USART1->DR);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
} 
#endif	
//�������������жϷ������
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	u8 Res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		Res =USART_ReceiveData(USART2);//(USART1->DR);	//��ȡ���յ�������
		if (USART2_RX_CNT<100){
		USART2_RX_BUF[USART2_RX_CNT] = Res;
    USART2_RX_CNT++;
		}
		
		Modbus_timeout_cnt=0;			
		Modbus_recevie_flag=1;
		
    USART_ClearFlag(USART2, USART_FLAG_RXNE);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		
  } 
  
#ifdef OS_TICKS_PER_SEC	 	//���ʱ�ӽ�����������,˵��Ҫʹ��ucosII��.
	OSIntExit();  											 
#endif
}

/******************��ȡ��������*******************/
void Uart2_Receive_Data(u8 *buf, u8 *len)
{
	u8 t;
	u32 rxlen=USART2_RX_CNT;
	u8* RxBuf = NULL;
	u16 data;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==USART2_RX_CNT&&rxlen) //���յ�������,�ҽ��������
	{
		data=getCRC16(USART2_RX_BUF,USART2_RX_CNT-2);
		if((USART2_RX_BUF[USART2_RX_CNT-2]==(data & 0x00ff)) && (USART2_RX_BUF[USART2_RX_CNT-1]==((data >> 8) & 0x00ff))) {
		for(i=0;i<rxlen;i++)
			{
				buf[i]=USART2_RX_BUF[i];	
				USART2_RX_BUF[i]=0;
			}		
			
		 }
		else{
				memset(USART2_RX_BUF,0,USART2_RX_CNT*sizeof(CDV_INT08U));
				USART2_RX_CNT=0;
		}
		*len=USART2_RX_CNT;	//��¼�������ݳ���
		USART2_RX_CNT=0;		//����
		USART2_REC_OVER=1;
	 }
	
//		for(i=0;i<1;i++)
//		{ 
//			if(USART2_RX_BUF[0]== 0x00&&USART2_RX_BUF[1]== 0x00&&USART2_RX_BUF[2]== 0x00&&USART2_RX_BUF[3]== 0x00&&USART2_RX_BUF[4]== 0x00&&USART2_RX_BUF[5]==(USART2_RX_CNT-6))
//			{
//				data=getCRC16(&USART2_RX_BUF[6],USART2_RX_CNT-8);
//	      if((USART2_RX_BUF[USART2_RX_CNT-2]==(data & 0x00ff)) && (USART2_RX_BUF[USART2_RX_CNT-1]==((data >> 8) & 0x00ff)))
//		
//				OnlineCmdCache(&USART4_RX_BUF[6], USART4_RX_CNT-6,4);
//			}
//		}	
		
	
//	if(2==USART_RX_STA) //�жϽ������
//	{	
//		len= USART_RX_BUF[0]+USART_RX_BUF[1]+USART_RX_BUF[2]+USART_RX_BUF[3]+USART_RX_BUF[4];
//		if(len==USART_RX_BUF[6]) //�ۼ�У��
//		{
//			if(0x01==USART_RX_BUF[5]) //IDΪ1 �л�2 ϵͳ��ʼ��
//			{
//				if('4'==USART_RX_BUF[1])
//				{
//					for(t=1;t<8;t++)
//					{
//						bufbuf[t]=USART_RX_BUF[t-1];
//						USART_RX_DATA1 = USART_RX_BUF[1];//��ȡ��������������ֱ��С�1��~��8��
//						USART_RX_STA=0;
//					}
//				}
//			}
//			
//			if(USART_RX_BUF[5]==bufbuf[6])//��ID��ͬ����������
//			{
//				USART_RX_STA=0;//���ձ�����
//			}
//			else if(USART_RX_BUF[5] == (bufbuf[6]+1) || (USART_RX_BUF[5]==0 && 0xff==bufbuf[6]))//��ID�����ȷ���򱣴�
//			{
//				for(t=1;t<8;t++)
//				{
//					bufbuf[t]=USART_RX_BUF[t-1];
//					USART_RX_DATA1 = USART_RX_BUF[1];//��ȡ��������������ֱ��С�1��~��8��
//				}
//			}
//			else
//			{
//				USART_RX_STA=0;//���ձ�����
//			}				
//			
//		}
//		USART_RX_STA=0;
//	}
}
/******���ڷ���**********/
void uart_send(void)
{
	u8 t;
	for(t=0;t<8;t++)
	{
		USART_SendData(USART2, bufbuf[t]);//�򴮿�2��������
		while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//�ȴ����ͽ���  
	}


}

/**
  * @brief  �����ַ�����CRCֵ
  *  
  * @param  ptr     �ַ�����ָ��
  *         len     �ַ����ĳ���      
  *   
  * @retval CRCֵ
	*
	* @note   
  */

u16 getCRC16(u8 *ptr,u16 len) 
{ 
    CDV_INT08U i; 
    CDV_INT16U crc = 0xFFFF; 
    if(len==0){
        len = 1;
    } 
    while(len--){   
        crc ^= *ptr; 
        for(i=0; i<8; i++){ 
            if(crc&1){ 
                crc >>= 1;  
                crc ^= 0xA001; 
        	}  
        	else{
                crc >>= 1;
        	} 
        }         
        ptr++; 
    } 
    return(crc); 
} 



void USART2_Send(u8 *buf,u16 len)
{
	u16 t;
	if(len==0 || len >100)
	{
		len = 1;
		return;
	}
	RS485_TX_EN=1;
	DMA_usart2Send((u32)buf,len);
//  for(t=0;t<len;t++)		//ѭ����������
//	{ 
//    while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {};  //�ȴ����ͽ���
//    USART_SendData(USART2,buf[t]);  //��������
//	}
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET) {};  //�ȴ����ͽ���
//	delay_us(1);	
	RS485_TX_EN=0;
 //	DMA_usart1Send((CDV_INT32U)buf, len);
 }

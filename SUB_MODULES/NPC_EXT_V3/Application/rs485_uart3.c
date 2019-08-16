	    
 #include "rs485_uart3.h"	 
 //#include "delay.h"
 
// #include "os.h"	 
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
#if EN_USART3_RX   		//���ʹ���˽���   	  

#endif										 

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void USART3_Send(u8 *buf,u32 len)
{
	USART3_TX_ENABLE;	
	USART3_RX_CNT=0;		//����
	DMA_usart3Send((u32)buf, len);	
USART3_TX_DISABLE;				//����Ϊ����ģʽ		
}


//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void USART3_Send_Data111111111(u8 *buf,u32 len)
{
	u16 t;
	USART3_TX_ENABLE;			//����Ϊ����ģʽ
	//delay_ms(10);
//	OS_CRITICAL_ENTER();
  for(t=0;t<len;t++)		//ѭ����������
	{
	  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���		
      USART_SendData(USART3,buf[t]); //��������
		//buf[t]=0;
		//if(RS485_RX_CNT>0)
		//	RS485_RX_CNT=RS485_RX_CNT-1;
	}	
//   OS_CRITICAL_EXIT();                           //�˳��ٽ���	
	while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET) {}; //�ȴ����ͽ���		
//	RS485_RX_CNT=0;	  
	//delay_ms(10);
	USART3_TX_DISABLE;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void USART3_Receive_Data(u8 *buf,u32 *len)
{
	u32 rxlen=USART3_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(20);		//�ȴ�20ms,��������20msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==USART3_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=USART3_RX_BUF[i];	
			USART3_RX_BUF[i]=0;
		}		
		*len=USART3_RX_CNT;	//��¼�������ݳ���
		USART3_RX_CNT=0;		//����
		USART3_REC_OVER=1;
	}
}
u8 USART3_Receive_Data2(u8 *buf,u32 *len)
{
//	u32 rxlen=USART3_RX_CNT;
//	u8 i=0;
//	*len=0;				//Ĭ��Ϊ0
//	delay_ms(50);		//�ȴ�50ms,��������20msû�н��յ�һ������,����Ϊ���ս���
//	if(rxlen==USART3_RX_CNT&&rxlen)//���յ�������,�ҽ��������
//	{
//		for(i=0;i<rxlen;i++)
//		{
//			buf[i]=USART3_RX_BUF[i];	
//			USART3_RX_BUF[i]=0;
//		}		
//		*len=USART3_RX_CNT;	//��¼�������ݳ���
//		USART3_RX_CNT=0;		//����
//		USART3_REC_OVER=1;
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}	
	  u8 i=0;
		*len=0;				//Ĭ��Ϊ0
		if(USART3_RX_CNT!=0)
		{
			 if(USART3_RX_CNT!=rxlen)
			 {	 
					rxlen=USART3_RX_CNT;
				  return 0;
			 }
       else
       {
					for(i=0;i<rxlen;i++)
					{
						buf[i]=USART3_RX_BUF[i];	
						USART3_RX_BUF[i]=0;
					}		
					*len=rxlen;	//��¼�������ݳ���
					USART3_RX_CNT=0;		//����
					rxlen=0;
					USART3_REC_OVER=1;
					return 1;
			 }				 
		}	
		else
		{
			return 0;
		}	
}


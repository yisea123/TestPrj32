#ifndef __RS485_H
#define __RS485_H			 	 
#include "cdv_include.h"	 								  
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
	  	
extern u8 RS485_RX_BUF[16]; 		//���ջ���,���64���ֽ�
extern u32 RS485_RX_CNT;   			//���յ������ݳ���
extern u8 DG_UART5_RX_OVER;     //������ɱ��
extern u8 Rec_Over;
//ģʽ����
#define RS485_TX_ENABLE		GPIO_SetBits(GPIOH,GPIO_Pin_15)	//485ģʽ����.0,����;1,����.
#define RS485_TX_DISABLE		GPIO_ResetBits(GPIOH,GPIO_Pin_15)	//485ģʽ����.0,����;1,����.

//����봮���жϽ��գ�����EN_UART4_RXΪ1����������Ϊ0
#define EN_UART4_RX 	1			//0,������;1,����.

														 
void RS485_Init(u32 bound);
void RS485_Send_Data(u8 *buf,u16 len);
void USART4_Send_Data(u8 *buf,u32 len);
void RS485_Receive_Data(u8 *buf,u8 *len); 
u8 UART4_Receive_Data2(u8 *buf,u32 *len);
void RS485_Receive_OVER(u8 *buf,u32 *len);
#endif	   


#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "user.h"
#include "cdv_include.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�û���������	   
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
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  		8  	//�����������ֽ��� 8
#define EN_USART2_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
#define EN_USART1_RX      1	  
#define RS485_TX_EN		PCout(1)	//485ģʽ����.0,����;1,����.
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u8  USART2_RX_BUF[200]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u8  USART_RX_STA;         		//����״̬���	
extern u8  USART_RX_NUM;//���ռ����
extern u8  USART_RX_DATA1;
extern u8	bufbuf[8];
extern u8	buf[8];
extern u16 USART2_RX_CNT;

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void Uart2_Receive_Data(u8 *buf, u8 *len);//��ȡ��������
void uart_send(void);//���ڷ���
u16 getCRC16(u8 *ptr,u16 len);
void USART2_Send(u8 *buf,u16 len);
//void AddTx(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo);
#endif



#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "user.h"
//////////////////////////////////////////////////////////////////////////////////	 
//±¾³ÌĞòÖ»¹©Ñ§Ï°Ê¹ÓÃ£¬Î´¾­×÷ÕßĞí¿É£¬²»µÃÓÃÓÚÆäËüÈÎºÎÓÃÍ¾
//ÓÃ»§Çı¶¯´úÂë	   
//º£Ê¢µç×Ó
//ĞŞ¸ÄÈÕÆÚ:2014/1/9
//°æ±¾£ºV1.0
//°æÈ¨ËùÓĞ£¬µÁ°æ±Ø¾¿¡£
//Copyright(C) ¼ÎĞËº£Ê¢µç×ÓÓĞÏŞ¹«Ë¾
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   
//V1.3ĞŞ¸ÄËµÃ÷ 
//Ö§³ÖÊÊÓ¦²»Í¬ÆµÂÊÏÂµÄ´®¿Ú²¨ÌØÂÊÉèÖÃ.
//¼ÓÈëÁË¶ÔprintfµÄÖ§³Ö
//Ôö¼ÓÁË´®¿Ú½ÓÊÕÃüÁî¹¦ÄÜ.
//ĞŞÕıÁËprintfµÚÒ»¸ö×Ö·û¶ªÊ§µÄbug
//V1.4ĞŞ¸ÄËµÃ÷
//1,ĞŞ¸Ä´®¿Ú³õÊ¼»¯IOµÄbug
//2,ĞŞ¸ÄÁËUSART_RX_STA,Ê¹µÃ´®¿Ú×î´ó½ÓÊÕ×Ö½ÚÊıÎª2µÄ14´Î·½
//3,Ôö¼ÓÁËUSART_REC_LEN,ÓÃÓÚ¶¨Òå´®¿Ú×î´óÔÊĞí½ÓÊÕµÄ×Ö½ÚÊı(²»´óÓÚ2µÄ14´Î·½)
//4,ĞŞ¸ÄÁËEN_USART1_RXµÄÊ¹ÄÜ·½Ê½
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  		8  	//¶¨Òå×î´ó½ÓÊÕ×Ö½ÚÊı 8
#define EN_USART2_RX 			1		//Ê¹ÄÜ£¨1£©/½ûÖ¹£¨0£©´®¿Ú1½ÓÊÕ
#define EN_USART1_RX      1	  
#define RS485_TX_EN		PCout(5)	//485Ä£Ê½¿ØÖÆ.0,½ÓÊÕ;1,·¢ËÍ.
extern u8  USART_RX_BUF[USART_REC_LEN]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û 
extern u8  USART2_RX_BUF[100]; //½ÓÊÕ»º³å,×î´óUSART_REC_LEN¸ö×Ö½Ú.Ä©×Ö½ÚÎª»»ĞĞ·û 
extern u8  USART_RX_STA;         		//½ÓÊÕ×´Ì¬±ê¼Ç	
extern u8  USART_RX_NUM;//½ÓÊÕ¼ÆÊıÖ
extern u8  USART_RX_DATA1;
extern u8	bufbuf[8];
extern u8	buf[8];
extern u16 USART2_RX_CNT;

//Èç¹ûÏë´®¿ÚÖĞ¶Ï½ÓÊÕ£¬Çë²»Òª×¢ÊÍÒÔÏÂºê¶¨Òå
void uart1_init(u32 bound);
void uart2_init(u32 bound);
void Uart2_Receive_Data(u8 *buf, u8 *len);//»ñÈ¡´®¿ÚÊı¾İ
void uart_receive(void);//»ñÈ¡´®¿ÚÊı¾İ
void uart_send(void);//´®¿Ú·¢ËÍ
u16 getCRC16(u8 *ptr,u16 len);
void USART2_Send(u8 *buf,u16 len);
//void AddTx(CDV_INT08U* txBuf, CDV_INT08U txLen, CDV_INT08U uartNo);
#endif



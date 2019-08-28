/**
  ******************************************************************************
  * @file    /cmdParse.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2019-8-3
  * @brief   
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2019 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "cmdParse.h"

/** @brief  �������
  * @param  buf     �������ַ���ָ��
  *         len       �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ڴ��ڽ�����
	*         20160107����Ӵ��ں�
  *         ����ʹ��addtx�ȣ������workermanage�������������ᵼ������
���ֶ�		���ֶ�		�������ݳ���
  */
	
int CmdParse(u8 *rxbuf,u16 rxlen ,u8** rtbuf,u16* rtlen) {
	
	static u8 buf[100];
	static u16 len = 0;
	u16 crc = 0;
	
	switch(rxbuf[0]) {
//		case 'T':
//			switch(rxbuf[1]) {
//				case 'I':// init
//					if(OPT_SUCCESS == CascadeModbus_MapInit(rxbuf+2, rxlen-4)) {
//						u32 val = 0x0000F99F;
//	          WriteToInLed((u8*)&val, 4);
//						len = 5;
//						//NEWCH(buf, len);
//						buf[0] = 'T';
//						buf[1] = 'I';
//						buf[2] = 'S';
//						crc=getCRC16(buf,3);
//	          MemCpy(buf + 3, &crc, 2);
//							
//				  } else {
//						len = 5;
//						//NEWCH(buf, len);
//						buf[0] = 'T';
//						buf[1] = 'I';
//						buf[2] = 'F';
//						crc=getCRC16(buf,3);
//	          MemCpy(buf + 3, &crc, 2);
//					}
//	
//					*rtbuf = buf;
//					*rtlen = len;
//					break;
//				case 'R':
//					Restart();
//				
//					break;
//				case 'W':
//					if(LED3 == LED_ON || 0 != Cascade_Host_Transfer(rxbuf+2, rxlen-4, rtbuf, rtlen))
//					{
//						len = 5;
//						//NEWCH(buf, len);
//						buf[0] = 'T';
//						buf[1] = 'W';
//						buf[2] = 'F';
//						crc=getCRC16(buf,3);
//	          MemCpy(buf + 3, &crc, 2);
//					  *rtbuf = buf;
//					  *rtlen = len;
//					}
//				
//					break;
//				default:
//					break;
//			}
//			break;
//		default:
//			break;
//		
//		
	}
	if(0 == strncmp((char*)rxbuf,"GET DATA",8)){//���ü�صĹ��˺� SET TIME 00
	
		sprintf((char*)buf, "YOUR DATA IS = %s\r\n", rxbuf);
		len = strlen((char*)buf);
	  *rtbuf = buf;
	  *rtlen = len;
					
		}else{
	
		sprintf((char*)buf, "no cmd\r\n");
		len = strlen((char*)buf);
	  *rtbuf = buf;
	  *rtlen = len;		
			
		}
	return 0;
}
//int CmdParse(u8 *rxbuf,u16 rxlen ,u8* rtbuf,u16* rtlen)
//{
//	RET_STATUS ret = OPT_FAILURE;
////	if (len != buf[2] + 3)                                         /*�������ݳ��Ȳ�������*/
////		return ret;
//	
//	
//	
//	
//	switch (buf[0]) {
//		case 0xF1:                                                   /*�����㷢��NPC�ն�*/
//		{
//			                                                           /**/
//			switch (buf[1]) {
//				case 0x10:                                               /*ԭ�·�ָ��*/
//					//ret = NPC1Parse(buf + 4, len - 4, arg);
//				#if USE_16BIT_CMD == 1u
//				  ret = NPC1Parse(buf + 5, len - 5, arg);//ʣ�೤��16bit
//				#else
//				  ret = NPC1Parse(buf + 4, len - 4, arg);
//				#endif
//					break;
//				case 0x11:                                               /*���·�ָ��:ͨ�ô���*/
//					ret = NPC2Parse(buf + 3, len - 3, arg);
//					break;
//				case 0x01:                                               /*��ȡNPC��Ϣ*/
//		      //CascadeCombine(0x00);
//#if USE_CASCADE == 1u
//     			CascadeTableSend(buf, len, arg);
//#endif
//					break;
//				case 0x02:
//					break;
//				case 0x03:
//					break;
//				case 0x04:                                               /*ͨ�ô�������*/
//					ret = UniSerialComSetCmd(buf + 4, len - 4, arg);
//				  if(OPT_SUCCESS != ret)
//					{
//						arg->buf[1] += 0x80;
//					  ResRequest(arg->buf, arg->len, 0,0,arg, RC_CRC);
//					}
//					break;
//				case 0x05:                                               /*���ߵ��Թ���*/
//#if USE_WORKER_DEBUG == 1u
//				WorkerDebugParse(buf + 2 ,len - 2, arg);
//#endif
//					break;
//				case 0x06:
//					break;
//				case 0x07:
//					break;
//				case 0x30:
//#if USE_CASCADE == 1u
//				  NPC_NETParse(buf + 2 ,len - 2, arg);
//#endif
//					break;
//				default:
//					break;
//			}
//		}
//		break;
//		case 0xF2:                                                   /*Ӧ�ò㷢��NPC�ն� */
//		{
//			                                                           /**/
//			switch (buf[1]) {
//				case 0x29:
//				  AlarmQuery( buf + 4 ,len - 4, arg); 
//				  break;
//				case 0x30:
//#if USE_CASCADE == 1u
//				  NPC_NETParse(buf + 2 ,len - 2, arg);
//#endif
//					break;
//				case 0x31:
//					//ResParaRequest(buf + 4, len - 4, 0, 0, arg->uart);
//				  ResRequest(arg->buf, arg->len, 0,0,arg, RC_CRC);
//					AlarmCancel(buf[3]);
//					break;
//				case 0x40:
//					Log_Cmd(arg->buf + 4, arg->len - 4,  arg);
//					break;
//				case 0x07:
//					break;
//				case 0x08:
//					break;
//				case 0x10:
//					ret = YYCCtrlParse1(buf + 2, len - 2, arg);
//					break;
//				case 0x11:                                              
//					ret = NPC1Parse(buf + 4, len - 4, arg);
//					break;
//				case 0x05:                                               /*���ߵ��Թ���*/
//#if USE_WORKER_DEBUG == 1u
//				WorkerDebugParse(buf + 2 ,len - 2, arg);
//#endif
//					break;
//				default:
//					break;
//			}
//			//OnlineParse(buf, len, uart);
//		}
//		break;
//		case 0xF3:
//		{
//#if USE_CENTRALIZEDCONTROL == 1u
//		  switch (buf[1]) {
//				case 0x01://��ϵͳ��Ϣ
//					CentralizedControl_QuerySysInfo(buf, len, arg);
//					break;
//				case 0x02://���豸����
//					CentralizedControl_QueryDevData(buf, len, arg);
//					break;
//				case 0x03://ϵͳ
//					CentralizedControl_SysOperation(buf, len, arg);
//					break;
//				case 0x04://��������
//					CentralizedControl_OTA(buf, len, arg);
//					break;
//				case 0x05://д�豸����
//					CentralizedControl_SetDevData(buf, len, arg);
//					break;
//				default:
//					break;
//			}
//#endif
////			global_clk = CalcTimeMS(GetCdvTimeTick(), global_start);
////			global_cnt2++;
////			if(global_clk > 100) {
////				break;
////			}
//		}
//		break;
//		default:
//		{
//			if (arg->hostid == buf[0])
//			{
//				ModbusParse(buf, len, arg);
//			}			
//			break;
//		}
//	}
//	
//	return ret;
//}


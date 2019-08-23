/**
  ******************************************************************************
  * @file    /cmdParse.c 
  * @author  MMY
  * @version V1.0.0
  * @date    2017-3-23
  * @brief   
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2017 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "cmdParse.h"
	
	
	
	
	
/** @brief  npc1.0���������
  * @param  buf     �������ַ���ָ��
  *         len       �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   
  */
RET_STATUS NPC1Parse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	RET_STATUS ret = OPT_FAILURE;
	switch(buf[0]){//CDV��ַӦ��
		case APP_ID:
//			DebugParse(buf, len, arg->uart);
			break;
		case CDV_ID:
			ret = OnlineParse(buf, len, arg);
			break;
		default:
			break;
	}
	
	return ret;
}

/** @brief  npc2.0���������
  * @param  buf     �������ַ���ָ��
  *         len     �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   npc1���°汾��ʵ��
����ID	�豸��ַ	��Դ���ͻ�������	ǰ�ñ����ֶ�1	ǰ�ñ����ֶ�2	������	�Ĵ���λ��	����λ����	��������λ,�м���|$����������ַ����С�|$����ת1��Ϊ^hsdz$	���ñ����� 	Э��	Э�����
  */
RET_STATUS NPC2Parse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{

	RET_STATUS ret       = OPT_FAILURE;	
	CDV_INT08U native    = CascadeGetNativeNo(); //������
//	CDV_INT08U devAddr   = buf[1];
//	CDV_INT08U res       = buf[2];
//	/*�����ֶ�*/
	CDV_INT08U ftmp1     = buf[3];
//	CDV_INT08U ftmp2     = buf[4];
//	
//	CDV_INT08U fc        = buf[5];            /*������*/
//	CDV_INT08U regPos    = buf[6];            /*�Ĵ���λ��*/
	CDV_INT08U datLen    = buf[7];            /*����λ����*/
//	CDV_INT08U* p_dat    = buf + 8;           /*��������*/
//	/*�����ֶ�*/
//	CDV_INT08U btmp1     = buf[8 + datLen];
	CDV_INT08U prot      = buf[9 + (datLen?(len - 11):4)];   /*Э��*///datLen:0����4B��1��ֵ��len-11��B
//	CDV_INT08U* p_pp     = buf + 10 + datLen; /*Э�����*/


	ASSERT(NULL != buf);
	ASSERT(0 != len);
	
	if (native == buf[0])                     /*�������ID*/
	{
		
		switch(prot){
			case 0x00:                            /*modbus*/
				ret = UniSerialModbusParse(buf, len, arg->uart, arg);
				break;
			case 0x01:
				break;
			default:
				break;
		}
		
	} else {                              /*ת��*/
		
	}
	
	return ret;
}

/** @brief  Ӧ�ò���ƽ���
  * @param  buf     �������ַ���ָ��
  *         len       �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   buf[0] Ϊ����ID
  */
RET_STATUS YYCCtrlParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	CDV_INT08U no , opt, ctrl1, ctrl2, resultLen = 0, result = 0;
	WORKER_STATUS status;
	CDV_INT32U    step;
	RET_STATUS ret = OPT_FAILURE;
	opt = buf[1];
	ctrl1 = buf[2];
	ctrl2 = buf[3];
	switch (opt) {
		case 0x00:                                                   /*�����߿���*/
			ret = ManagerControl((WORKER_STATUS)ctrl1);
	  	if(OPT_FAILURE == ret)
			{
				if(arg->buf)
				arg->buf[1] += 0x80;
			}
	
			break;
		case 0x01:                                                   /*���˿���*/
			ret = WorkerControl(ctrl1, (WORKER_STATUS)ctrl2);
		  if(OPT_FAILURE == ret)
			{
				if(arg->buf)
				arg->buf[1] += 0x80;
			}
			break;
		case 0x10:                                                   /*�����߿���*/
			if (0x00 == ctrl1)
			{
				ret = ManagerQueryStatus(&result);
				resultLen = 1;
			}
			break;
		case 0x11:                                                   /*���˿���*/
			if (0x00 == ctrl2)
			{
				ret = WorkerQueryStatus(ctrl1, &result);
				resultLen = 1;
			}
			else if (0x01 == ctrl2)
			{
				ret = WorkerQueryStep(ctrl1, (CDV_INT16U*)&result);
				resultLen = 1;
			}
			break;
		default:
			break;
		
	}
	ResRequest(arg->buf, arg->len, &result, resultLen, arg);
	
	return ret;
}
/** @brief  �������
  * @param  buf     �������ַ���ָ��
  *         len       �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ڴ��ڽ�����
	*         20160107����Ӵ��ں�
  *         ����ʹ��addtx�ȣ������workermanage�������������ᵼ������
���ֶ�		���ֶ�		�������ݳ���
  */
RET_STATUS CmdParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	//OS_ERR err;
	RET_STATUS ret = OPT_FAILURE;
	
//	if (len != buf[2] + 3)                                         /*�������ݳ��Ȳ�������*/
//		return ret;
	
	switch (buf[0]) {
		case 0x01:                                                   /*�����㷢��NPC�ն�*/
		{
			                                                           /**/
			switch (buf[1]) {
				case 0x01:                                               /*��ȡNPC��Ϣ*/
		      //CascadeCombine(0x00);
     			CascadeTableSend(buf, len, arg);
					break;
				case 0x02:
					break;
				case 0x03:
					break;
				case 0x04:
					ret = UniSerialComSetCmd(buf + 4, len - 4, arg);
					break;
				case 0x05:
					break;
				case 0x06:
					break;
				case 0x07:
					break;
				case 0x10:                                               /*ԭ�·�ָ��*/
					ret = NPC1Parse(buf + 4, len - 4, arg);
					break;
				case 0x11:                                               /*���·�ָ��*/
					ret = NPC2Parse(buf + 3, len - 3, arg);
					break;
				default:
					break;
			}
		}
		break;
		case 0x02:                                                   /*Ӧ�ò㷢��NPC�ն� */
		{
			                                                           /**/
			switch (buf[1]) {
				case 0x30:
					//AlarmQuery( buf + 4 ,len - 4, arg);
					break;
				case 0x31:
					//ResParaRequest(buf + 4, len - 4, 0, 0, arg->uart);
				  ResRequest(arg->buf, arg->len, 0,0,arg);
					AlarmCancel(buf[3]);
					break;
				case 0x40:
					Log_Cmd(arg->buf + 4, arg->len - 4,  arg);
					break;
				case 0x07:
					break;
				case 0x08:
					break;
				case 0x10:
					ret = YYCCtrlParse(buf + 3, len - 3, arg);
					break;
				default:
					break;
			}
			//OnlineParse(buf, len, uart);
		}
		break;
		case MODBUS_ADDR:
		{
			ModbusParse(buf, len, arg->uart);
		}					
		break;
		default:
//			ret = NPC1Parse(buf, len, uart);//��ʱ����0x01
			break;
	}
	
	return ret;
}


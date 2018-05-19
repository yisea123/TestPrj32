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
	
	
	
	
	
/** @brief  npc1.0的命令解析
  * @param  buf     待分析字符串指针
  *         len       待分析字符串的长度
  * @retval 返回值说明
  * @note   
  */
RET_STATUS NPC1Parse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	RET_STATUS ret = OPT_FAILURE;
	switch(buf[0]){//CDV地址应答
		case CDV_ID:
			ret = OnlineParse(buf, len, arg);
			break;
		case APP_ID:
//			DebugParse(buf, len, arg->uart);
			break;
		default:
			break;
	}
	
	return ret;
}

/** @brief  npc2.0的命令解析
  * @param  buf     待分析字符串指针
  *         len     待分析字符串的长度
  * @retval 返回值说明
  * @note   npc1的新版本的实现
主机ID	设备地址	资源类型或总线名	前置保留字段1	前置保留字段2	功能码	寄存器位置	数据位长度	报文数据位,中间用|$隔开，如果字符中有“|$”则转1换为^hsdz$	后置保留字 	协议	协议解析
  */
RET_STATUS NPC2Parse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	RET_STATUS ret       = OPT_FAILURE;	
#if _NPC_VERSION_ > 1u
	CDV_INT08U native    = CascadeGetNativeNo(); //本机号
#endif
//	CDV_INT08U devAddr   = buf[1];
//	CDV_INT08U res       = buf[2];
//	/*保留字段*/
	CDV_INT08U ftmp1     = buf[3];
//	CDV_INT08U ftmp2     = buf[4];
//	
//	CDV_INT08U fc        = buf[5];            /*功能码*/
//	CDV_INT08U regPos    = buf[6];            /*寄存器位置*/
	CDV_INT08U datLen    = buf[7];            /*数据位长度*/
//	CDV_INT08U* p_dat    = buf + 8;           /*报文数据*/
//	/*保留字段*/
//	CDV_INT08U btmp1     = buf[8 + datLen];
	CDV_INT08U prot      = buf[9 + len - 11/*(datLen?(len - 11):4)*/];   /*协议*///datLen:0变量4B；1数值（len-11）B
//	CDV_INT08U* p_pp     = buf + 10 + datLen; /*协议解析*/


	ASSERT(NULL != buf);
	ASSERT(0 != len);
	
	if (native == buf[0])                     /*检查主机ID*/
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
		
	} else {                              /*转发*/
		
	}
	return ret;
}

/** @brief  应用层控制解析
  * @param  buf     待分析字符串指针
  *         len       待分析字符串的长度
  * @retval 返回值说明
  * @note   buf[0] 为主机ID
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
		case 0x00:                                                   /*生产线控制*/
			ret = ManagerControl((WORKER_STATUS)ctrl1);
	  	if(OPT_FAILURE == ret)
			{
				if(arg->buf)
				arg->buf[1] += 0x80;
			}
	
			break;
		case 0x01:                                                   /*工人控制*/
			ret = WorkerControl(ctrl1, (WORKER_STATUS)ctrl2);
		  if(OPT_FAILURE == ret)
			{
				if(arg->buf)
				arg->buf[1] += 0x80;
			}
			break;
		case 0x10:                                                   /*生产线控制*/
			if (0x00 == ctrl1)
			{
				ret = ManagerQueryStatus(&result);
				resultLen = 1;
			}
			break;
		case 0x11:                                                   /*工人控制*/
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
			//case 0x20
		default:
			break;
		
	}
	ResRequest(arg->buf, arg->len, &result, resultLen, arg);
	
	return ret;
}

/** @brief  应用层控制解析20170819
  * @param  buf     待分析字符串指针
  *         len       待分析字符串的长度
  * @retval 返回值说明
  * @note   buf[0] 为要查询的工人数
  */
RET_STATUS YYCCtrlParse1(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	CDV_INT08U    i = 0, num , *opt, *ctrl1, *ctrl2, resultLen = 0, result = 0;
	WORKER_STATUS status;
	CDV_INT32U    step;
	RET_STATUS    ret = OPT_FAILURE;
	num = buf[0];
	opt = buf + 2;
	ctrl1 = buf + 3;
	ctrl2 = buf + 4;

	if (num)
	{
		if ((len-2)/4 == num)
		{
			for (i = 0; i < num; i++)
			{
				switch (*opt) {
						case 0x0:                                                   /*查状态*/
							
								ret = WorkerQueryStatus(*ctrl1, (WORKER_STATUS*)ctrl2);
							
							break;
						case 0x1:                                                   /*查位置*/
							
								ret = WorkerQueryStep(*ctrl1, (CDV_INT16U*)ctrl2);
							
							break;
						default:
							break;
				}
				
				opt = opt + 4;
				ctrl1 = ctrl1 + 4;
				ctrl2 = ctrl2 + 4;
			}
		}
		ResRequest(arg->buf, arg->len, 0,0,arg);
	}
	else
	{
		
		switch (*opt) {
			case 0x00:                                                   /*生产线控制*/
				ret = ManagerControl((WORKER_STATUS)*ctrl1);
				if(OPT_FAILURE == ret)
				{
					if(arg->buf)
					arg->buf[1] += 0x80;
				}
		
				break;
			case 0x01:                                                   /*工人控制*/
				ret = WorkerControl(*ctrl1, (WORKER_STATUS)*ctrl2);
				if(OPT_FAILURE == ret)
				{
					if(arg->buf)
					arg->buf[1] += 0x80;
				}
				break;
			case 0x10:                                                   /*生产线控制*/
				if (0x00 == *ctrl1)
				{
					ret = ManagerQueryStatus(&result);
					resultLen = 1;
				}
				break;
			case 0x11:                                                   /*工人控制*/
				if (0x00 == *ctrl2)
				{
					ret = WorkerQueryStatus(*ctrl1, &result);
					resultLen = 1;
				}
				else if (0x01 == *ctrl2)
				{
					ret = WorkerQueryStep(*ctrl1, (CDV_INT16U*)&result);
					resultLen = 1;
				}
				break;
			default:
				break;
			
		}
		ResRequest(arg->buf, arg->len, &result, resultLen, arg);
	
	}
	
	return ret;
}

/** @brief  命令解析
  * @param  buf     待分析字符串指针
  *         len       待分析字符串的长度
  * @retval 返回值说明
  * @note   在串口接收中
	*         20160107：添加串口号
  *         不能使用addtx等，因如果workermanage先申请了锁，会导致死锁
主字段		从字段		报文数据长度
  */
RET_STATUS CmdParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
{
	RET_STATUS ret = OPT_FAILURE;
//	if (len != buf[2] + 3)                                         /*报文数据长度不正常？*/
//		return ret;
	
	
	
	
	switch (buf[0]) {
		case 0xF1:                                                   /*开发层发送NPC终端*/
		{
			                                                           /**/
			switch (buf[1]) {
				case 0x10:                                               /*原下发指令*/
					ret = NPC1Parse(buf + 4, len - 4, arg);
					break;
				case 0x11:                                               /*新下发指令:通用串口*/
					ret = NPC2Parse(buf + 3, len - 3, arg);
					break;
				case 0x01:                                               /*获取NPC信息*/
		      //CascadeCombine(0x00);
#if _NPC_VERSION_ > 1u
     			CascadeTableSend(buf, len, arg);
#endif
					break;
				case 0x02:
					break;
				case 0x03:
					break;
				case 0x04:
					ret = UniSerialComSetCmd(buf + 4, len - 4, arg);
				  if(OPT_SUCCESS != ret)
					{
						arg->buf[1] += 0x80;
					  ResRequest(arg->buf, arg->len, 0,0,arg);
					}
					break;
				case 0x05:
					break;
				case 0x06:
					break;
				case 0x07:
					break;
				case 0x30:
#if _NPC_VERSION_ > 1u
				  NPC_NETParse(buf + 2 ,len - 2, arg);
#endif
					break;
				default:
					break;
			}
		}
		break;
		case 0xF2:                                                   /*应用层发送NPC终端 */
		{
			                                                           /**/
			switch (buf[1]) {
				case 0x29:
				  AlarmQuery( buf + 4 ,len - 4, arg); 
				  break;
				case 0x30:
#if _NPC_VERSION_ > 1u
				  NPC_NETParse(buf + 2 ,len - 2, arg);
#endif
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
					ret = YYCCtrlParse1(buf + 2, len - 2, arg);
					break;
				case 0x11:                                              
					ret = NPC1Parse(buf + 4, len - 4, arg);
					break;
				default:
					break;
			}
			//OnlineParse(buf, len, uart);
		}
		break;
		case 0xF3:
		{
		  switch (buf[1]) {
				case 0x01:
					break;
				case 0x02:
					CentralizedControl_QueryDevData(buf, len, arg);
					break;
				case 0x03:
					break;
				case 0x04:
					break;
				default:
					break;
			}
		}
		break;
		default:
		{
			if (arg->hostid == buf[0])
			{
				ModbusParse(buf, len, arg->uart);
			}			
			break;
		}
	}
	
	return ret;
}


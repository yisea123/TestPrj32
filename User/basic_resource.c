/**
  ******************************************************************************
  * @file    /cdv_resource.c 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   user function
  * 
@verbatim  

               CDV基本资源控制函数，牵扯到联机时modbus和脱机运行时直接操作，减少modbus与其他文件的关联
							 20151221：DisplayCmd中改变中文长度的判断规则，根据最后一个字节判断
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "basic_resource.h"
	
	
//	void DisplayVar(const CDV_INT08U valNo, const CDV_INT08U row, const CDV_INT08U uartNo);
	
	
	
	/*CDV资源全局变量，在CDVInit中初始化*/


//CDV_INT08U resReqed = 0;
//CDV_INT08U g_resUart = 0;/*标记调用cmdparse时的串口，用于回复*/
/**
  * @brief  CDV资源初始化
  *  
  * @param  无
  *   
  * @retval 无
  *
  * @note   配置所有CDV资源的变量
  */
void CdvInit(void) {
//	CDV_INT08U *ptr;
  CDV_INT08U i;
//	SPI_Flash_Read((CDV_INT08U *)&RW_SRP_NUM_RUN,SCRIP_NUM_RUN,2);
//	SPI_Flash_Read((CDV_INT08U *)&RW_SRP_NUM_DEBUG,SCRIP_NUM_DEBUG,2);
	IOInit();
	
	memset(&g_modbusCoil , 0x00, sizeof(g_modbusCoil));

  for(i = 0 ; i < CDV_O_NUM/* + CDV_EXO_NUM*2 */; i ++) {
		OWrite(i , BIT_0);
	}
	
	for(i = 0 ; i < CDV_I_NUM/* + CDV_EXO_NUM*2 */; i ++) {
		IRead(i);
	}
//	while(1) {
//		for(i = 0 ; i < CDV_IO_NUM/* + CDV_EXO_NUM*2 */; i ++) {
//			IRead(i);
//		}
//	}
	
		MemCpy(g_pluse, g_i, COIL_CHN);
//SPI_Flash_Write((CDV_INT08U *)&g_modbusReg, REG_ADDR, sizeof(g_modbusReg));
 //   SPI_Flash_Write((CDV_INT08U *)&g_modbusInReg, INREG_ADDR, sizeof(g_modbusInReg));
	/*modbus初始化*/
	
//#ifndef	CDV_V1
    /*FPGA程序下载配置*/
//	CH_CDV_DIP = 0;
//	for(i = 0; i < CDV_DIP_NUM ; i++){
//		CH_CDV_DIP |= ((DIP_ON == READ_DIP_SW(i) ? 1 : 0)<< i);/*拨码开关从1到4分别控制fpga0-3的下载程序*/
//	}
	
ClearUniversalCnt();

    //FpgaRecvCtl();	
//	if(DIP_OFF == READ_DIP_SW(5) && DIP_OFF == READ_DIP_SW(4)/* && DIP_OFF == READ_DIP_SW(0)*/) {
//	/*可以添加读取fpga配置信息，然后进行不同的配置*/
//		//FpgaInit(RW_CFG_FPGA);
////	  for( i = 1; i<= 2 ; i++) {
////		  if(dip & (0x01 << i))
////		  {
////			  Fpga_Config(i, FPGA_MOTOR);//下载引脚配置
////			  Fpga_Down(i, FPGA_MOTOR);
////		  } else {
////			  Fpga_Config(i, FPGA_IO);//第二块
////			  Fpga_Down(i, FPGA_IO);
////		  }
////	  }
//	}
//#endif

}
	
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
  * @brief  分析字符串并执行驱动
  *  
  * @param  rxBuf     字符串
  *         rxLen     字符串长度     
  *         debug      0脱机运行
                       1脱机调试
                       2联机调试
  *   
  * @retval 返回值说明
            OPT_SUCCESS命令执行完成
            OPT_FAILURE命令未执行完成，比如等待未等到
  *
  * @note   
  */

RET_STATUS ResParse(CDV_INT08U* rxBuf, CDV_INT16U rxLen , CMD_ARG *arg) {
	RET_STATUS ret = OPT_SUCCESS;
	
	switch(rxBuf[0]){
		case 0x05:/*变量*/
			ret = ValCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x08:/*逻辑资源*/
			ret = LogicScript(rxBuf+1, rxLen - 1, arg);
			break;
		case 0x00:/*主轴命令*/
#if ENABLE_FPGA_DOWN
			ret = MotorCmd(rxBuf + 1, rxLen - 1, arg);
#endif
			break;
		case 0x01:/*I*/
			ret = ICmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x02:/*O*/
			ret = OCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x03:/*定时器*/
			ret = TimeCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x04:/*消息*/
			ret = MsgCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x0B:
			ret = AdcCmd(rxBuf + 1,rxLen - 1, arg);
		  break;
		case 0x0C:
			ret = DacCmd(rxBuf + 1,rxLen - 1, arg);
			break;
		case 0x0D:/*计时器*/
			ret = ClockCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x0E:/*工人*/
			ret = WorkerCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x10:/*生产线*/
			ret = LineCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0xFE:/*系统控制*/
			ret = SysCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		default:
			ret = OPT_SUCCESS;
			break;
	}
	
	if(rxBuf[0] != 0x08 && OPT_FAILURE == ret) {
		TaskSched();
	}
	
	return ret;
}

///**
//  * @brief  显示脚本命令
//  *  
//  * @param  scriptNo     待分析工人号  
//  *   
//  * @retval 返回值说明
//            OPT_SUCCESS命令执行完成
//  *
//  * @note   使用到UCOS的锁机制，保证每次只有一个在操作串口tx队列
//						显示资源类型	显示资源号	行号（00-FF）																															字符串
//						00（主轴）		（00-FF）		（00-FF）									
//						01（I)				（00-FF）		（00-FF）	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	
//						02（O）				（00-FF）		（00-FF）	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	
//						03（定时器）	（00-FF）		（00-FF）									
//						04（消息）		（00-FF）		（00-FF）	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	
//						05（变量）		（00-FF）		（00-FF）									
//						FF（注释）		（00-FF）									
//  */

//RET_STATUS DisplayCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U debug, CDV_INT08U uartNo) { 
////	OS_ERR err;
//	//CPU_SR_ALLOC();//OS_CRITICAL_ENTER()
//	CDV_INT08U len , row , type , strLen , *txBuf = NULL;
//	CDV_INT32S num;
//	
////	if(debug) {
////		return OPT_SUCCESS;
////	}
//	strLen = rxBuf[rxLen - 1];//获取最后一个字节，表示的是前面的中文串长度
//	switch(rxBuf[0]) {
//		case 0x09:
//			//strLen = rxLen - 2;//中文长度
//			len = rxLen/*strLen + 4*/;//发给APP的字符串长度
//			row = rxBuf[2];//第几行显示
//		    type = 0x05;//注释类型
//			break;
////		case 0x00:
////			//strLen = rxLen - 3;//中文长度
////			len = strLen + 8;//发给APP的字符串长度
////			row = rxBuf[2];//第几行显示
////		  type = 0xFF;//数值类型
////		  //num = R_MOTOR_POS(SCRIPT_DISPLAY_RES_NO(scriptNo));
//////		  READ_MOTOR_POS(rxBuf[1] , num);//取得要显示的int值
////			break;
////		case 0x03:
////			//strLen = rxLen - 3;//中文长度
////			len = strLen + 8;//发给APP的字符串长度
////			row = rxBuf[2];//第几行显示
////		  type = 0xFF;//数值类型
//////		  num = RW_TIMER(rxBuf[1]);//取得要显示的int值
////			break;
////		case 0x04:
////			//strLen = rxLen - 3;//中文长度
////			len = strLen + 4;//发给APP的字符串长度
////			row = rxBuf[2];//第几行显示
////		  type = READ_MSG(rxBuf[1]);//取得要显示的int值
////			break;
//		case 0x01:
//			//strLen = rxLen - 3;//中文长度
//			len = strLen + 12;//发给APP的字符串长度
//			row = rxBuf[2];//第几行显示
//		    type = IRead(rxBuf[1]);//取得要显示的int值
//			break;
//		case 0x02:
//			//strLen = rxLen - 3;//中文长度
//			len = strLen + 12;//发给APP的字符串长度
//			row = rxBuf[2];//第几行显示
//		    type = ORead(rxBuf[1]);//取得要显示的int值
//			break;
//		case 0x05:
//			//strLen = rxLen - 3;//中文长度
//			len = strLen + 8;//发给APP的字符串长度
//			row = rxBuf[2];//第几行显示
//		    type = 0xFF;//数值类型
//		    num = VarGet(rxBuf[1]);//取得要显示的int值
//		
//		
//		DisplayVar(rxBuf[1], row, uartNo);
//		
//		return OPT_SUCCESS;
//		
////			break;
//		default:
//			return OPT_SUCCESS;
//		
//	}
//	
//	/*把显示内容加载到串口tx队列*/
//	//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(len));
//	//while(len == 0 || len > 40);
//  NEW08U(txBuf , len);
//	txBuf[0] = APP_ID;
//	txBuf[1] = 0x02;
//	txBuf[2] = row;
//	txBuf[3] = type;
//	switch (type) {
//		case 0xFF:
//			txBuf[4] = (CDV_INT08U)(num>>24);
//		    txBuf[5] = (CDV_INT08U)(num>>16);
//		    txBuf[6] = (CDV_INT08U)(num>>8);
//		    txBuf[7] = (CDV_INT08U)num;
//			memcpy(txBuf + 8, rxBuf + 3, strLen);
//			break;
//		case 0x00:
//		case 0x01:
//		case 0x02:
//		case 0x03:
//			memcpy(txBuf + 4, rxBuf + 3, strLen+8);
//			break;
//		case 0x05:
//			memcpy(txBuf + 4, rxBuf + 3, rxLen-4/*strLen*/);
//			break;
//		default:
//			DELETE (txBuf);
//			return OPT_SUCCESS;
//	}
//	AddTx(txBuf , len, uartNo);
//	DELETE (txBuf);
//	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err);   //延时 10ms
//	return OPT_SUCCESS;
//}


/**
  * @brief  定时器脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   资源号（00-FF）	00延时	FF	uint32数值低位	uint32数值	uint32数值	uint32数值高位					字符串
		                                FE	变量代号高位	变量代号低位							
  */



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//************************************************
//电机向前/向后运动
//no：电机号（0-7）
//num：向前步数
//type：1向前，0向后
//************************************************
//RET_STATUS Motor_Forward_Backward(CDV_INT08U no /*, CDV_INT08U opt*/ , CDV_INT32S num,u8 type) {	
//#ifndef	CDV_V1
//	u16 FPGA_POSL;
//	u16 FPGA_POSH;
//	u32 FPGA_POS;
//	u8 dir=type? 1:0;
//	FPGA_MOTOR_INFO fpgaPar;
//	switch(no)
//	{
//		case 0:
//			     FPGA_POSH=Read_Fpga_Regist(0xd6,1);
//					 FPGA_POSL=Read_Fpga_Regist(0xd7,1);
//		       break;
//		case 1:
//			     FPGA_POSH=Read_Fpga_Regist(0xd8,1);
//					 FPGA_POSL=Read_Fpga_Regist(0xd9,1);
//		       break;
//		case 2:
//			     FPGA_POSH=Read_Fpga_Regist(0xd6,2);
//					 FPGA_POSL=Read_Fpga_Regist(0xd7,2);
//		       break;
//		case 3:
//			     FPGA_POSH=Read_Fpga_Regist(0xd8,2);
//					 FPGA_POSL=Read_Fpga_Regist(0xd9,2);
//		       break;
//		case 4:
//			     FPGA_POSH=Read_Fpga_Regist(0xd6,3);
//					 FPGA_POSL=Read_Fpga_Regist(0xd7,3);
//		       break;
//		case 5:
//			     FPGA_POSH=Read_Fpga_Regist(0xd8,3);
//					 FPGA_POSL=Read_Fpga_Regist(0xd9,3);
//		       break;
//		case 6:
//			     FPGA_POSH=Read_Fpga_Regist(0xd6,4);
//					 FPGA_POSL=Read_Fpga_Regist(0xd7,4);
//		       break;
//		case 7:
//			     FPGA_POSH=Read_Fpga_Regist(0xd8,4);
//					 FPGA_POSL=Read_Fpga_Regist(0xd9,4);
//		       break;
//		default:break;
//  }		
//  FPGA_POS =(FPGA_POSH<<16)+FPGA_POSL;
//	if(1 != MODBUS_READ_CFG_FPGA(no>>1))//判断配置的是不是电机
//		return OPT_SUCCESS;
//  fpgaPar.acc = RW_MOTOR(no).acc;
//	fpgaPar.dec = RW_MOTOR(no).dec;
//	fpgaPar.maxSpd = RW_MOTOR(no).maxSpeed;
//	MODBUS_RESET_MOTOR_END(no);
////	if(0 != num){
//			fpgaPar.len = num;
////		if(0 != fpgaPar.len) {
//			if(MODBUS_READ_RUN_DIR(no)) {//如果用户设定了翻转方向
//				dir = dir? 0 :1;
//			}
//			/*相对移动*/
//			FpgaMotoRun(no ,1,dir ,&fpgaPar);
//			if(dir==0)
//			 FPGA_POS=FPGA_POS+num;	
//			else
//				FPGA_POS=FPGA_POS-num;
//			WRITE_MOTOR_POS(no , FPGA_POS);	
////		}
////	}
//#endif
//	return OPT_SUCCESS;
//}	
/**
  * @brief  电机绝对运行
  *  
  * @param  no     要操作的电机号
  *         num    0    归零
  *                非零 绝对运动目标位置(脉冲)
  *   
  * @retval 
  *
  * @note   控制单轴电机相对运动、绝对运动、归零等
  *         从modbus寄存器中读RW_MOTOR。maxSpeed，acc，dec，upLimit，downLimit，minSpeed（由联机状态时设定好）
  *                           R_MOTOR_POS (变成主要维护值，可读写，每次发送运动命令成功后更改该值)
  *         发送命令给FPGA
  */
//RET_STATUS MotorRun(CDV_INT08U no /*, CDV_INT08U opt*/ , CDV_INT32S num) {	
//#ifndef	CDV_V1
//	u16 FPGA_POSL;
//	u16 FPGA_POSH;
//	u32 FPGA_POS;
//	u8 dir;
//	FPGA_MOTOR_INFO fpgaPar;
//	switch(no)
//	{
//		case 0:
//			     FPGA_POSH=Read_Fpga_Regist(0xd6,1);
//					 FPGA_POSL=Read_Fpga_Regist(0xd7,1);
//		       break;
//		case 1:
//			     FPGA_POSH=Read_Fpga_Regist(0xd8,1);
//					 FPGA_POSL=Read_Fpga_Regist(0xd9,1);
//		       break;
//		case 2:
//			     FPGA_POSH=Read_Fpga_Regist(0xd6,2);
//					 FPGA_POSL=Read_Fpga_Regist(0xd7,2);
//		       break;
//		case 3:
//			     FPGA_POSH=Read_Fpga_Regist(0xd8,2);
//					 FPGA_POSL=Read_Fpga_Regist(0xd9,2);
//		       break;
//		case 4:
//			     FPGA_POSH=Read_Fpga_Regist(0xd6,3);
//					 FPGA_POSL=Read_Fpga_Regist(0xd7,3);
//		       break;
//		case 5:
//			     FPGA_POSH=Read_Fpga_Regist(0xd8,3);
//					 FPGA_POSL=Read_Fpga_Regist(0xd9,3);
//		       break;
//		case 6:
//			     FPGA_POSH=Read_Fpga_Regist(0xd6,4);
//					 FPGA_POSL=Read_Fpga_Regist(0xd7,4);
//		       break;
//		case 7:
//			     FPGA_POSH=Read_Fpga_Regist(0xd8,4);
//					 FPGA_POSL=Read_Fpga_Regist(0xd9,4);
//		       break;
//		default:break;
//  }		
//  FPGA_POS =((FPGA_POSH & 0x7f)<<16)+FPGA_POSL;
//	dir = num > FPGA_POS? 1 : 0;
//	if(1 != MODBUS_READ_CFG_FPGA(no>>1))//判断配置的是不是电机
//		return OPT_SUCCESS;
//  fpgaPar.acc = RW_MOTOR(no).acc;
//	fpgaPar.dec = RW_MOTOR(no).dec;
//	fpgaPar.maxSpd = RW_MOTOR(no).maxSpeed;
//	MODBUS_RESET_MOTOR_END(no);
//	if(0 != num){
//		if(dir) {//设定要走的步数
//			fpgaPar.len	= num - FPGA_POS;
//		} else {
//			fpgaPar.len = FPGA_POS - num;
//		}
//		if(0 != fpgaPar.len) {
//			if(MODBUS_READ_RUN_DIR(no)) {//如果用户设定了翻转方向
//				dir = dir? 0 :1;
//			}
//			/*绝对移动*/
//			FpgaMotoRun(no ,1,dir ,&fpgaPar);
//			//WRITE_MOTOR_POS(no , num);
//		}
//		}else {
//		if(MODBUS_READ_ZERO_DIR(no)) {//如果用户设定了翻转方向
//			dir = dir? 0 :1;
//		}
//		
//		/*归零*/
//		//__nop();
//		FpgaMotoRun(no ,0,dir ,&fpgaPar);
//		CLEAR_MOTOR_POS(no);
//		//memset( R_MOTOR_PPOS(no) , 0 , 4);
//	}

//#endif
//	return OPT_SUCCESS;
//}	

/**
  * @brief  电机虚拟绝对运行
  *  
  * @param  no     要操作的电机号
  *         num    0    归零
  *                非零 绝对运动目标位置(脉冲)
  *   
  * @retval 
  *
  * @note   需配合MotorStepRefresh();	及cdv刷新线程
  */
RET_STATUS VirtualMotorRun(CDV_INT08U no /*, CDV_INT08U opt*/ , CDV_INT32S num) {	
//#ifndef	CDV_V1
//	Moto_Solute_Run(no,num);
////	WRITE_MOTOR_STEP(no ,num);
////	MODBUS_SET_MOTOR_EN(no);
////	MODBUS_SET_ABS;
//#endif
	return OPT_SUCCESS;
}

/**
  * @brief  电机实际绝对运行
  *  
  * @param  no     要操作的电机号
  *         num    0    归零
  *                非零 绝对运动目标位置(脉冲)
  *   
  * @retval 
  *
  * @note   需配合MotorStepRefresh();	及cdv刷新线程
  */
RET_STATUS ActualMotorRun(CDV_INT08U no /*, CDV_INT08U opt*/ , CDV_INT32S num) {	
//#ifndef	CDV_V1
////	WRITE_DST_POS(no ,num);
//	Moto_Solute_Run(no,num);
//#endif
	return OPT_SUCCESS;
}

/**
  * @brief  电机急停
  *  
  * @param  no     要操作的电机号
  *         num    0    归零
  *                非零 绝对运动目标位置(脉冲)
  *   
  * @retval 
  *
  * @note   
  */
//RET_STATUS MotorStop(CDV_INT08U no ) {	
//#ifndef	CDV_V1
//    CDV_INT32S num;
//	FPGA_MOTOR_INFO fpgaPar;
//	
//	if(1 != MODBUS_READ_CFG_FPGA(no>>1))//判断配置的是不是电机
//		return OPT_SUCCESS;
//	
//    fpgaPar.acc = RW_MOTOR(no).acc;
//	fpgaPar.dec = RW_MOTOR(no).dec;
//	fpgaPar.maxSpd = RW_MOTOR(no).maxSpeed;
//	MODBUS_RESET_MOTOR_END(no);
//	fpgaPar.len	= 0;
//	/*绝对移动*/
//	FpgaMotoRun(no ,1,0 ,&fpgaPar);
//	//__nop();
//	num = R_MOTOR_POS(no);
//	WRITE_DST_POS(no , num);
//	//memcpy( R_MOTOR_PPOS(no) , &num , 4);
//	
//#endif
//	return OPT_SUCCESS;
//}
///**
//  * @brief  电机圆弧运行
//  *  
//  * @param  no     要操作的电机号
//  *   
//  * @retval 
//  *
//  * @note   控制单轴电机相对运动、绝对运动、归零等
//	*         从modbus寄存器中读RW_MOTOR。maxSpeed，acc，dec，upLimit，downLimit，minSpeed（由联机状态时设定好）
//	*                           R_MOTOR_POS (变成主要维护值，可读写，每次发送运动命令成功后更改该值)
//	*         发送命令给FPGA
//  */
//RET_STATUS MotorArc(CDV_INT08U no , CDV_INT32U x , CDV_INT32U y) {
//	
//	
//	
//	return OPT_SUCCESS;
//}
///**
//  * @brief  电机查询
//  *  
//  * @param  no     要操作的电机号0-7
//	*         opt    0 等待运行完成
//	*                1 
//	*                2  
//  *   
//  * @retval 
//  *
//  * @note   查询电机到位，速度，加速度等
//	*         从modbus寄存器中读RW_MOTOR。maxSpeed，acc，dec，upLimit，downLimit，minSpeed（由联机状态时设定好）
//	*                           R_MOTOR_POS (变成主要维护值，可读写，每次发送运动命令成功后更改该值)
//	*         发送命令给FPGA
//  */
//RET_STATUS MotorChk(CDV_INT08U no , CDV_INT08U opt) {
////	CDV_INT08U val;
//	RET_STATUS ret =OPT_SUCCESS;
//#ifndef	CDV_V1
//	if(1 != MODBUS_READ_CFG_FPGA(no>>1))//判断配置的是不是电机
//		return OPT_SUCCESS;
//	
//	switch (opt) {
//		case 0:
//			/*等待运行完成*/
//		    //__nop();//__NOP;
//		  
//			if(FpgaMotoWait(no)){
//				//MODBUS_SET_MOTOR_END(no);
//			} else {
//				MODBUS_RESET_MOTOR_END(no);
//				ret = OPT_FAILURE;
//			}
//			break;
//	  default:
//			break;
//	}
//#endif
//	return ret;
//}


///**
//  * @brief  CDV I刷新。
//  *  
//  * @param  无
//  *   
//  * @retval 无
//  *
//  * @note   读取所有外设I的值并刷新modbus线圈
//  */
//void IRefresh(void) {
//	CDV_INT08U i;
//	
//	for(i = 0 ; i < CDV_I_NUM ; i ++) {/*I*/
//		IRead(i);
//	}
//	
//	for(i = 0 ; i < CDV_DIP_NUM ; i ++) {/*DIP*/
//		DIPRead(i);
//	}
//	
//	for(i = 0 ; i < CDV_EXI_NUM*CDV_FPGA_NUM; i ++) {/*exI*/
////		SF0_CS1_DISENABLE;//这里把第二块FPGA作为扩展IO
////		SF0_CS1_DISENABLE;
////		SF0_CS4_DISENABLE;
////		SF0_CS2_DISENABLE;//所以只使能了第二块FPGA
////		SF0_CS2_ENABLE;
//		ExIRead(i);
//	}
//}
///**
//  * @brief  CDV O刷新。
//  *  
//  * @param  无
//  *   
//  * @retval 无
//  *
//  * @note   读取所有modbus线圈的O的值并刷新外设
//  */
//void ORefresh(void) {
//	CDV_INT08U i;
//	for(i = 0 ; i < CDV_IO_NUM + CDV_EXO_NUM*CDV_FPGA_NUM; i ++) {
//		OWrite(i , (IO_VAL)MODBUS_READ_O(i) , 0);
//	}
//	
////	for(i = 0 ; i < CDV_EXO_NUM ; i ++) {
////		ExOWrite(i , (IO_VAL)MODBUS_READ_EXO(i));
////	}
//}
///**
//  * @brief  CDV 电机刷新。
//  *  
//  * @param  无
//  *   
//  * @retval 无
//  *
//  * @note   读取所有modbus线圈的电机的值并刷新外设
//  */
//void MotorRefresh(void) {
//	CDV_INT08U i;
////	CDV_INT32S pos;
//	//OS_ERR err;
//	
////	if(1 == MODBUS_READ_RETURN) {//电机回零
////		for( i = 0; i< 8; i++) {
////		  if(1 == MODBUS_READ_MOTOR_EN(i)) {
////				MotorRun(i , 0);
////				MODBUS_RESET_MOTOR_END(i);
////			}
////		}
////		MODBUS_RESET_RETURN;
////	} else if(1 == MODBUS_READ_FRONT) {
////		for( i = 0; i< 8; i++) {
////		  if(1 == MODBUS_READ_MOTOR_EN(i)) {
////				READ_MOTOR_POS(i, pos);
////				MotorRun(i , pos + R_MOTOR_STEP(i));
////				MODBUS_RESET_MOTOR_END(i);
////			}
////		}
////		MODBUS_RESET_FRONT;
////	} else if(1 == MODBUS_READ_BACK) {
////		for( i = 0; i< 8; i++) {
////		  if(1 == MODBUS_READ_MOTOR_EN(i)) {
////				READ_MOTOR_POS(i, pos);
////				MotorRun(i , pos - R_MOTOR_STEP(i));
////				MODBUS_RESET_MOTOR_END(i);
////			}
////		}
////		MODBUS_RESET_BACK;
////	} else if(1 == MODBUS_READ_ABS) {
////		for( i = 0; i< 8; i++) {
////		  if(1 == MODBUS_READ_MOTOR_EN(i)) {
////				MotorRun(i , R_MOTOR_STEP(i));
////				MODBUS_RESET_MOTOR_END(i);
////			}
////		}
////		MODBUS_RESET_ABS;
////	} else if(1 == MODBUS_READ_ARC) {
////		MODBUS_RESET_ARC;
////	}

//	
//	for( i = 0; i< (CDV_FPGA_NUM<<1); i++) {
//	  if(OPT_SUCCESS == MotorChk(i,0)) {//判断归零是否完成
////		    if(R_MOTOR_POS(i) != R_MOTOR_STEP(i)) {//判断目标地址是否与当前位置一样
////				MotorRun(i , R_MOTOR_STEP(i));
////			}
//		}
//	}
//	for( i = 0; i< (CDV_FPGA_NUM<<1); i++) {
//	  if(1 == MODBUS_READ_MOTOR_END(i)) {//判断归零是否完成
////		    if(R_MOTOR_POS(i) != R_DST_POS(i)) {//判断目标地址是否与当前位置一样
//			   // OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_NON_STRICT,&err); //延时	
//				//MotorRun(i , R_DST_POS(i));
//				//OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_NON_STRICT,&err); //延时	
////			}
//		}
//	}
//	
//}
			
///**
//  * @brief  CDV 电机目标步数刷新。
//  *  
//  * @param  无
//  *   
//  * @retval 无
//  *
//  * @note   读取所有modbus线圈的电机的值并刷新外设
//  */
//void MotorStepRefresh(void) {
//	CDV_INT08U i;
//	CDV_INT32S pos;
//	if(1 == MODBUS_READ_RETURN) {//电机回零
//		for( i = 0; i< (CDV_FPGA_NUM<<1); i++) {
//		  if(1 == MODBUS_READ_MOTOR_EN(i)) {
//			    if(0 == R_MOTOR_POS(i)) {
//					pos = 1;
//					WRITE_MOTOR_POS(i , pos);
//				}
//		
//			    
//			    pos = 0;
//				WRITE_DST_POS(i ,pos);
//				MODBUS_RESET_MOTOR_EN(i);
//			}
//		}
//		MODBUS_RESET_RETURN;
//	} else if(1 == MODBUS_READ_FRONT) {
//		for( i = 0; i< (CDV_FPGA_NUM<<1); i++) {
//		  if(1 == MODBUS_READ_MOTOR_EN(i)) {
//				READ_DST_POS(i, pos);
//				pos += R_MOTOR_STEP(i);
//				WRITE_DST_POS(i ,pos);
//				MODBUS_RESET_MOTOR_EN(i);
//			}
//		}
//		MODBUS_RESET_FRONT;
//	} else if(1 == MODBUS_READ_BACK) {
//		for( i = 0; i< (CDV_FPGA_NUM<<1); i++) {
//		  if(1 == MODBUS_READ_MOTOR_EN(i)) {
//				READ_DST_POS(i, pos);
//				pos -= R_MOTOR_STEP(i);
//				WRITE_DST_POS(i ,pos);
//				MODBUS_RESET_MOTOR_EN(i);
//			}
//		}
//		MODBUS_RESET_BACK;
//	} else if(1 == MODBUS_READ_ABS) {
//		for( i = 0; i< (CDV_FPGA_NUM<<1); i++) {
//		  if(1 == MODBUS_READ_MOTOR_EN(i)) {
//				MODBUS_SET_MOTOR_END(i);
//				pos = R_MOTOR_STEP(i);
//				WRITE_DST_POS(i ,pos);
//				MODBUS_RESET_MOTOR_EN(i);
//			}
//		}
//		MODBUS_RESET_ABS;
//	} else if(1 == MODBUS_READ_ARC) {
//		MODBUS_RESET_ARC;
//	}

//}

///**
//  * @brief  变量运行显示
//  *  
//  * @param  valNo     待显示的变量号
//  *   
//  * @retval 返回值说明
//  *
//  * @note   从flash中获取变量的信息，根据数值还是枚举来组织中文
//  */
//void DisplayVar(const CDV_INT08U valNo, const CDV_INT08U row, const CDV_INT08U uartNo) {
//	//CDV_INT08U i = 0;
//	CDV_INT08U *txBuf = NULL;
//	CDV_INT08U len = 5;//, row ;
//	CDV_INT08U no , parNo , type , strLen, parLen;
//	CDV_INT32S num;
//	
//	CDV_INT32U size = 0;//脚本大小，供跳转
//	CDV_INT32U shift = 0;//偏移起始地址的字节数, 查询参数时的临时shift
//	CDV_INT32U strAddr, parAddr;//endAddr = 0, 
//	CDV_INT32U addr;//得到变量脚本的地址

//	
//		no = valNo;

//		num = VarGet(no);

//		//ValSet(no , num);
//		//获取名称
//		addr = GetResAddr(0x05, no, 0x00, 2);//获取no变量的地址
//		if(!addr)
//			return;
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, addr, 2);//读取资源脚本大小
//	  shift = 2;
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&strLen, addr + shift, 1);//读取资源名称长度
//		shift += 1;
//		strAddr = addr + shift;//名称地址保存
//		
//		
//		
//		
//		
//		
//		//获取变量值
//		addr = GetResAddr(0x05, no, 0x00, 3);//值类型
//		if(!addr)
//			return;
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, addr, 2);//读取资源参数大小
////		if(size != 1)
////			return;
////		
//		shift = 2;
//		parAddr = addr + shift + 1;//参数地址保存
//		parLen = size - 1;//参数长度保存
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&parNo, addr + shift, 1);//读取参数代号
//		
//			
//			if(parNo == 0x01) {//数值
//				type = 0xff;
//				len = 8 + strLen;
//			}
//			else if(parNo == 0x00) {//枚举
//				if(parLen%2  || (parLen >>1)>0x20)//枚举值的数量检查
//					return;
//				
//				
//					if(num > (parLen>>1)-1 || num < 0) {
//						num = 0x00;
//					}
//					type = num;
//				
//				len = 5 + strLen + parLen;
//			}
//			else
//				return;
//		
//		
//		NEW08U(txBuf , len);
//		txBuf[0] = APP_ID;
//		txBuf[1] = 0x02;
//		txBuf[2] = row;//第几条设置命令
//		
//		switch (type) {
//			case 0xFF:
//				txBuf[3] = type;
//				txBuf[4] = (CDV_INT08U)(num>>24);
//				txBuf[5] = (CDV_INT08U)(num>>16);
//				txBuf[6] = (CDV_INT08U)(num>>8);
//				txBuf[7] = (CDV_INT08U)num;
//				SPI_Flash_Read(txBuf + 8, strAddr, strLen);
//				break;
//			default:
//				txBuf[3] = type;
//			  txBuf[4] = (CDV_INT08U)(parLen>>1);
//				SPI_Flash_Read(txBuf + 5, parAddr, parLen);
//			  SPI_Flash_Read(txBuf + 5 + parLen, strAddr, strLen);
//				
//				break;
//		}
//		AddTx(txBuf , len , uartNo);
//		DELETE (txBuf);
//		
//	
//}

/*
  *
  *把几个U8转换成一个U32
  *
  *
  */

CDV_INT32S CalculateForAll(CDV_INT08U* buf,CDV_INT32S addr,CDV_INT32S lenth)
{
	CDV_INT32S i,x=0;
	for(i=0;i<lenth;i++)
	{
		//x|=buf[addr++]<<8*((lenth-1)-i);
		x|=buf[addr++]<<8*(i);
	}
  
	return x;
}

/** @brief  资源反馈(旧版)。快点弃用
  * @param  rxBuf     原始字符串
  *         rxLen     ↑长度
	*         para      附加参数字符串
	*         paraLen   ↑长度
  * @retval 无
  * @note   最终发送的字符串如下
	*         ○rxBuf 不为NULL
	*       ┌  3byte  ┬──rxLen byte──┬   paraLen   ┬   2byte  ┐
	*                                                     │          │
	*     rxBuf-6    rxBuf                para            │    CRC   │                                    
	*       │         │                  │             │          │                                                             
	*                                                                                      
	*         ○rxBuf 为NULL
	*       ┌     paraLen   ┬   2byte  ┐
	*       │               │          │
	*      para              │    CRC   │                                    
	*       │               │          │                                                                                     
  *                                                                               
  */
void ResParaRequest(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CDV_INT08U* para, CDV_INT08U paraLen, CDV_INT08U uartNo)	{
	CDV_INT08U rxRealLen;
	CDV_INT08U txLen;
	CDV_INT08U *txBuf = NULL;
	//CDV_INT08U *txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(txLen));
	if(NULL != rxBuf)
	{
		rxRealLen = rxLen + 3 + 3;
		txLen = paraLen + rxRealLen;
		NEW08U(txBuf,txLen);
	  MemCpy(txBuf , rxBuf - 3 - 3, rxRealLen);
		
	}
	else
	{
		rxRealLen = 0;
		txLen = paraLen;
		NEW08U(txBuf,txLen);
	}
	
	if(paraLen > 0)
		MemCpy(txBuf + rxRealLen , para , paraLen);
	
	AddTx(txBuf , txLen , uartNo);
	DELETE(txBuf);
//	free(txBuf);
//	txBuf = NULL;
	
//	resReqed = 1;
}
/** @brief  资源反馈(新版)。
  * @param  rxBuf     原始字符串
  *         rxLen     ↑长度
	*         para      附加参数字符串
	*         paraLen   ↑长度
  * @retval 无
  * @note   最终发送的字符串如下
	*                 ┌──rxLen byte──┬   paraLen   ┬   2byte  ┐
	*                                                    │          │
	*               rxBuf                para            │    CRC   │                                    
	*                 │                  │             │          │                                                                                                                                             
  *                                                                               
  */
void ResRequest2(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CDV_INT08U* para, CDV_INT08U paraLen, CMD_ARG *arg)	{
	
	CDV_INT08U txLen = 0;
	CDV_INT08U *txBuf = NULL;
	
	if(NULL != rxBuf)
	{
		txLen += rxLen;
	}
	if(NULL != para)
	{
		txLen += paraLen;
	}
	NEW08U(txBuf,txLen);
	
	if(rxLen > 0)
	  MemCpy(txBuf , rxBuf , rxLen);
	
	
	if(paraLen > 0)
		MemCpy(txBuf + rxLen , para , paraLen);
	
//	AddTx(txBuf , txLen , uartNo);
//	DELETE(txBuf);
	
	arg->reqbuf = txBuf;
	arg->reqlen = txLen;
	
//	resReqed = 1;
}
/** @brief  资源反馈(新版)。
  * @param  rxBuf     原始字符串
  *         rxLen     ↑长度
	*         para      附加参数字符串
	*         paraLen   ↑长度
            ctrl      0无 1 crc
  * @retval 无
  * @note   最终发送的字符串如下
	*                 ┌──rxLen byte──┬   paraLen   ┬   2byte  ┐
	*                                                    │          │
	*               rxBuf                para            │    CRC   │                                    
	*                 │                  │             │          │                                                                                                                                             
  *                                                                               
  */
void ResRequest(CDV_INT08U* rxBuf, CDV_INT16U rxLen , CDV_INT08U* para, CDV_INT16U paraLen, CMD_ARG *arg, REQUEST_CTRL ctrl)	{
	
	CDV_INT16U txLen = 0;
	CDV_INT16U crc = 0xFFFF;
	CDV_INT08U *txBuf = NULL;
	
	if(NULL != rxBuf)
	{
		txLen += rxLen;
	}
	if(NULL != para)
	{
		txLen += paraLen;
	}
	if(ctrl) // crc 暂定
	{
		txLen += 2;
	}
	
	NEW08U(txBuf,txLen);
	
	if(rxLen > 0)
	  MemCpy(txBuf , rxBuf , rxLen);
	
	
	if(paraLen > 0)
		MemCpy(txBuf + rxLen , para , paraLen);
	
	if(ctrl) {
	  crc = MODBUS_CRC16(txBuf, rxLen + paraLen, crc);
		MemCpy(txBuf + rxLen + paraLen, &crc, 2);
	}
	
	arg->reqbuf = txBuf;
	arg->reqlen = txLen;
}
//void ResRequest(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CDV_INT08U* para, CDV_INT08U paraLen, CMD_ARG *arg)	{
//	
//	CDV_INT08U txLen = 0;
//	CDV_INT08U *txBuf = NULL;
//	
//	if(NULL != rxBuf)
//	{
//		txLen += rxLen;
//	}
//	if(NULL != para)
//	{
//		txLen += paraLen;
//	}
//	NEW08U(txBuf,txLen);
//	
//	if(rxLen > 0)
//	  memcpy(txBuf , rxBuf , rxLen);
//	
//	
//	if(paraLen > 0)
//		memcpy(txBuf + rxLen , para , paraLen);
//	
//	AddTx(txBuf , txLen , uartNo);
//	DELETE(txBuf);
//	
//	resReqed = 1;
//}

/** @brief  将缓存中的命令添加到串口。
  * @param  
  * @retval 无
  * @note                                                                                                                                           
  *                                                                               
  */
RET_STATUS DoResRequest(CMD_ARG *arg)	{

	if(NULL == arg->reqbuf || 0 == arg->reqlen) {
		return OPT_FAILURE;
	}
  AddTxNoCrcPlus(arg->reqbuf , arg->reqlen , arg);
//	AddTx(arg->reqbuf , arg->reqlen , arg->uart);
	DELETE(arg->reqbuf);
	arg->reqlen = 0;
//	resReqed = 0;
	
	return OPT_SUCCESS;
}

/** @brief  处理特殊记录。
  * @param  
  * @retval 无
  * @note                                                                                                                                           
  *                                                                               
  */
RET_STATUS DoSpecRequest(CMD_ARG *arg)	{

	if(NULL == arg->specbuf || 0 == arg->speclen)
	{
		return OPT_FAILURE;
	}

	AddTxPlus(arg->specbuf , arg->speclen , arg);
	DELETE(arg->specbuf);
	arg->speclen = 0;
	
	return OPT_SUCCESS;
}


///** @brief  变量写，资源调用。
//  * @param  
//  * @retval 无
//  * @note                                                                                                                                           
//  *                                                                               
//  */
//void ResVarSet(CDV_INT32U no, CDV_INT32S num, CMD_ARG *arg)	{

//	ValSet(no, num);
//	arg->speclen = 12;
//	NEW08U(arg->specbuf, arg->speclen);
//	memcpy(arg->specbuf, "VARSET:", 7);
//	memcpy(arg->specbuf + 7, &no, 1);
//	memcpy(arg->specbuf + 8, &num, 4);
//	
//	return;
//}

/** @brief  变量写，资源调用。
  * @param  
  * @retval 无
  * @note                                                                                                                                           
  *                                                                               
  */
void ResVarSet(CDV_INT32U no, CDV_INT32S num, CMD_ARG *arg)	{

	u8 type = 0x05;
	ValSet(no, num);
	arg->speclen = arg->len + 7;
	NEW08U(arg->specbuf, arg->speclen);
	MemCpy(arg->specbuf, arg->buf, arg->len);
	MemCpy(arg->specbuf + arg->len, &type, 1);
	MemCpy(arg->specbuf + arg->len + 1, &no, 2);
	MemCpy(arg->specbuf + arg->len + 3, &num, 4);
	
	return;
}


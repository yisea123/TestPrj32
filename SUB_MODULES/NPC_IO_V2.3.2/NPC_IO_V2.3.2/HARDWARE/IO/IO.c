/**
  ******************************************************************************
  * @file    /IO.c 
  * @author  MMY
  * @version V0.0.1
  * @date    1-12-2015
  * @brief   a package of io and dip function
  * 
@verbatim  

               
@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2015 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "IO.h"

CDV_IO g_cdvI[CDV_I_NUM];
CDV_IO g_cdvO[CDV_O_NUM];
CDV_IO g_dipSW[CDV_DIP_NUM];
CDV_INT08U g_dip = 0;
CDV_INT08U* g_i = (CDV_INT08U*)g_modbusInCoil.coilCh;
CDV_INT08U* g_o = (CDV_INT08U*)g_modbusOCoil.coilCh;

RET_STATUS ResParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CMD_ARG *arg);
RET_STATUS OnlineParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg);
RET_STATUS NPC1Parse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg);


/*GPIO设置*/

void NPC_GpioConfig(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE );	  //使能GPIOA GPIOB GPIOC GPIOD通道时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	 	  // 端口配置PA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;	 	// 端口配置PA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 	// 端口配置PB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	 	// 端口配置PB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	 	// 端口配置PC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	 	// 端口配置PC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	    //关闭调试 端口重新映射  使用仿真器调试时，不能用此语  SWJ 完全禁用
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //关闭调试 端口重新映射  使用仿真器调试时，不能用此语  JTAG-DP 禁用 + SW-DP 使能
	GPIO_SetBits(GPIOA,GPIO_Pin_All);
	GPIO_SetBits(GPIOB,GPIO_Pin_All);
	GPIO_SetBits(GPIOC,GPIO_Pin_All);
	GPIO_SetBits(GPIOD,GPIO_Pin_All);
}

void NPC_IOInit(void) {
	
//	O初始化
	g_cdvO[0].port = PORT_C;
	g_cdvO[0].pin  = PIN_4;
	g_cdvO[1].port = PORT_C;
	g_cdvO[1].pin  = PIN_5;
	g_cdvO[2].port = PORT_B;
	g_cdvO[2].pin  = PIN_0;
	g_cdvO[3].port = PORT_B;
	g_cdvO[3].pin  = PIN_1;
	g_cdvO[4].port = PORT_B;
	g_cdvO[4].pin  = PIN_10;
	g_cdvO[5].port = PORT_B;
	g_cdvO[5].pin  = PIN_11;
	g_cdvO[6].port = PORT_B;
	g_cdvO[6].pin  = PIN_12;
	g_cdvO[7].port = PORT_B;
	g_cdvO[7].pin  = PIN_13;
	g_cdvO[8].port = PORT_B;
	g_cdvO[8].pin  = PIN_14;
	g_cdvO[9].port = PORT_B;
	g_cdvO[9].pin  = PIN_15;
	g_cdvO[10].port = PORT_C;
	g_cdvO[10].pin  = PIN_6;
	g_cdvO[11].port = PORT_C;
	g_cdvO[11].pin  = PIN_7;
	g_cdvO[12].port = PORT_C;
	g_cdvO[12].pin  = PIN_8;
	g_cdvO[13].port = PORT_C;
	g_cdvO[13].pin  = PIN_9;
	g_cdvO[14].port = PORT_A;
	g_cdvO[14].pin  = PIN_8;
	
//	I初始化

	g_cdvI[0].port = PORT_B;
	g_cdvI[0].pin  = PIN_9;
	g_cdvI[1].port = PORT_B;
	g_cdvI[1].pin  = PIN_8;
	g_cdvI[2].port = PORT_B;
	g_cdvI[2].pin  = PIN_7;
	g_cdvI[3].port = PORT_B;
	g_cdvI[3].pin  = PIN_6;
	g_cdvI[4].port = PORT_B;
	g_cdvI[4].pin  = PIN_5;
	g_cdvI[5].port = PORT_B;
	g_cdvI[5].pin  = PIN_4;
	g_cdvI[6].port = PORT_B;
	g_cdvI[6].pin  = PIN_3;
	g_cdvI[7].port = PORT_D;
	g_cdvI[7].pin  = PIN_2;
	g_cdvI[8].port = PORT_C;
	g_cdvI[8].pin  = PIN_12;
	g_cdvI[9].port = PORT_C;
	g_cdvI[9].pin  = PIN_11;
	g_cdvI[10].port = PORT_C;
	g_cdvI[10].pin  = PIN_10;
	g_cdvI[11].port = PORT_A;
	g_cdvI[11].pin  = PIN_15;
	g_cdvI[12].port = PORT_A;
	g_cdvI[12].pin  = PIN_12;
	g_cdvI[13].port = PORT_A;
	g_cdvI[13].pin  = PIN_11;
		
//	拨码开关初始化
	
	g_dipSW[0].port = PORT_A;
	g_dipSW[0].pin  = PIN_0;
	g_dipSW[1].port = PORT_A;
	g_dipSW[1].pin  = PIN_1;
	g_dipSW[2].port = PORT_A;
	g_dipSW[2].pin  = PIN_7;
	
}

/**
  * @brief  I参数设置
  *  
  * @param  no     要设置的I号
  *         flag   设置的参数标记
						buf    参数buf
  *   
  * @retval 无
  *
  * @note   
  */
void IParWrite(CDV_INT32U no , CDV_INT16U flag , CDV_INT08U* buf) {
	CDV_INT08U i = 0 , len = 0;
	CDV_INT16U sf   = 0x0001;
	CDV_INT32U addr = IPAR_ADDR + no * 2;
	for ( i = 0; i < I_PAR_NUM ; i++) {
		if (flag & (sf<<i)) {
//			SPI_Flash_Write(buf+len, addr+i, 1);
			len ++;
		}
	}
}

/**
  * @brief  I参数读取
  *  
  * @param  no     要设置的I号
  *         flag   设置的参数标记
						buf    参数buf
  *   
  * @retval 无
  *
  * @note   
  */
void IParRead(CDV_INT32U no , CDV_INT16U flag , CDV_INT08U* buf , CDV_INT08U* len) {
	CDV_INT08U i = 0;
	CDV_INT16U sf   = 0x0001;
	CDV_INT32U addr = IPAR_ADDR + no * 2;
	*len = 0;
	for ( i = 0; i < I_PAR_NUM ; i++) {
		if (flag & (sf<<i)) {
//			SPI_Flash_Read(buf+(*len), addr+i, 1);
			(*len)++;
		}
	}
}


/**
  * @brief  CDV 读I 30
  *  
  * @param  no     要读的I号
  *   
  * @retval CDV_INT08U：1高电平、0低电平
  *
  * @note   刷新modbus线圈
  */
CDV_INT08U ExIRead(CDV_INT32U no) {
	CDV_INT08U val;
//	val = FpgaIRead(no);
	return val;
}
/**
  * @brief  CDV 读O 34
  *  
  * @param  no     要读的O号
  *   
  * @retval CDV_INT08U：1高电平、0低电平
  *
  * @note   刷新modbus线圈
  */
CDV_INT08U ExORead(CDV_INT32U no) {
	CDV_INT08U val;
//	val = FpgaORead(no);
  return val;
}
/**
  * @brief  CDV O输出 34
  *  
  * @param  no     要读的I号
  *         val    CDV_INT08U：1高电平、0低电平
  *   
  * @retval 无
  *
  * @note   刷新modbus线圈
  */
void ExOWrite(CDV_INT08U no , CDV_INT08U val) {
//	FpgaOWrite(no , val);
}
/**
  * @brief  CDV 读I(包含EXI
  *  
  * @param  no     要读的I号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   刷新modbus线圈
  */
IO_VAL IRead(CDV_INT32U no) {
	CDV_INT08U val;
	if(no >= CDV_I_NUM) {
		val = ExIRead(no - CDV_I_NUM);
	} else {
		val = GPIO_ReadInputDataBit(g_cdvI[no].port , g_cdvI[no].pin);
	}	
	
	switch((IO_VAL)val) {
		case BIT_1:
			SET_I(no);
			break;
		case BIT_0:
			RESET_I(no);
			break;
		default:
			break;
	}
		
	return (IO_VAL)val;
}

/**
  * @brief  CDV 读I(包含EXI
  *  
  * @param  no     要读的I号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   读取的是npc端口的bit值，bit1对应1，bit0对应0
  */
CDV_INT08U IBitRead(CDV_INT32U no) {
	CDV_INT08U val;
	IO_VAL bit = IRead(no);
	val = (BIT_1 == bit ? 1 : 0);
	return val;
}

/**
  * @brief  CDV 读I,防抖(包含EXI
  *  
  * @param  no     要读的I号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   刷新modbus线圈
20151225:增加了判断cdv状态，如果不是运行，就退出while，没有判断暂停
  */
IO_VAL ShakeIRead(CDV_INT32U no) {
	CDV_INT08U cnt = 0,freq = 0, sleep = 0 ,len;
	IO_VAL bit , bitLast;
	IParRead(no , 0x0001 , &freq , &len);
	if(1!=len)
//		WhileError();
	IParRead(no , 0x0002 , &sleep , &len);
	if(1!=len)
//		WhileError();

	do {
//		if(no >= CDV_IO_NUM) {
//		  bit = ExIRead(no - CDV_IO_NUM);
//		} else {
//			bit = GPIO_ReadInputDataBit(g_cdvI[no].port , g_cdvI[no].pin) ? BIT_1 : BIT_0;
//		}	
		bit = IRead(no);
		
		if (cnt > 0) {//判断前后读的是不是相等的
			if (bit != bitLast) {
				cnt = 0;
			}
		}
		
		bitLast = bit;
//		TimeDelay(0 , sleep);
	} while (++cnt < freq);
	
	return bit;
}

/**
  * @brief  CDV 读O（包含EXO
  *  
  * @param  no     要读的O号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   刷新modbus线圈
  */
IO_VAL ORead(CDV_INT32U no) {
	CDV_INT08U val;
	if(no >= CDV_O_NUM) {
		val = ExORead(no - CDV_O_NUM);
	} else {
		val = GPIO_ReadInputDataBit(g_cdvO[no].port , g_cdvO[no].pin);
	}
	
	switch((IO_VAL)val) {
		case BIT_1:
			SET_O(no);
			break;
		case BIT_0:
			RESET_O(no);
			break;
		default:
			break;
	}
		
	return (IO_VAL)val;
}
/**
  * @brief  CDV 读O（包含EXO
  *  
  * @param  no     要读的O号
  *   
  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *
  * @note   读取的是npc端口的bit值，bit1对应1，bit0对应0
  */
CDV_INT08U OBitRead(CDV_INT32U no) {
	CDV_INT08U val;
	IO_VAL bit = ORead(no);
	val = (BIT_1 == bit ? 1 : 0);
	return val;
}
/**
  * @brief  CDV O输出(包含EXO
  *  
  * @param  no     要读的I号
  *         val    IO_VAL枚举值：BIT_1高电平、BIT_0低电平
  *   
  * @retval 无
  *
  * @note   刷新modbus线圈
  */
void OWrite(CDV_INT32U no , IO_VAL ioVal) {
	
	CDV_INT08U val = ioVal;
	
	switch(ioVal){
		case BIT_1:
			SET_O(no);
			break;
		case BIT_0:
			RESET_O(no);
			break;
		default:
			break;
	}
	
	if(no >= CDV_O_NUM) {
		ExOWrite(no - CDV_O_NUM, val);
	} else {
		GPIO_WriteBit(g_cdvO[no].port , g_cdvO[no].pin, val);
	}
}
///**
//  * @brief  CDV 读拨码开关
//  *  
//  * @param  no     要读的DIP号
//  *   
//  * @retval IO_VAL枚举值：BIT_1高电平、BIT_0低电平
//  *
//  * @note   刷新modbus线圈
//  */
IO_VAL DIPRead(CDV_INT08U no) {
	CDV_INT08U val;
	val = GPIO_ReadInputDataBit(g_dipSW[no].port , g_dipSW[no].pin);
	if(val) {
	  //	  MODBUS_SET_DIP(no);
		return BIT_1;
	}	else {
	  //	  MODBUS_RESET_DIP(no);
		return BIT_0;
	}		
}


////////////////////////////////////////////////////////////////////

/**
  * @brief  I脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明
            OPT_SUCCESS命令执行完成
            OPT_FAILURE命令未执行完成，比如等待未等到
  *
  * @note   资源号（00-FF）	00等待	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
  */

RET_STATUS ICmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) { 
//	OS_ERR err;
	CDV_INT08U opt, type;
	CDV_INT32U no , num , val;
	IO_VAL bit;
  RET_STATUS ret =OPT_SUCCESS;
	
	CDV_INT08U tmpPar[I_PAR_NUM] = {0} , tmpLen = 0;
//  if(1 == debug) {
//		return ret;
//	}
//	//strLen = rxLen - 3;//中文长度
 // OS_ERR err;

	no  = *(CDV_INT32U*)(rxBuf);//rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	num  = *(CDV_INT32U*)(rxBuf + 6);

	switch(opt) {
		case 0x00:/*等待*/
//			  num = Arithmetic((char*)rxBuf + 6, rxLen - 6);
		    bit = (num)?BIT_1:BIT_0;
		    if(bit != IRead(no))
				  ret = OPT_FAILURE;
			break;
		case 0x01:/*抖动等待*/
		    bit = (num)?BIT_1:BIT_0;
		    if(bit != ShakeIRead(no))
				ret = OPT_FAILURE;
			break;
		case 0x02:/*查询*/
			bit = IRead(no);
		  val = (BIT_1 == bit ? 1 : 0);
//			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg);
		  break;
		case 0x03:/*抖动查询*/
			bit = ShakeIRead(no);
			val = (BIT_1 == bit ? 1 : 0);
//			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg);
			break;
		case 0x04:/*读取到变量*/
			if(0xFE == type) {
				bit = IRead(no);
			  val = (BIT_1 == bit ? 1 : 0);
//				ResVarSet(num, val, arg);
			}
			break;
		case 0x05:/*参数设置*/
			if(0xFF == type) {
			  IParWrite(no , num , rxBuf + 10);
			}
			break;
		case 0x06:/*参数读取*/			
			if(0xFF == type) {
			  IParRead(no , num , tmpPar , &tmpLen);
//			  ResRequest(arg->buf, arg->len, tmpPar, tmpLen, arg);
			}
			break;
		case 0x07:/*跳变等待*/
//			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
			ret = OPT_FAILURE;
//			bit = READ_PLUSE_I(no);
			if(bit != IRead(no))
			{
				//delay_ms(5);
				if(bit != IRead(no))
				{
					if(BIT_0 == bit)
					{
//						SET_PLUSE_I(no);
					}
					else
					{
//						RESET_PLUSE_I(no);
					}
					ret = OPT_SUCCESS;
				}
			}
//			OSSemPost (&PLUSE_SEM,OS_OPT_POST_1,&err);
// 		    bit = IRead(no);
// 		    while(bit == IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x08:/*上跳变等待*/
//			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
			ret = OPT_FAILURE;
//			bit = READ_PLUSE_I(no);
			if(bit != IRead(no))
			{
				//delay_ms(5);
				if(bit != IRead(no))
				{
					if(BIT_0 == bit)
					{
						ret = OPT_SUCCESS;
//						SET_PLUSE_I(no);
					}
					else
					{
//						RESET_PLUSE_I(no);
					}
				}
			}
//			OSSemPost (&PLUSE_SEM,OS_OPT_POST_1,&err);
				
// 		    while(BIT_0 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
// 		    while(BIT_1 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x09:/*下跳变等待*/
//			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //请求信号量
			ret = OPT_FAILURE;
//			bit = READ_PLUSE_I(no);
			if(bit != IRead(no))
			{
				//delay_ms(5);
				if(bit != IRead(no))
				{
					if(BIT_0 == bit)
					{
//						SET_PLUSE_I(no);
					}
					else
					{
						ret = OPT_SUCCESS;
//						RESET_PLUSE_I(no);
					}
				}
			}
//			OSSemPost (&PLUSE_SEM,OS_OPT_POST_1,&err);
// 		    while(BIT_1 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
// 		    while(BIT_0 != IRead(no)  && ( CDV_STOP != g_cdvStat))
// 					delay_ms(1);
			break;
		case 0x0A:/*去抖读取到变量*/
			if(0xFE == type) {
				bit = ShakeIRead(no);
			  val = (BIT_1 == bit ? 1 : 0);
				//ValSet(num, ShakeIRead(no));
//				ResVarSet(num, val, arg);
			}
		default:
			break;
	}

	return ret;
}

/**
  * @brief  O脚本命令
  *  
  * @param  scriptNo     待分析工人号  
  *   
  * @retval 返回值说明OPT_SUCCESS，OPT_FAILURE
  *
  * @note   资源号（00-FF）	00输出	枚举号（00-03	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	枚举值	字符串
		                                FE	变量代号高位	变量代号低位							

  */

RET_STATUS OCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) { 
//	OS_ERR err;
  //CDV_INT08U no , opt, type, val  /*, strLen*/;
	CDV_INT08U opt, type;
	CDV_INT32U num , no, val;
	IO_VAL bit;
	//OS_ERR err;

//	//strLen = rxLen - 3;//中文长度
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];	
	no  = *(CDV_INT32U*)(rxBuf);//rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	num  = *(CDV_INT32U*)(rxBuf + 6);

	switch(opt) {
		case 0x00:/*输出*/			
//			switch(type) {				
//				case 0xFE:
//					bit = (VarGet(num))?BIT_1:BIT_0;
//					OWrite(no,bit);
//					break;
//				default:
//					bit = (num)?BIT_1:BIT_0;
//					OWrite(no, bit);
//					break;
//			}
//			bit = (Arithmetic((char*)rxBuf + 6, rxLen - 6))?BIT_1:BIT_0;
			OWrite(no, bit);
			break;
		case 0x01:/*等待*/
//			num = Arithmetic((char*)rxBuf + 6, rxLen - 6);
		  bit = (num) ? BIT_1 : BIT_0;
		  if(bit != ORead(no))
				return OPT_FAILURE;
		  break;
		case 0x04:/*查询*/
			bit = ORead(no);
		  val = (BIT_1 == bit ? 1 : 0);
//			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg);
		  break;
		case 0x02:/*读取到变量*/
			if(0xFE == type) {
				bit = ORead(no);
		    val = (BIT_1 == bit ? 1 : 0);
			  //DW_CDV_VAR(rxBuf[3]) = ORead(no);
				//ValSet(num, ORead(no));
//			ResVarSet(num, val, arg);
			}
			break;
		default:
			break;
		
	}

	return OPT_SUCCESS;
}

//********************************************************
//整个I口状态读取
//num:第几个I口，1-20
//********************************************************
u16 Read_Input_All()
{
	u16 state=0xffff;
  u8 i , S[15]={0};
	for(i=0;i<14;i++)
	{
	 S[i] = GPIO_ReadInputDataBit(g_cdvI[i].port,g_cdvI[i].pin);
	}
state = (S[0]<<15)|(S[1]<<14)|(S[2]<<13)|(S[3]<<12)|(S[4]<<11)|(S[5]<<10)|(S[6]<<9)
				|(S[7]<<8)|(S[8]<<7)|(S[9]<<6)|(S[10]<<5)|(S[11]<<4)|(S[12]<<3)
				|(S[13]<<2);
state =  state|0x0003;	
	{
	int i=0;
	for(i = 0 ; i < CDV_I_NUM/* + CDV_EXO_NUM*2 */; i ++) {
		IRead(i);
	}
}
return state;
}

//********************************************************
//整个O口状态读取
//num:第几个I口，1-22
//********************************************************
u16 Read_Output_ALL()
{
	u16 state = 0xffff;
	u8 i , S[15]={0};
	 for(i=0;i<15;i++)
	{
	S[i] = GPIO_ReadOutputDataBit(g_cdvO[i].port,g_cdvO[i].pin);
	}
	state = (S[0]<<15)|(S[1]<<14)|(S[2]<<13)|(S[3]<<12)|(S[4]<<11)|(S[5]<<10)|(S[6]<<9)
				|(S[7]<<8)|(S[8]<<7)|(S[9]<<6)|(S[10]<<5)|(S[11]<<4)|(S[12]<<3)
				|(S[13]<<2)|(S[14]<<1);
	{
	int i=0;
	for(i = 0 ; i < CDV_O_NUM/* + CDV_EXO_NUM*2 */; i ++) {
		ORead(i);
	}
}
	return state;
}

//设置输出
void OUTPUT(void)
{ u8 i;
	 for(i=0;i<15;i++)
	{
			if(READ_ID == 001)
		
			GPIO_ResetBits(g_cdvO[i].port,g_cdvO[i].pin);
		else 
			GPIO_SetBits(g_cdvO[i].port,g_cdvO[i].pin); 
			
		}	
	}




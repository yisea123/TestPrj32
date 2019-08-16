
/**
  ******************************************************************************
  * @file    /modbus.c 
  * @author  MMY
  * @version V2.0.0
  * @date    4-July-2015
  * @brief   modbus function and var and 
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
	
#include "modbus.h" 
#include  <stdlib.h>

MODBUS_OCoil g_modbusOCoil={0};
MODBUS_Input_Coil g_modbusInCoil={0};
MODBUS_Register g_modbusReg={0};
MODBUS_Input_Register g_modbusInReg={0};
CDV_INT08U  Modbus_TX_len;
CDV_INT08U Modbus_addr=0x01;//本机地址

 CDV_INT08U Debug_mode;
 CDV_INT08U Modbus_slave_addr;    //目标从机地址
 CDV_INT08U Function_Code;		//主机模式功能码
 CDV_INT16U Modbus_slave_reg_addr;	//从机寄存器地址
 CDV_INT16U Modbus_slave_reg_num;	//从机寄存器数
 CDV_INT16U Modbus_reg_val1;//写入从机寄存器的值，若Modbus_slave_reg_num=1，则写入的是16位数据，Modbus_reg_val2无效
 CDV_INT16U Modbus_reg_val2;//若Modbus_slave_reg_num=2，则写入的是32位数据，Modbus_reg_val1为高16，Modbus_reg_val2位低16
 CDV_REG08 Modbus_recevie_flag=0;
 CDV_REG32 Modbus_timeout_cnt=0;
 CDV_REG16 Modbus_SendWait_Cnt = 0;//等待发送计时
 CDV_INT16U Modbus_SendWait_Time = 0;//等待发送时间
//extern CDV_INT08U Modbus_RX_buf[Modbus_max_recevie_buf];
 CDV_INT08U Modbus_RX_buf[Modbus_max_recevie_buf];
 CDV_REG08 ModbusCmdBuf_Cnt,ModbusCmdBuf_CntMax,ModbusCmdExecution_State;
 CDV_INT08U Modbus_TX_buf[Modbus_max_send_buf];
//extern CDV_INT08U Modbus_RX_len;
 CDV_INT08U Modbus_RX_len;
 CDV_REG08 Modbus_cmd_flag,Modbus_exe_flag,Modbus_Process_Flag;
 CDV_INT08U Modbus_SendWait_flag;
 CDV_REG16 Modbus_SendWait_Cnt;
 CDV_INT08U USART_TX_BUF_ADDR[Modbus_max_send_buf];
 CDV_INT08U USART_TX_BUF[Modbus_max_send_buf];
 CDV_REG08 Tx_flag;
 CDV_INT08U Modbus_ReadRegister_num;//?????????
 CDV_INT16U Modbus_ReadRegister_value[100];//?????????
 CDV_INT08U Modbus_ReadRegister_ture,AlarmB_Flag_last;
 CDV_INT08U Modbus_local_UI;
 float TEMP_last;
 CDV_INT08U Command_Number;//MODBUS???????
 CDV_INT08U Interface_Number,Interface_Number0,Interface_Number_last;//MODBUS??????????
 CDV_REG08 All_Parameters_flag;

//#define Modbus_addr  0x01  //本机地址
 CDV_INT08U Modbus_addr;//本机地址
 CDV_INT08U Modbus_mode;//0:从机模式 1:主机模式
 CDV_INT16U Ctrl_commond_addr;
 CDV_INT16U Ctrl_num;
 CDV_INT08U slave_number[10],HeartBeat_slave_request_flag,slave_number_dis[10];
 CDV_REG08 HeartBeat_flag,Interface_refresh_flag;//心跳包发送标志
 CDV_REG16 HeartBeat_timeout;//心跳包发送间隔计数
 CDV_REG08	slave_display;//从机号显示标志
 CDV_REG16	slave_count;//从机号显示计时
 CDV_REG08	slave_display2;	
 MODBUS_Coil g_modbusCoil;
 MODBUS_Register g_modbusReg;
 
 
 void ModbusRequest(CDV_INT08U no, CDV_INT08U errNo);
/**
  * @brief  对字符串进行modbus分析
  *  
  * @param  rxBuf     待分析字符串指针
  *         len       待分析字符串的长度
  *         txBuff    应答字符串  
  *   
  * @retval 返回值说明
  *
  * @note   RecvParse中
  */
CDV_INT08U ModbusParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen)
{	
	if(rxBuf[0]==Modbus_addr)
	{
		switch(rxBuf[1]){ //命令码筛选
			case 1://读线圈
				ReadCoil(rxBuf,rxLen);
				break;
			case 2://读输入线圈
				ReadInCoil(rxBuf,rxLen);
				break;
			case 3://读寄存器
				ReadRegister(rxBuf,rxLen);
				break;
			case 4://读输入寄存器
				//ReadInRegister(rxBuf,rxLen);
				break;
			case 5://写单线圈
				WriteCoil(rxBuf,rxLen);
				break;
			case 6://预置单寄存器
				WriteRegister(rxBuf,rxLen);
				break;
			case 7://读取异常状态
				break;
			case 8://回送诊断校验
				break;
			case 9://编程
				break;
			case 10://控询
				break;
			case 11://读取事件计数
				break;
			case 12://读取通信事件记录
				break;
			case 15://写多线圈
				WriteMultiCoil(rxBuf,rxLen);
				break;
			case 16://预置多寄存器
				WriteMultiRegister(rxBuf,rxLen);
				break;
			case 17://报告从机标志
				break;
			case 22://扩展功能------CSB功能
				break;
			default: //命令码无效不应答
//				ModbusRequest(rxBuf[1], 1);
				break;
		}
		return 1;
	}
	return 0;
}
///**
//  * @brief  不需要回复的串口命令发送准备函数
//  *         
//  *  
//  * @param  txBuf     发送字符串首地址指针
//  *         txLen     发送字符串长度,不包括crc
//  *           
//  *   
//  * @retval 返回值说明
//  *
//  * @note   
//  */
void AddTx(CDV_INT08U* txBuf, CDV_INT08U txLen) 
	{
	//OS_ERR err;
	CDV_INT08U len = txLen + 2;
	if(NULL == txBuf || 0 == txLen)
		return;
	//if(USART_CAN_DO){
		
	//	USART_TX_ADD_WITH_LEN(len);//开辟空间	
		//memcpy(USART_TX_BUF_ADDR, txBuf , txLen);		
		RequestAdd(txBuf, len);
	//	USART_TX_QUEUE_SELF_ADD;			
	}
void RequestAdd(CDV_INT08U* txBuf,CDV_INT08U txLen){
	CDV_INT16U crc;
	Modbus_TX_len=txLen;
	
	USART_TX_BUF[0] = txBuf[0];
	USART_TX_BUF[1] = txBuf[1];
	crc = getCRC16(USART_TX_BUF,txLen-2);
	USART_TX_BUF[txLen-2] = crc & 0xff; 
	USART_TX_BUF[txLen-1] = (crc >> 8) & 0xff;
}

/**
  * @brief  modbus读线圈
  *         01
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  */
void ReadCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//获取位状态
	
	CDV_INT08U i, len ,txbuf[20];
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	num = (rxBuf[4]<<8) + rxBuf[5];
	if(0x0001 >num || num> 0x002F)
	{
		ModbusRequest(rxBuf[1] , 3);
		return;
	} 
	else if(addr > 0xFF || addr + num > 0xFF)
	{
		ModbusRequest(rxBuf[1] , 2);
		return;
	}
	
	sta = (addr >> 3) /*+ 3*/;//读线圈的初始char
	sf = addr & 0x07;
	numCh = (num>>3)+((num & 0x07)?1:0);//读char数量
	
	
	len = 5 + numCh;//tx总长度
	//USART_TX_ADD_WITH_LEN(len);//开辟空间
	USART_TX_BUF[2] = numCh;
	//所处字节
	for(i=0;i<USART_TX_BUF[2]+1;i++){//多操作一位，防止最后一个字节少读位
				USART_TX_BUF[3+i] = MODBUS_COIL_CH(sta+i);
			}
	//偏移
	for(i=0;i<USART_TX_BUF[2];i++){
				USART_TX_BUF[3+i] = USART_TX_BUF[3+i]>>sf | USART_TX_BUF[4+i]<<(8-sf);
			}
	if(/*sf != 0 && */i!= 0)
		USART_TX_BUF[3+i-1] &= (0xFF>>(8 - (0 ==(num & 0x07)?8:(num & 0x07)))); // 补0
	#if MULTIDEVICE_TEST
		txbuf[0]=rxBuf[0];
	#else
		txbuf[0]=READ_ID;
	#endif
	txbuf[1]=0x01;
	
	AddTx(txbuf, len-2);
	memcpy(&txbuf[2] , &USART_TX_BUF[2] , len);
	USART2_Send(txbuf,len);
 // RequestAdd(rxBuf, len);
	//USART_TX_QUEUE_SELF_ADD;			
}

/**
  * @brief  modbus读输入线圈
  *         02
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  */
void ReadInCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//获取位状态
	
	CDV_INT08U i, len ,txbuf[20];
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	num = (rxBuf[4]<<8) + rxBuf[5];//读i数量
	
	if(0x0001 >num || num> 0x002F){
		ModbusRequest(rxBuf[1] , 3);
		return;
	} else if(addr > 0xFF || addr + num > 0xFF) {
		ModbusRequest(rxBuf[1] , 2);
		return;
	}
	
	sta = (addr >> 3) /*+ 3*/;//读线圈的初始char
	sf = addr & 0x07;
	numCh = (num>>3)+((num & 0x07)?1:0);//读char数量
	len = 5 + numCh;//tx总长度
	//USART_TX_ADD_WITH_LEN(len);//开辟空间
	USART_TX_BUF[2] = numCh;
	//所处字节
	for(i=0;i<USART_TX_BUF[2]+1;i++){//多操作一位，防止最后一个字节少读位
				USART_TX_BUF[3+i] = MODBUS_INCOIL_CH(sta+i);
			}
	//偏移
	for(i=0;i<USART_TX_BUF[2];i++){
				USART_TX_BUF[3+i] = USART_TX_BUF[3+i]>>sf | USART_TX_BUF[4+i]<<(8-sf);
			}
	if(/*sf != 0 &&*/ i!= 0)
		USART_TX_BUF[3+i-1] &= (0xFF>>(8 - (0 ==(num & 0x07)?8:(num & 0x07))));// 补0
	#if MULTIDEVICE_TEST
		txbuf[0]=rxBuf[0];
	#else
	//txbuf[0]=Modbus_addr;
	  txbuf[0]=READ_ID;
	#endif
	txbuf[1]=0x02;
	
	AddTx(txbuf, len-2);
	memcpy(&txbuf[2] , &USART_TX_BUF[2] , len);
	USART2_Send(txbuf,len);
//  RequestAdd(rxBuf, len);
//	AddTx(txbuf, len-2);
	//USART_TX_QUEUE_SELF_ADD;			
}

/**
  * @brief  modbus读寄存器
  *         03
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  */
void ReadRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//获取位状态
	
	CDV_INT08U i, len ,txbuf[2];
	CDV_INT16U addr, num;
	addr = (rxBuf[2]<<8) + rxBuf[3];
//	if(addr>=0x1000 && addr<0x1FFF) {
//		addr = ((addr>>4)&0x00f0 ) | ((addr) &0x000f);
//	}
	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(0x0001 >num || num> 0x008D){
		//while(1);
		ModbusRequest(rxBuf[1] , 3);
		return;
	} else if(addr > 0x1000 || addr + num > 0x1200) {
		//while(1);
		ModbusRequest(rxBuf[1] , 2);
		return;
	}
	
	len = 5 + (num<<1);//tx总长度
	//USART_TX_ADD_WITH_LEN(len);//开辟空间
	USART_TX_BUF[2] = num<<1;
	//所处字节
//	Modbus_EleParm_Measure(addr);
//	ReadRegisterCmdParse(addr,num);
	for(i=0;i<num;i++){
		USART_TX_BUF[3 +(i<<1)] = HIGH16U(MODBUS_REG_ADDR(addr + i));
		USART_TX_BUF[3 +(i<<1)+ 1] = LOW16U(MODBUS_REG_ADDR(addr + i));
	}
	#if MULTIDEVICE_TEST
		txbuf[0]=rxBuf[0];
	#else
		txbuf[0]=READ_ID;
	#endif
	txbuf[1]=0x03;
	AddTx(txbuf, len-2);
  //RequestAdd(rxBuf, len);
	//USART_TX_QUEUE_SELF_ADD;			
}

/**
  * @brief  modbus写单线圈
  *         05
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  */
void WriteCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//获取位状态
	
	CDV_INT08U len,txbuf[50];
	CDV_INT16U addr, num;
//	CSB_INT16U sta, sf;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	if(addr>=0x1010 && addr<0x101f) {
		addr = (COIL_RES_MOTOR_EN) | ((addr) &0x00f);
	} else if(addr>=0x1020 && addr<0x102f) {
		addr = (COIL_RES_MOTOR_EN) | (((addr) &0x00f)+8);
	} else if(addr>=0x1030 && addr<0x103f) {
		addr = (COIL_RES_ZERO_DIR) | ((addr) &0x00f);
	} else if(addr>=0x1000 && addr<0x100f) {
		addr = (COIL_RES_ZERO_DIR) | (((addr) &0x00f)+8);
	}
	
	num = (rxBuf[4]<<8) + rxBuf[5];
//	sta = (addr >> 3) /*+ 3*/;//读线圈的第几字节 0->*
//	sf = addr & 0x07;//读线圈的第几位 9->0
	if(addr > 0xAAAA ) {
		ModbusRequest(rxBuf[1] , 2);
		return;
	}else if(num != 0x0000&&num != 0xFF00 ) {
		ModbusRequest(rxBuf[1] , 3);
		return;
	}
//	else if (0<=addr<15){
//		if (num == 0x0000||num == 0x0001)
//			switch(num){
//				case 0:
//	    GPIO_ResetBits(g_cdvO[addr].port,g_cdvO[addr].pin); 
//				break;
//				case 1:
//			GPIO_SetBits(g_cdvO[addr].port,g_cdvO[addr].pin);
//				break;				
//	  }	
//	}
	len = rxLen;//tx总长度=rx 原样应答
	//USART_TX_ADD_WITH_LEN(len);//开辟空间
	//memcpy(USART_TX_BUF_ADDR , rxBuf , len);
	
	if (0xFF00 == num){
		SET_COIL_ADDR(addr);
		
	} 
	else if (0x0000 == num)
	{
		RESET_COIL_ADDR(addr)
	} else{
		
	 }
	
	 if(READ_O(addr))
		{
			OWrite(addr, BIT_1);
			
//			if(addr == 200)
//				ChangeToCdvStat(CDV_RUN);
//			else if(addr == 201)
//				ChangeToCdvStat(CDV_PAUSE);
		}
		else
		{
			OWrite(addr, BIT_0);
			
//			if(addr == 200)
//				ChangeToCdvStat(CDV_ONLINE);
//			else if(addr == 201)
//				ChangeToCdvStat(CDV_RUN);
		}
//	}
//	WriteCoilCmdParse(addr);
//	Modbus_TX_len=len;
//	for(i=0;i<Modbus_TX_len;i++)
//	USART_TX_BUF[i]=rxBuf[i];
//	//USART_TX_QUEUE_SELF_ADD;	
				
	#if MULTIDEVICE_TEST
		txbuf[0]=rxBuf[0];
	#else
		txbuf[0]=READ_ID;
	#endif
	txbuf[1]=0x05;
	USART_TX_BUF[2]= rxBuf[2];
	USART_TX_BUF[3]= rxBuf[3];
	USART_TX_BUF[4]= rxBuf[4];
	USART_TX_BUF[5]= rxBuf[5];
//	AddTx(txbuf , len-2);
	AddTx(txbuf, len-2);
	memcpy(&txbuf[2] , &USART_TX_BUF[2] , len);
	USART2_Send(txbuf,len);
}

/**
  * @brief  modbus写多线圈
  *         15
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  */
void WriteMultiCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen){
	
	CDV_INT08U i, len ,txbuf[50] ;
	CDV_INT16U addr, num, numCh,addr1,num1;
	CDV_INT16U sta, sf, end, esf;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	
//	if(addr>=0x1010 && addr<0x101f) {
//		addr = (COIL_RES_MOTOR_EN) | ((addr) &0x00f);
//	} else if(addr>=0x1020 && addr<0x102f) {
//		addr = (COIL_RES_MOTOR_EN) | (((addr) &0x00f)+8);
//	} else if(addr>=0x1030 && addr<0x103f) {
//		addr = (COIL_RES_ZERO_DIR) | ((addr) &0x00f);
//	} else if(addr>=0x1000 && addr<0x100f) {
//		addr = (COIL_RES_ZERO_DIR) | (((addr) &0x00f)+8);
//	}
	
	num = (rxBuf[4]<<8) + rxBuf[5];//写的数量，要处理多少字节关键要看这个，而不能看numch
	sta = (addr >> 3) /*+ 3*/;//初始char
	end = ((addr+num-1) >> 3) /*+ 3*/;//最终char
	sf = addr & 0x07;
	esf = ((addr+num-1) & 0x07) +1;
	
	numCh = end - sta;//间隔数
	
	if(0x0001 >num || num> 0x002F ||((num>>3)+(num&0x07?1:0)) != rxBuf[6]){
		ModbusRequest(rxBuf[1] , 3);
		return;
	} else if(addr > 0xFF || addr + num > 0xFF) {
		ModbusRequest(rxBuf[1] , 2);
		return;
	}
//	if(numCh > rxBuf[6])
//		return;//读char数量
	len = rxLen;   //tx总长度=6
	//USART_TX_ADD_WITH_LEN(len);//开辟空间
	//memcpy(USART_TX_BUF_ADDR , rxBuf , len);
//	if (0<=addr&&addr<=14)
//		{
//		for(i=0;i<15;i++)
//			{
////			addr1 = (addr>>i)&0x0001;
//			num1 = (num>>i)&0x0001;
//		if (num1 == 0||num1 == 1)
//			 {
//			switch(num1)
//				 {
//				case 0:
//	    GPIO_ResetBits(g_cdvO[i].port,g_cdvO[i].pin); 
//				break;
//				case 1:
//			GPIO_SetBits(g_cdvO[i].port,g_cdvO[i].pin);
//				break;	
//			    }	
//		   }
//	  }	
	
	
	//第一个字节
	if(numCh != 0)
	  MODBUS_COIL_CH(sta) = (rxBuf[7]<<sf) | (MODBUS_COIL_CH(sta) & (0xFF>>(8-sf)));
	else
		MODBUS_COIL_CH(sta) = ((rxBuf[7]<<sf)& (0xFF>>(8-esf))) | (MODBUS_COIL_CH(sta) & ((0xFF>>(8-sf)) | (0xFF<<esf)));
	//中间字节
	for(i=1;i<numCh;i++){
				MODBUS_COIL_CH(sta+i) = (rxBuf[7+i]<<sf) | (rxBuf[7+i-1]>>(8-sf));
			}
	//最后字节
		//	if(i >= rxBuf[6])
	if(numCh != 0 )
		MODBUS_COIL_CH(sta+i) = (MODBUS_COIL_CH(sta+i) & (0xFF<<(esf))) 
	| (((i>=rxBuf[6]?0x00 : (rxBuf[7+i]<<sf)) | (rxBuf[7+i-1]>>(8-sf)))& (0xFF>>(8-esf))); 
	
for(i = addr; i< addr + num ; i++)
	if(READ_O(i))
	{
		OWrite(i, BIT_1);
	}
	else
	{
		OWrite(i, BIT_0);
	}
  
	#if MULTIDEVICE_TEST
		txbuf[0]=rxBuf[0];
	#else
		txbuf[0]=READ_ID;
	#endif
	txbuf[1]=0x0F;
	USART_TX_BUF[2]=rxBuf[2];
	USART_TX_BUF[3]=rxBuf[3];
	USART_TX_BUF[4]=rxBuf[4];
	USART_TX_BUF[5]=rxBuf[5];
	
	memcpy(&txbuf[2] , &USART_TX_BUF[2] , len-2);
	AddTx(txbuf, len-5);
	USART2_Send(USART_TX_BUF,len-3);
 	
  }
	
/**
  * @brief  modbus写多寄存器
  *         16
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  */
void WriteMultiRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//获取位状态
	
	CDV_INT08U len ,txbuf[2], i;
	CDV_INT16U addr, num , data;
	addr = (rxBuf[2]<<8) + rxBuf[3];

//	if(addr>=0x1000 && addr<0x1FFF) {
//		addr = ((addr>>4)&0x00f0 ) | ((addr) &0x000f);
//	}

	num = (rxBuf[4]<<16) + rxBuf[5];
	//numCh = rxBuf[6];
//	if(0x0001 >num || num> 0x007B ||numCh != num*2){
//		ModbusRequest(rxBuf[1] , 3);
//		return;
//	} else if(addr > 0x0AAA || addr + num > 0x0AAA) {
//		ModbusRequest(rxBuf[1] , 2);
//		return;
//	}
	
//	else if(num < 0x0000 || num > 0xFFFF) {
//		ModbusRequest(rxBuf[1] , 3);
//		return;
//	}
	for( i = 0; i< num ; i++) {
		data = (rxBuf[7+2*i]<<8) + rxBuf[8+2*i];
		MODBUS_REG_ADDR(addr+i) = data;
	}
//	Modbus_EleParm_Set2(addr);
//	WriteMultiRegisterCmdParse(addr);
	len = 6;//tx总长度=rx 原样应答

	#if MULTIDEVICE_TEST
		txbuf[0]=rxBuf[0];
	#else
		txbuf[0]=READ_ID;
	#endif
	txbuf[1]=0x10;
	USART_TX_BUF[2]= rxBuf[2];
	USART_TX_BUF[3]= rxBuf[3];
	USART_TX_BUF[4]= rxBuf[4];
	USART_TX_BUF[5]= rxBuf[5];
	AddTx(txbuf , len);
	
}

/**
  * @brief  modbus写单寄存器
  *         06
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  */
void WriteRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//获取位状态
	
	CDV_INT08U len ,i;
	CDV_INT16U addr, num;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	if(addr>=0x1000 && addr<0x1FFF) {
		addr = ((addr>>4)&0x00f0 ) | ((addr) &0x000f);
	}
	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(addr > 0x0AAA ) {
		ModbusRequest(rxBuf[1] , 2);
		return;
	}
//	else if(num < 0x0000 || num > 0xFFFF) {
//		ModbusRequest(rxBuf[1] , 3);
//		return;
//	}
	WriteMultiRegisterCmdParse(addr);
	len = rxLen;//tx总长度=rx 原样应答
	Modbus_TX_len=len;
	//USART_TX_ADD_WITH_LEN(len);//开辟空间
	//memcpy(USART_TX_BUF_ADDR , rxBuf , len);
	MODBUS_REG_ADDR(addr) = num;
	for(i=0;i<Modbus_TX_len;i++)
	USART_TX_BUF[i]=rxBuf[i];
	//RequestAdd(rxBuf, len);
	//AddTx(txBuf, len-2);
	//USART_TX_QUEUE_SELF_ADD;			
}

/**
  * @brief  modbus读输入寄存器
  *         04
  *  
  * @param  rxBuf     待分析字符串指针
  *         rxLen     待分析字符串的长度
  *           
  *   
  * @retval 返回值说明
  *
  * @note   使用到全局变量,不能重入
  */
void ReadInRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//获取位状态
	
	CDV_INT08U i, len ,txbuf[2];
	CDV_INT16U addr, num;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	if(addr>=0x1000 && addr<0x1FFF) {
		addr = ((addr>>8)&0x000f )<<1;// | ((addr<<4) &0x00f0);
	}

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(0x0001 >num || num> 0x007D){
		ModbusRequest(rxBuf[1] , 3);
		return;
	} else if(addr > 0x0AAA || addr + num > 0x0AAA) {
		ModbusRequest(rxBuf[1] , 2);
		return;
	}
	
	len = 5 + (num<<1);//tx总长度
	//USART_TX_ADD_WITH_LEN(len);//开辟空间
	USART_TX_BUF[2] = num<<1;
	//所处字节
//	ReadInRegisterCmdParse(addr,num);
	for(i=0;i<num;i++){
//		USART_TX_BUF[3 +(i<<1)] = HIGH16U(MODBUS_INREG_ADDR(addr + i));
//		USART_TX_BUF[3 +(i<<1)+ 1] = LOW16U(MODBUS_INREG_ADDR(addr + i));
		USART_TX_BUF[3 +(i<<1)] = HIGH16U(MODBUS_REG_ADDR(addr + i));
		USART_TX_BUF[3 +(i<<1)+ 1] = LOW16U(MODBUS_REG_ADDR(addr + i));		
		
	}
	#if MULTIDEVICE_TEST
		txbuf[0]=rxBuf[0];
	#else
		txbuf[0]=Modbus_addr;
	#endif
	txbuf[1]=0x04;
	AddTx(txbuf, len-2);
  //RequestAdd(rxBuf, len);
	//USART_TX_QUEUE_SELF_ADD;			
}

/******************************************************/
// 函数名 	: void WriteCoilCmdParse(CSB_INT16U addr)
// 功能描述	: 读线圈命令解析
// 参数 addr:线圈地址
// 修改日期	:
// 添加日期	:	2016-11-5	
// 返回值   : 	无
/******************************************************/
void WriteCoilCmdParse(CDV_INT16U addr)
{
//	#define COIL_CTRL       	 0x00/*控制命令线圈起始地址*/ 
//#define COIL_ULTRA    		 0x00/*超声启停线圈地址*/
//#define COIL_HEAT      		 0x01/*加热控制线圈地址*/
//#define COIL_MOTOR      		 0x02/*抛动电机控制线圈地址*/
//#define COIL_MCP     	    0x03/*循环泵控制线圈地址*/
//#define COIL_SOLENOIDVALVE1 0x04/*电磁阀1控制线圈地址*/
//#define COIL_SOLENOIDVALVE2 0x05/*电磁阀2控制线圈地址*/	
	}
	
	/**
* @brief  modbus功能码出错回复
*
*
* @param  no     功能码
*         err     错误码
*
*
* @retval 返回值说明
*
* @note
*/
void ModbusRequest(CDV_INT08U no, CDV_INT08U errNo) {
	CDV_INT08U len, txBuf[8];
	//	OS_ERR err;
	switch (no) {
	case 65:
		len = 6;//tx总长度=8
		//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
//NEW08U(txBuf, len);
		txBuf[0] = Modbus_addr;
		txBuf[1] = 0x41 + 0x80;
		txBuf[2] = 00;
		txBuf[3] = 00;
		txBuf[4] = no;
		txBuf[5] = errNo;
		break;
	default:
		len = 3;//tx总长度=5
		//txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*len);
		//NEW08U(txBuf, len);
		txBuf[0] = Modbus_addr;
		txBuf[1] = no + 0x80;
		txBuf[2] = errNo;
		break;
	}
//	AddTx(txBuf, len);
//	DELETE(txBuf);
	Modbus_TX_len=len+2;
}

void WriteMultiRegisterCmdParse(CDV_INT16U addr)
{
	
}

void Modbus_Cmd_Receive(void)
{
	CDV_INT16U modbus_crc_h,modbus_crc_l,modbus_crc_b,modbus_crc;
	if(Modbus_timeout_cnt>1)	//一帧数据间隔时间 0.2ms
	{
		Modbus_recevie_flag=0;
		Modbus_timeout_cnt = 0;	

	#if MULTIDEVICE_TEST
		
		if(USART2_RX_BUF[0]>=1&&USART3_RX_BUF[0]<=9&&(USART3_RX_STA>5))//将针对本机的命令存入缓存
	#else
		if(USART2_RX_BUF[0]==READ_ID&&USART2_RX_CNT>5) //将针对本机的命令存入缓存
	#endif
		{			
			modbus_crc_h=USART2_RX_BUF[USART2_RX_CNT-2];
			modbus_crc_l=USART2_RX_BUF[USART2_RX_CNT-1];
			modbus_crc=(u16)((modbus_crc_l)<<8)|modbus_crc_h;//得到接收的16位crc值
			modbus_crc_b=getCRC16(USART2_RX_BUF,USART2_RX_CNT-2);//计算一帧的crc值
			if(modbus_crc_b==modbus_crc)//计算的crc与接收的比较
			{					
				memcpy(Modbus_RX_buf,USART2_RX_BUF,USART2_RX_CNT);
				memset(USART2_RX_BUF,0,USART2_RX_CNT*sizeof(CDV_INT08U));	
				Modbus_RX_len=USART2_RX_CNT;
				USART2_RX_CNT=0;
				Modbus_cmd_flag=1;					
			
			}
			else
			{				
				memset(USART2_RX_BUF,0,USART2_RX_CNT*sizeof(CDV_INT08U));
				USART2_RX_CNT=0;
			}
		}
		else
		{			
			memset(USART2_RX_BUF,0,USART2_RX_CNT*sizeof(CDV_INT08U));
			USART2_RX_CNT=0;
		}
	}	
}

/*****************************通讯处理***************************************/
void modbus(void)
{ 
//	u8 i=0;//,lcd_buf[30];
//	RTC_TimeTypeDef RTC_TimeStruct;	
	if(1)
	{
//		Debug_DataMeasure();
#include <stm32f10x.h>
		Modbus_Cmd_Receive();
		if(Modbus_mode==0)//从机模式
		{
				
			Modbus_cmd();	
		}
	}
}

/********************处于从机状态时对数据帧的解析**************************/
void Modbus_cmd(void)
{
	if(Modbus_cmd_flag)
	{
		Modbus_cmd_flag = 0;
		ModbusCmdExecution_State = 1;
		switch(Modbus_RX_buf[1])//命令码筛选
		{ 
			case 100://脚本联机
				break;
			case 1://读线圈
				ReadCoil(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 2://读输入线圈
				ReadInCoil(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 3://读寄存器
//				ReadRegister(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 4://读输入寄存器
//				ReadInRegister(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 5://写单线圈
//				WriteCoil(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 6://预置单寄存器
//				WriteRegister(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 7://读取异常状态
				break;
			case 8://回送诊断校验
				break;
			case 9://编程
				break;
			case 10://控询
				break;
			case 11://读取事件计数
				break;
			case 12://读取通信事件记录
				break;
			case 15://写多线圈
				WriteMultiCoil(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 16://预置多寄存器
//				WriteMultiRegister(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 17://报告从机标志
				break;
			case 22://扩展功能------CSB功能
				break;
			case 65://扩展功能------CSB功能
				//OperateScript(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 0xfe://自定义读
				break;
			case 0xff://急停
				break;
			default: //命令码无效不应答
				//ModbusRequest(Modbus_RX_buf[1], 1);
				break;
			}
	
		memset(Modbus_RX_buf,0,Modbus_max_recevie_buf*sizeof(CDV_INT08U));	
	}
}
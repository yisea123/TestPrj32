
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
CDV_INT08U Modbus_addr=0x01;//������ַ

 CDV_INT08U Debug_mode;
 CDV_INT08U Modbus_slave_addr;    //Ŀ��ӻ���ַ
 CDV_INT08U Function_Code;		//����ģʽ������
 CDV_INT16U Modbus_slave_reg_addr;	//�ӻ��Ĵ�����ַ
 CDV_INT16U Modbus_slave_reg_num;	//�ӻ��Ĵ�����
 CDV_INT16U Modbus_reg_val1;//д��ӻ��Ĵ�����ֵ����Modbus_slave_reg_num=1����д�����16λ���ݣ�Modbus_reg_val2��Ч
 CDV_INT16U Modbus_reg_val2;//��Modbus_slave_reg_num=2����д�����32λ���ݣ�Modbus_reg_val1Ϊ��16��Modbus_reg_val2λ��16
 CDV_REG08 Modbus_recevie_flag=0;
 CDV_REG32 Modbus_timeout_cnt=0;
 CDV_REG16 Modbus_SendWait_Cnt = 0;//�ȴ����ͼ�ʱ
 CDV_INT16U Modbus_SendWait_Time = 0;//�ȴ�����ʱ��
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

//#define Modbus_addr  0x01  //������ַ
 CDV_INT08U Modbus_addr;//������ַ
 CDV_INT08U Modbus_mode;//0:�ӻ�ģʽ 1:����ģʽ
 CDV_INT16U Ctrl_commond_addr;
 CDV_INT16U Ctrl_num;
 CDV_INT08U slave_number[10],HeartBeat_slave_request_flag,slave_number_dis[10];
 CDV_REG08 HeartBeat_flag,Interface_refresh_flag;//���������ͱ�־
 CDV_REG16 HeartBeat_timeout;//���������ͼ������
 CDV_REG08	slave_display;//�ӻ�����ʾ��־
 CDV_REG16	slave_count;//�ӻ�����ʾ��ʱ
 CDV_REG08	slave_display2;	
 MODBUS_Coil g_modbusCoil;
 MODBUS_Register g_modbusReg;
 
 
 void ModbusRequest(CDV_INT08U no, CDV_INT08U errNo);
/**
  * @brief  ���ַ�������modbus����
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         len       �������ַ����ĳ���
  *         txBuff    Ӧ���ַ���  
  *   
  * @retval ����ֵ˵��
  *
  * @note   RecvParse��
  */
CDV_INT08U ModbusParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen)
{	
	if(rxBuf[0]==Modbus_addr)
	{
		switch(rxBuf[1]){ //������ɸѡ
			case 1://����Ȧ
				ReadCoil(rxBuf,rxLen);
				break;
			case 2://��������Ȧ
				ReadInCoil(rxBuf,rxLen);
				break;
			case 3://���Ĵ���
				ReadRegister(rxBuf,rxLen);
				break;
			case 4://������Ĵ���
				//ReadInRegister(rxBuf,rxLen);
				break;
			case 5://д����Ȧ
				WriteCoil(rxBuf,rxLen);
				break;
			case 6://Ԥ�õ��Ĵ���
				WriteRegister(rxBuf,rxLen);
				break;
			case 7://��ȡ�쳣״̬
				break;
			case 8://�������У��
				break;
			case 9://���
				break;
			case 10://��ѯ
				break;
			case 11://��ȡ�¼�����
				break;
			case 12://��ȡͨ���¼���¼
				break;
			case 15://д����Ȧ
				WriteMultiCoil(rxBuf,rxLen);
				break;
			case 16://Ԥ�ö�Ĵ���
				WriteMultiRegister(rxBuf,rxLen);
				break;
			case 17://����ӻ���־
				break;
			case 22://��չ����------CSB����
				break;
			default: //��������Ч��Ӧ��
//				ModbusRequest(rxBuf[1], 1);
				break;
		}
		return 1;
	}
	return 0;
}
///**
//  * @brief  ����Ҫ�ظ��Ĵ��������׼������
//  *         
//  *  
//  * @param  txBuf     �����ַ����׵�ַָ��
//  *         txLen     �����ַ�������,������crc
//  *           
//  *   
//  * @retval ����ֵ˵��
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
		
	//	USART_TX_ADD_WITH_LEN(len);//���ٿռ�	
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
  * @brief  modbus����Ȧ
  *         01
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
  */
void ReadCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//��ȡλ״̬
	
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
	
	sta = (addr >> 3) /*+ 3*/;//����Ȧ�ĳ�ʼchar
	sf = addr & 0x07;
	numCh = (num>>3)+((num & 0x07)?1:0);//��char����
	
	
	len = 5 + numCh;//tx�ܳ���
	//USART_TX_ADD_WITH_LEN(len);//���ٿռ�
	USART_TX_BUF[2] = numCh;
	//�����ֽ�
	for(i=0;i<USART_TX_BUF[2]+1;i++){//�����һλ����ֹ���һ���ֽ��ٶ�λ
				USART_TX_BUF[3+i] = MODBUS_COIL_CH(sta+i);
			}
	//ƫ��
	for(i=0;i<USART_TX_BUF[2];i++){
				USART_TX_BUF[3+i] = USART_TX_BUF[3+i]>>sf | USART_TX_BUF[4+i]<<(8-sf);
			}
	if(/*sf != 0 && */i!= 0)
		USART_TX_BUF[3+i-1] &= (0xFF>>(8 - (0 ==(num & 0x07)?8:(num & 0x07)))); // ��0
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
  * @brief  modbus��������Ȧ
  *         02
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
  */
void ReadInCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//��ȡλ״̬
	
	CDV_INT08U i, len ,txbuf[20];
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	num = (rxBuf[4]<<8) + rxBuf[5];//��i����
	
	if(0x0001 >num || num> 0x002F){
		ModbusRequest(rxBuf[1] , 3);
		return;
	} else if(addr > 0xFF || addr + num > 0xFF) {
		ModbusRequest(rxBuf[1] , 2);
		return;
	}
	
	sta = (addr >> 3) /*+ 3*/;//����Ȧ�ĳ�ʼchar
	sf = addr & 0x07;
	numCh = (num>>3)+((num & 0x07)?1:0);//��char����
	len = 5 + numCh;//tx�ܳ���
	//USART_TX_ADD_WITH_LEN(len);//���ٿռ�
	USART_TX_BUF[2] = numCh;
	//�����ֽ�
	for(i=0;i<USART_TX_BUF[2]+1;i++){//�����һλ����ֹ���һ���ֽ��ٶ�λ
				USART_TX_BUF[3+i] = MODBUS_INCOIL_CH(sta+i);
			}
	//ƫ��
	for(i=0;i<USART_TX_BUF[2];i++){
				USART_TX_BUF[3+i] = USART_TX_BUF[3+i]>>sf | USART_TX_BUF[4+i]<<(8-sf);
			}
	if(/*sf != 0 &&*/ i!= 0)
		USART_TX_BUF[3+i-1] &= (0xFF>>(8 - (0 ==(num & 0x07)?8:(num & 0x07))));// ��0
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
  * @brief  modbus���Ĵ���
  *         03
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
  */
void ReadRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//��ȡλ״̬
	
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
	
	len = 5 + (num<<1);//tx�ܳ���
	//USART_TX_ADD_WITH_LEN(len);//���ٿռ�
	USART_TX_BUF[2] = num<<1;
	//�����ֽ�
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
  * @brief  modbusд����Ȧ
  *         05
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
  */
void WriteCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//��ȡλ״̬
	
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
//	sta = (addr >> 3) /*+ 3*/;//����Ȧ�ĵڼ��ֽ� 0->*
//	sf = addr & 0x07;//����Ȧ�ĵڼ�λ 9->0
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
	len = rxLen;//tx�ܳ���=rx ԭ��Ӧ��
	//USART_TX_ADD_WITH_LEN(len);//���ٿռ�
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
  * @brief  modbusд����Ȧ
  *         15
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
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
	
	num = (rxBuf[4]<<8) + rxBuf[5];//д��������Ҫ��������ֽڹؼ�Ҫ������������ܿ�numch
	sta = (addr >> 3) /*+ 3*/;//��ʼchar
	end = ((addr+num-1) >> 3) /*+ 3*/;//����char
	sf = addr & 0x07;
	esf = ((addr+num-1) & 0x07) +1;
	
	numCh = end - sta;//�����
	
	if(0x0001 >num || num> 0x002F ||((num>>3)+(num&0x07?1:0)) != rxBuf[6]){
		ModbusRequest(rxBuf[1] , 3);
		return;
	} else if(addr > 0xFF || addr + num > 0xFF) {
		ModbusRequest(rxBuf[1] , 2);
		return;
	}
//	if(numCh > rxBuf[6])
//		return;//��char����
	len = rxLen;   //tx�ܳ���=6
	//USART_TX_ADD_WITH_LEN(len);//���ٿռ�
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
	
	
	//��һ���ֽ�
	if(numCh != 0)
	  MODBUS_COIL_CH(sta) = (rxBuf[7]<<sf) | (MODBUS_COIL_CH(sta) & (0xFF>>(8-sf)));
	else
		MODBUS_COIL_CH(sta) = ((rxBuf[7]<<sf)& (0xFF>>(8-esf))) | (MODBUS_COIL_CH(sta) & ((0xFF>>(8-sf)) | (0xFF<<esf)));
	//�м��ֽ�
	for(i=1;i<numCh;i++){
				MODBUS_COIL_CH(sta+i) = (rxBuf[7+i]<<sf) | (rxBuf[7+i-1]>>(8-sf));
			}
	//����ֽ�
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
  * @brief  modbusд��Ĵ���
  *         16
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
  */
void WriteMultiRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//��ȡλ״̬
	
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
	len = 6;//tx�ܳ���=rx ԭ��Ӧ��

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
  * @brief  modbusд���Ĵ���
  *         06
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
  */
void WriteRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//��ȡλ״̬
	
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
	len = rxLen;//tx�ܳ���=rx ԭ��Ӧ��
	Modbus_TX_len=len;
	//USART_TX_ADD_WITH_LEN(len);//���ٿռ�
	//memcpy(USART_TX_BUF_ADDR , rxBuf , len);
	MODBUS_REG_ADDR(addr) = num;
	for(i=0;i<Modbus_TX_len;i++)
	USART_TX_BUF[i]=rxBuf[i];
	//RequestAdd(rxBuf, len);
	//AddTx(txBuf, len-2);
	//USART_TX_QUEUE_SELF_ADD;			
}

/**
  * @brief  modbus������Ĵ���
  *         04
  *  
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  *           
  *   
  * @retval ����ֵ˵��
  *
  * @note   ʹ�õ�ȫ�ֱ���,��������
  */
void ReadInRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen){//��ȡλ״̬
	
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
	
	len = 5 + (num<<1);//tx�ܳ���
	//USART_TX_ADD_WITH_LEN(len);//���ٿռ�
	USART_TX_BUF[2] = num<<1;
	//�����ֽ�
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
// ������ 	: void WriteCoilCmdParse(CSB_INT16U addr)
// ��������	: ����Ȧ�������
// ���� addr:��Ȧ��ַ
// �޸�����	:
// �������	:	2016-11-5	
// ����ֵ   : 	��
/******************************************************/
void WriteCoilCmdParse(CDV_INT16U addr)
{
//	#define COIL_CTRL       	 0x00/*����������Ȧ��ʼ��ַ*/ 
//#define COIL_ULTRA    		 0x00/*������ͣ��Ȧ��ַ*/
//#define COIL_HEAT      		 0x01/*���ȿ�����Ȧ��ַ*/
//#define COIL_MOTOR      		 0x02/*�׶����������Ȧ��ַ*/
//#define COIL_MCP     	    0x03/*ѭ���ÿ�����Ȧ��ַ*/
//#define COIL_SOLENOIDVALVE1 0x04/*��ŷ�1������Ȧ��ַ*/
//#define COIL_SOLENOIDVALVE2 0x05/*��ŷ�2������Ȧ��ַ*/	
	}
	
	/**
* @brief  modbus���������ظ�
*
*
* @param  no     ������
*         err     ������
*
*
* @retval ����ֵ˵��
*
* @note
*/
void ModbusRequest(CDV_INT08U no, CDV_INT08U errNo) {
	CDV_INT08U len, txBuf[8];
	//	OS_ERR err;
	switch (no) {
	case 65:
		len = 6;//tx�ܳ���=8
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
		len = 3;//tx�ܳ���=5
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
	if(Modbus_timeout_cnt>1)	//һ֡���ݼ��ʱ�� 0.2ms
	{
		Modbus_recevie_flag=0;
		Modbus_timeout_cnt = 0;	

	#if MULTIDEVICE_TEST
		
		if(USART2_RX_BUF[0]>=1&&USART3_RX_BUF[0]<=9&&(USART3_RX_STA>5))//����Ա�����������뻺��
	#else
		if(USART2_RX_BUF[0]==READ_ID&&USART2_RX_CNT>5) //����Ա�����������뻺��
	#endif
		{			
			modbus_crc_h=USART2_RX_BUF[USART2_RX_CNT-2];
			modbus_crc_l=USART2_RX_BUF[USART2_RX_CNT-1];
			modbus_crc=(u16)((modbus_crc_l)<<8)|modbus_crc_h;//�õ����յ�16λcrcֵ
			modbus_crc_b=getCRC16(USART2_RX_BUF,USART2_RX_CNT-2);//����һ֡��crcֵ
			if(modbus_crc_b==modbus_crc)//�����crc����յıȽ�
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

/*****************************ͨѶ����***************************************/
void modbus(void)
{ 
//	u8 i=0;//,lcd_buf[30];
//	RTC_TimeTypeDef RTC_TimeStruct;	
	if(1)
	{
//		Debug_DataMeasure();
#include <stm32f10x.h>
		Modbus_Cmd_Receive();
		if(Modbus_mode==0)//�ӻ�ģʽ
		{
				
			Modbus_cmd();	
		}
	}
}

/********************���ڴӻ�״̬ʱ������֡�Ľ���**************************/
void Modbus_cmd(void)
{
	if(Modbus_cmd_flag)
	{
		Modbus_cmd_flag = 0;
		ModbusCmdExecution_State = 1;
		switch(Modbus_RX_buf[1])//������ɸѡ
		{ 
			case 100://�ű�����
				break;
			case 1://����Ȧ
				ReadCoil(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 2://��������Ȧ
				ReadInCoil(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 3://���Ĵ���
//				ReadRegister(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 4://������Ĵ���
//				ReadInRegister(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 5://д����Ȧ
//				WriteCoil(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 6://Ԥ�õ��Ĵ���
//				WriteRegister(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 7://��ȡ�쳣״̬
				break;
			case 8://�������У��
				break;
			case 9://���
				break;
			case 10://��ѯ
				break;
			case 11://��ȡ�¼�����
				break;
			case 12://��ȡͨ���¼���¼
				break;
			case 15://д����Ȧ
				WriteMultiCoil(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 16://Ԥ�ö�Ĵ���
//				WriteMultiRegister(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 17://����ӻ���־
				break;
			case 22://��չ����------CSB����
				break;
			case 65://��չ����------CSB����
				//OperateScript(Modbus_RX_buf,Modbus_RX_len);
				break;
			case 0xfe://�Զ����
				break;
			case 0xff://��ͣ
				break;
			default: //��������Ч��Ӧ��
				//ModbusRequest(Modbus_RX_buf[1], 1);
				break;
			}
	
		memset(Modbus_RX_buf,0,Modbus_max_recevie_buf*sizeof(CDV_INT08U));	
	}
}
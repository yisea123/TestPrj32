
/**
  ******************************************************************************
  * @file    /modbus.c 
  * @author  MMY
  * @version V2.0.0
  * @date    4-July-2015
  * @brief   modbus function and var and 
  * 
@verbatim  
             20160520��ֻ�ܽ�����ͨio�Ķ�д�ͱ����Ķ�д
             20170411������������������������������
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

MODBUS_Coil g_modbusCoil={0};
MODBUS_Input_Coil g_modbusInCoil={0};
MODBUS_Register g_modbusReg={0};
MODBUS_Input_Register g_modbusInReg={0};

//int ReadCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo);
//int ReadInCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo);
//int ReadRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo);
//int ReadInRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo);
//int WriteCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo);
//int WriteRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo);
//int WriteMultiCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo);
//int WriteMultiRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo);


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
int ReadCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo){//��ȡλ״̬
	
	CDV_INT08U i, len ;
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf;
	CDV_INT08U *txBuf = NULL;
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(0x0001 >num || num> 0x07D0){
		ModbusRequest(rxBuf[1] , 3, uartNo);
		return 3;
	} else if(addr > (COIL_CHN<<3) || addr + num > (COIL_CHN<<3)) {
		ModbusRequest(rxBuf[1] , 2, uartNo);
		return 2;
	}
	
	sta = (addr >> 3) /*+ 3*/;//����Ȧ�ĳ�ʼchar
	sf = addr & 0x07;
	numCh = (num>>3)+((num & 0x07)?1:0);//��char����
	
	
	len = 5 + numCh-2;//tx�ܳ���
	//USART_TX_ADD_WITH_LEN(len);//���ٿռ�
	NEW08U(txBuf , len);
	txBuf[0] = rxBuf[0];
	txBuf[1] = rxBuf[1];
	txBuf[2] = (num>>3);//numCh;
	//�����ֽ�
	for(i=0;i<txBuf[2]+1;i++){//�����һλ����ֹ���һ���ֽ��ٶ�λ
		txBuf[3+i] = MODBUS_COIL_CH(sta+i);
	}
	//ƫ��
	for(i=0;i<txBuf[2];i++){
		txBuf[3+i] = txBuf[3+i]>>sf | txBuf[4+i]<<(8-sf);
	}
	if(/*sf != 0 && */i!= 0)
		txBuf[3+i-1] &= (0xFF>>(8 - (0 ==(num & 0x07)?8:(num & 0x07))));// ��0
	
  AddTx(txBuf , len, uartNo);
	DELETE(txBuf);
	return 0;
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
int ReadInCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo){//��ȡλ״̬
	
	CDV_INT08U i, len ;
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf;
	CDV_INT08U *txBuf = NULL;
	
	addr = (rxBuf[2]<<8) + rxBuf[3];
	num = (rxBuf[4]<<8) + rxBuf[5];//��i����
	
	if(0x0001 >num || num> 0x07D0){
		ModbusRequest(rxBuf[1] , 3, uartNo);
		return 3;
	} else if(addr > (INCOIL_CHN<<3) || addr + num > (INCOIL_CHN<<3)) {
		ModbusRequest(rxBuf[1] , 2, uartNo);
		return 2;
	}
	
	for(i = 0 ; i < CDV_I_NUM; i ++) {
	  IRead(i);
	}
	
	sta = (addr >> 3) /*+ 3*/;//����Ȧ�ĳ�ʼchar
	sf = addr & 0x07;
	numCh = (num>>3)+((num & 0x07)?1:0);//��char����
	len = 5 + numCh-2;//tx�ܳ���
	
	NEW08U(txBuf , len);
	txBuf[0] = rxBuf[0];
	txBuf[1] = rxBuf[1];
	txBuf[2] = (num>>3);//numCh;
	//�����ֽ�
	for(i=0;i<txBuf[2]+1;i++){//�����һλ����ֹ���һ���ֽ��ٶ�λ
				txBuf[3+i] = MODBUS_INCOIL_CH(sta+i);
			}
	//ƫ��
	for(i=0;i<txBuf[2];i++){
				txBuf[3+i] = txBuf[3+i]>>sf | txBuf[4+i]<<(8-sf);
			}
	if(/*sf != 0 &&*/ i!= 0)
		txBuf[3+i-1] &= (0xFF>>(8 - (0 ==(num & 0x07)?8:(num & 0x07))));// ��0
	
  AddTx(txBuf , len, uartNo);	
	DELETE(txBuf);
	return 0;
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
int ReadRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo){//��ȡλ״̬
	
	CDV_INT08U i, len ;
	CDV_INT16U addr, num;
	CDV_INT08U *txBuf = NULL;
	
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(0x0001 >num || num> 0x007D){
		ModbusRequest(rxBuf[1] , 3, uartNo);
		return 3;
	} else if(addr > REG_N || addr + num > REG_N) {
		ModbusRequest(rxBuf[1] , 2, uartNo);
		return 2;
	}
	
	len = 5 + (num<<1)-2;//tx�ܳ���
	
	NEW08U(txBuf , len);
	txBuf[0] = rxBuf[0];
	txBuf[1] = rxBuf[1];
	txBuf[2] = num<<1;
	//�����ֽ�
	for(i=0;i<num;i++){
		txBuf[3 +(i<<1)] = HIGH16U(MODBUS_REG_ADDR(addr + i));
		txBuf[3 +(i<<1)+ 1] = LOW16U(MODBUS_REG_ADDR(addr + i));
	}
	
  AddTx(txBuf , len, uartNo);
  DELETE(txBuf);
	return 0;	
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
int ReadInRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo){//��ȡλ״̬
	
	CDV_INT08U i, len ;
	CDV_INT16U addr, num;
	CDV_INT08U *txBuf = NULL;
	
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(0x0001 >num || num> 0x007D){
		ModbusRequest(rxBuf[1] , 3, uartNo);
		return 3;
	} else if(addr > INREG_N || addr + num > INREG_N) {
		ModbusRequest(rxBuf[1] , 2, uartNo);
		return 2;
	}
	
	len = 5 + (num<<1)-2;//tx�ܳ���
	
	NEW08U(txBuf , len);
	txBuf[0] = rxBuf[0];
	txBuf[1] = rxBuf[1];
	txBuf[2] = num<<1;
	//�����ֽ�
	for(i=0;i<num;i++){
		txBuf[3 +(i<<1)] = HIGH16U(MODBUS_INREG_ADDR(addr + i));
		txBuf[3 +(i<<1)+ 1] = LOW16U(MODBUS_INREG_ADDR(addr + i));
	}
	
  AddTx(txBuf , len, uartNo);
	DELETE(txBuf);
	return 0;	
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
int WriteCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo){//��ȡλ״̬
	
	//CDV_INT08U len ;
	CDV_INT16U addr, num;
	//CDV_INT08U *txBuf = NULL;
//	CDV_INT16U sta, sf;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	
	num = (rxBuf[4]<<8) + rxBuf[5];
//	sta = (addr >> 3) /*+ 3*/;//����Ȧ�ĵڼ��ֽ� 0->*
//	sf = addr & 0x07;//����Ȧ�ĵڼ�λ 9->0
	if(addr > (COIL_CHN<<3) ) {
		ModbusRequest(rxBuf[1] , 2, uartNo);
		return 2;
	}else if(num != 0x0000 && num != 0xFF00) {
		ModbusRequest(rxBuf[1] , 3, uartNo);
		return 3;
	}
//	len = rxLen;//tx�ܳ���=rx ԭ��Ӧ��
//	
//	NEW08U(txBuf , len);
//	
//	memcpy(txBuf , rxBuf , len);
	
	if (0xFF00 == num){
		SET_COIL_ADDR(addr);
		//OWrite(addr,BIT_1);//����
		
	} else if (0x0000 == num){
		RESET_COIL_ADDR(addr);
		//OWrite(addr,BIT_0);//����
		
	} else {
		
	}
	

	
	AddTx(rxBuf , rxLen, uartNo);
//	AddTx(txBuf , len, uartNo);
//	DELETE(txBuf);
	return 0;	
				
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
int WriteRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo){//��ȡλ״̬
	
	CDV_INT16U addr, num;
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(addr > REG_N ) {
		ModbusRequest(rxBuf[1] , 2, uartNo);
		return 2;
	}
//	else if(num < 0x0000 || num > 0xFFFF) {
//		ModbusRequest(rxBuf[1] , 3, uartNo);
//		return;
//	}
	
	MODBUS_REG_ADDR(addr) = num;
	
  AddTx(rxBuf , rxLen, uartNo);
	SPI_Flash_Write((CDV_INT08U *)&num, (VAL_STADDR + 2 * (addr)), 2);
	return 0;
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
int WriteMultiCoil(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo){
	
	CDV_INT08U i;
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf, end, esf;
	addr = (rxBuf[2]<<8) + rxBuf[3];
	
	num = (rxBuf[4]<<8) + rxBuf[5];//д��������Ҫ��������ֽڹؼ�Ҫ������������ܿ�numch
	sta = (addr >> 3) /*+ 3*/;//��ʼchar
	end = ((addr+num-1) >> 3) /*+ 3*/;//����char
	sf = addr & 0x07;
	esf = ((addr+num-1) & 0x07) +1;
	
	numCh = end - sta;//�����
	
	if(0x0001 >num || num> 0x07B0 ||((num>>3)+(num&0x07?1:0)) != rxBuf[6]){
		ModbusRequest(rxBuf[1] , 3, uartNo);
		return 3;
	} else if(addr > (COIL_CHN<<3) || addr + num > (COIL_CHN<<3)) {
		ModbusRequest(rxBuf[1] , 2, uartNo);
		return 2;
	}
	
//	if(numCh > rxBuf[6])
//		return;//��char����
	
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
  AddTx(rxBuf , 6, uartNo);		
  return 0;	
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
int WriteMultiRegister(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U uartNo){//��ȡλ״̬
	
	CDV_INT08U i;
	CDV_INT16U addr, num , numCh , data;
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	numCh = rxBuf[6];
	
	if(0x0001 >num || num> 0x007B ||numCh != num*2){
		ModbusRequest(rxBuf[1] , 3, uartNo);
		return 3;
	} else if(addr > REG_N || addr + num > REG_N) {
		ModbusRequest(rxBuf[1] , 2, uartNo);
		return 2;
	}
	
//	else if(num < 0x0000 || num > 0xFFFF) {
//		ModbusRequest(rxBuf[1] , 3);
//		return;
//	}
	for( i = 0; i< num ; i++) {
		data = (rxBuf[7+2*i]<<8) + rxBuf[8+2*i];
		MODBUS_REG_ADDR(addr+i) = data;
	}
	
	AddTx(rxBuf , 6/*rxLen*/, uartNo);
	ValToFlash((addr>>1), (num>>1));
 
  return 0;	
}

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

CDV_INT08U ModbusParse(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CDV_INT08U uartNo){	

//	OS_ERR err;
	int ret = 0;
	switch(rxBuf[1]){ //������ɸѡ
		case 1://����Ȧ
			ret = ReadCoil(rxBuf,rxLen, uartNo);
			break;
		case 2://��������Ȧ
			ret = ReadInCoil(rxBuf,rxLen, uartNo);
			break;
		case 3://���Ĵ���
			ret = ReadRegister(rxBuf,rxLen, uartNo);
			break;
		case 4://������Ĵ���
			ret = ReadInRegister(rxBuf,rxLen, uartNo);
			break;
		case 5://д����Ȧ
			ret = WriteCoil(rxBuf,rxLen, uartNo);
			break;
		case 6://Ԥ�õ��Ĵ���
			ret = WriteRegister(rxBuf,rxLen, uartNo);
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
			ret = WriteMultiCoil(rxBuf,rxLen, uartNo);
			break;
		case 16://Ԥ�ö�Ĵ���
			ret = WriteMultiRegister(rxBuf,rxLen, uartNo);
			break;
		case 17://����ӻ���־
			break;
		case 22://��չ����------CDV����
			break;
		case 65://��չ����------CDV����
			break;
		case 0xfe://�Զ����
			break;
		default: //��������Ч��Ӧ��
			ModbusRequest(rxBuf[1], 1, uartNo);
			break;
	}
	
	return ret;
}


  /************************************************************
	********************����������Ϻ���*************************
	************************************************************/
/**
  * @brief  �ֽ����ݻ���
  *  
  * @param  
  *   
  * @retval 
  *
  * @note   
  */
void SwapByte(CDV_INT08U* rbuf, CDV_INT08U* dbuf)
{
	CDV_INT08U tmp;
	ASSERT(rbuf && dbuf);
	tmp = *rbuf;
	*rbuf = *dbuf;
	*dbuf = tmp;	
}
	

/**
  * @brief  16�ֽڸߵ��ֽڻ��������ڴ�С��ת��
  *  
  * @param  size  �Ĵ������ٸ��ֽڣ�16λ ȡ2
            num   buf�ж��ٸ��ֽ�
  *   
  * @retval CDV_INT08U*
  *
  * @note   
  */
CDV_INT08U* Endian_TF(CDV_INT08U* buf, const u16 num, const u16 size)
{
	int i, j;
	ASSERT(!(num%size));
	ASSERT(!(size%2));
//	
//	for (i = 0; i < num; i++)
//	{
//		buf[i] = ((((CDV_INT16U)(buf[i]) & 0xff00) >> 8) | (((CDV_INT16U)(buf[i]) & 0x00ff) << 8));
//	}
	
	for (i = 0; i < num / size; i++)
	{
		for (j = 0; j < size / 2; j++)
		{
			SwapByte (buf + size*i + j , buf + size*i + size - 1 - j);
		}
	}
	
	
	return buf;
}


/** @brief  ����������� д�Ĵ���
  * @param  dev     �Է��豸��
            addr    ��ַ
            num     ����
            cmdBuf  ��������
            cmdLen  ��������
  * @retval 
  * @note   �ⲿ���ø�������cmdBuf
  */
void WriteRegisterCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U* regVal, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen)
{
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	ASSERT(NULL != regVal);
	ASSERT(0x7D >= num);
	
	*cmdLen = 7 + (num << 1);
	NEW08U(*cmdBuf, *cmdLen);
	(*cmdBuf)[0] = dev;
	(*cmdBuf)[1] = 0x10;
	(*cmdBuf)[2] = (addr>>8) & 0xff;
	(*cmdBuf)[3] = (addr) & 0xff;
	(*cmdBuf)[4] = (num>>8) & 0xff;
	(*cmdBuf)[5] = (num) & 0xff;
  (*cmdBuf)[6] = num << 1;
	memcpy((*cmdBuf) + 7, Endian_TF(regVal, num << 1, 2), (*cmdBuf)[6]);
}
/** @brief  ����������� ���Ĵ���
  * @param  dev     �Է��豸��
            addr    ��ַ
            num     ����
            cmdBuf  ��������
            cmdLen  ��������
  * @retval 
  * @note   �ⲿ���ø�������cmdBuf
  */
void ReadRegisterCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen)
{
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	ASSERT(0x7D >= num);
	
	*cmdLen = 6;
	NEW08U(*cmdBuf, *cmdLen);
	(*cmdBuf)[0] = dev;
	(*cmdBuf)[1] = 0x03;
	(*cmdBuf)[2] = (addr>>8) & 0xff;
	(*cmdBuf)[3] = (addr) & 0xff;
	(*cmdBuf)[4] = (num>>8) & 0xff;
	(*cmdBuf)[5] = (num) & 0xff;
}
/** @brief  ����������� ��ֻ���Ĵ���
  * @param  dev     �Է��豸��
            addr    ��ַ
            num     ����
            cmdBuf  ��������
            cmdLen  ��������
  * @retval 
  * @note   �ⲿ���ø�������cmdBuf
  */
void ReadInRegisterCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen)
{
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	ASSERT(0x7D >= num);
	
	*cmdLen = 6;
	NEW08U(*cmdBuf, *cmdLen);
	(*cmdBuf)[0] = dev;
	(*cmdBuf)[1] = 0x04;
	(*cmdBuf)[2] = (addr>>8) & 0xff;
	(*cmdBuf)[3] = (addr) & 0xff;
	(*cmdBuf)[4] = (num>>8) & 0xff;
	(*cmdBuf)[5] = (num) & 0xff;
}
/** @brief  ����������� д��Ȧ
  * @param  dev     �Է��豸��
            addr    ��ַ
            num     ����
            cmdBuf  ��������
            cmdLen  ��������
  * @retval 
  * @note   �ⲿ���ø�������cmdBuf
  */
void WriteCoilCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U val, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen)
{
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	
	*cmdLen = 6;
	NEW08U(*cmdBuf, *cmdLen);
	(*cmdBuf)[0] = dev;
	(*cmdBuf)[1] = 0x05;
	(*cmdBuf)[2] = (addr>>8) & 0xff;
	(*cmdBuf)[3] = (addr) & 0xff;
	(*cmdBuf)[4] = val ? 0xFF : 0x00;
	(*cmdBuf)[5] = 0x00;
}
/** @brief  ����������� ��ֻ����Ȧ
  * @param  dev     �Է��豸��
            addr    ��ַ
            num     ����
            cmdBuf  ��������
            cmdLen  ��������
  * @retval 
  * @note   �ⲿ���ø�������cmdBuf
  */
void ReadInCoilCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen)
{
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	ASSERT(0x7D0 >= num);
	
	*cmdLen = 6;
	NEW08U(*cmdBuf, *cmdLen);
	(*cmdBuf)[0] = dev;
	(*cmdBuf)[1] = 0x02;
	(*cmdBuf)[2] = (addr>>8) & 0xff;
	(*cmdBuf)[3] = (addr) & 0xff;
	(*cmdBuf)[4] = (num>>8) & 0xff;
	(*cmdBuf)[5] = (num) & 0xff;
}
/** @brief  ����������� ����Ȧ
  * @param  dev     �Է��豸��
            addr    ��ַ
            num     ����
            cmdBuf  ��������
            cmdLen  ��������
  * @retval 
  * @note   �ⲿ���ø�������cmdBuf
  */
void ReadCoilCmd(CDV_INT08U dev, CDV_INT16U addr, CDV_INT16U num, 
     CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen)
{
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	ASSERT(0x7D0 >= num);
	
	*cmdLen = 6;
	NEW08U(*cmdBuf, *cmdLen);
	(*cmdBuf)[0] = dev;
	(*cmdBuf)[1] = 0x01;
	(*cmdBuf)[2] = (addr>>8) & 0xff;
	(*cmdBuf)[3] = (addr) & 0xff;
	(*cmdBuf)[4] = (num>>8) & 0xff;
	(*cmdBuf)[5] = (num) & 0xff;
}
  /************************************************************
	********************����������Ϻ���*************************
	************************************************************/
/** @brief  ����������� ����Ȧ 01
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ⲿ���ø�������cmdBuf
  */
int ReadCoilReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen){//��ȡλ״̬
	
	CDV_INT08U i;
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf;
	
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(0x0001 >num || num> 0x07D0){
		return 3;
	} else if(addr > (COIL_CHN<<3) || addr + num > (COIL_CHN<<3)) {
		return 2;
	}
	
	sta = (addr >> 3) /*+ 3*/;//����Ȧ�ĳ�ʼchar
	sf = addr & 0x07;
	numCh = (num>>3)+((num & 0x07)?1:0);//��char����
	
	
	*cmdLen = 5 + numCh-2;//tx�ܳ���
	NEW08U(*cmdBuf , *cmdLen);
	(*cmdBuf)[0] = rxBuf[0];
	(*cmdBuf)[1] = rxBuf[1];
	(*cmdBuf)[2] = numCh;//;(num>>3)
	//�����ֽ�
	for(i=0;i<(*cmdBuf)[2]+1;i++){//�����һλ����ֹ���һ���ֽ��ٶ�λ
		(*cmdBuf)[3+i] = MODBUS_COIL_CH(sta+i);
	}
	//ƫ��
	for(i=0;i<(*cmdBuf)[2];i++){
		(*cmdBuf)[3+i] = (*cmdBuf)[3+i]>>sf | (*cmdBuf)[4+i]<<(8-sf);
	}
	if(/*sf != 0 && */i!= 0)
		(*cmdBuf)[3+i-1] &= (0xFF>>(8 - (0 ==(num & 0x07)?8:(num & 0x07))));// ��0
	
	return 0;
}

/** @brief  ����������� ��������Ȧ 02
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ⲿ���ø�������cmdBuf
  */
int ReadInCoilReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen){//��ȡλ״̬
	
	CDV_INT08U i;
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf;
	
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	
	addr = (rxBuf[2]<<8) + rxBuf[3];
	num = (rxBuf[4]<<8) + rxBuf[5];//��i����
	
	if(0x0001 >num || num> 0x07D0){
		return 3;
	} else if(addr > (INCOIL_CHN<<3) || addr + num > (INCOIL_CHN<<3)) {
		return 2;
	}
	
	for(i = 0 ; i < CDV_I_NUM; i ++) {
	  IRead(i);
	}
	
	sta = (addr >> 3) /*+ 3*/;//����Ȧ�ĳ�ʼchar
	sf = addr & 0x07;
	numCh = (num>>3)+((num & 0x07)?1:0);//��char����
	*cmdLen = 5 + numCh-2;//tx�ܳ���
	
	NEW08U((*cmdBuf) , *cmdLen);
	(*cmdBuf)[0] = rxBuf[0];
	(*cmdBuf)[1] = rxBuf[1];
	(*cmdBuf)[2] = numCh;//;(num>>3)
	//�����ֽ�
	for(i=0;i<(*cmdBuf)[2]+1;i++){//�����һλ����ֹ���һ���ֽ��ٶ�λ
				(*cmdBuf)[3+i] = MODBUS_INCOIL_CH(sta+i);
			}
	//ƫ��
	for(i=0;i<(*cmdBuf)[2];i++){
				(*cmdBuf)[3+i] = (*cmdBuf)[3+i]>>sf | (*cmdBuf)[4+i]<<(8-sf);
			}
	if(/*sf != 0 &&*/ i!= 0)
		(*cmdBuf)[3+i-1] &= (0xFF>>(8 - (0 ==(num & 0x07)?8:(num & 0x07))));// ��0
	
	return 0;
}

/** @brief  ����������� ���Ĵ��� 03
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ⲿ���ø�������cmdBuf
  */
int ReadRegisterReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen){//��ȡλ״̬
	
	CDV_INT08U i;
	CDV_INT16U addr, num;
		
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(0x0001 >num || num> 0x007D){
		return 3;
	} else if(addr > REG_N || addr + num > REG_N) {
		return 2;
	}
	
	*cmdLen = 5 + (num<<1)-2;//tx�ܳ���
	
	NEW08U((*cmdBuf) , *cmdLen);
	(*cmdBuf)[0] = rxBuf[0];
	(*cmdBuf)[1] = rxBuf[1];
	(*cmdBuf)[2] = num<<1;
	//�����ֽ�
	for(i=0;i<num;i++){
		(*cmdBuf)[3 +(i<<1)] = HIGH16U(MODBUS_REG_ADDR(addr + i));
		(*cmdBuf)[3 +(i<<1)+ 1] = LOW16U(MODBUS_REG_ADDR(addr + i));
	}

	return 0;	
}

/** @brief  ����������� ������Ĵ��� 04
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ⲿ���ø�������cmdBuf
  */
int ReadInRegisterReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen){//��ȡλ״̬
	
	CDV_INT08U i;
	CDV_INT16U addr, num;
		
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(0x0001 >num || num> 0x007D){
		return 3;
	} else if(addr > INREG_N || addr + num > INREG_N) {
		return 2;
	}
	
	*cmdLen = 5 + (num<<1)-2;//tx�ܳ���
	
	NEW08U((*cmdBuf) , *cmdLen);
	(*cmdBuf)[0] = rxBuf[0];
	(*cmdBuf)[1] = rxBuf[1];
	(*cmdBuf)[2] = num<<1;
	//�����ֽ�
	for(i=0;i<num;i++){
		(*cmdBuf)[3 +(i<<1)] = HIGH16U(MODBUS_INREG_ADDR(addr + i));
		(*cmdBuf)[3 +(i<<1)+ 1] = LOW16U(MODBUS_INREG_ADDR(addr + i));
	}
	
	return 0;	
}

/** @brief  ����������� д����Ȧ05
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ⲿ���ø�������cmdBuf
  */
int WriteCoilReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen){//��ȡλ״̬
	CDV_INT16U addr, num;
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	addr = (rxBuf[2]<<8) + rxBuf[3];
	
	num = (rxBuf[4]<<8) + rxBuf[5];
	if(addr > (COIL_CHN<<3) ) {
		return 2;
	}else if(num != 0x0000 && num != 0xFF00) {
		return 3;
	}
	
	if (0xFF00 == num){
		SET_COIL_ADDR(addr);
		
	} else if (0x0000 == num){
		RESET_COIL_ADDR(addr);
		
	} else {
		
	}
	
	if(READ_O(addr))
	{
		OWrite(addr, BIT_1);
		
		if(addr == 200)
			ChangeToCdvStat(CDV_RUN);
		else if(addr == 201)
			ChangeToCdvStat(CDV_PAUSE);
	}
	else
	{
		OWrite(addr, BIT_0);
		
		if(addr == 200)
			ChangeToCdvStat(CDV_ONLINE);
		else if(addr == 201)
			ChangeToCdvStat(CDV_RUN);
	}
	*cmdLen = 6;
	NEW08U((*cmdBuf) , *cmdLen);
	memcpy((*cmdBuf), rxBuf , 6);
	return 0;	
				
}
				
/** @brief  ����������� д���Ĵ���0x06
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ⲿ���ø�������cmdBuf
  */
int WriteRegisterReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen){//��ȡλ״̬
	
	CDV_INT16U addr, num;
		
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	
	if(addr > REG_N ) {
		return 2;
	}
	
	MODBUS_REG_ADDR(addr) = num;
	
	*cmdLen = 6;
	NEW08U((*cmdBuf) , *cmdLen);
	memcpy((*cmdBuf), rxBuf , 6);
	
	SPI_Flash_Write((CDV_INT08U *)&num, (VAL_STADDR + 2 * (addr)), 2);
	return 0;
}
			
/** @brief  ����������� д����Ȧ0x0F
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  * @retval ����ֵ˵��
  * @note   �ⲿ���ø�������cmdBuf
  */
int WriteMultiCoilReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen){
	
	CDV_INT08U i;
	CDV_INT16U addr, num, numCh;
	CDV_INT16U sta, sf, end, esf;
		
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	addr = (rxBuf[2]<<8) + rxBuf[3];
	
	num = (rxBuf[4]<<8) + rxBuf[5];//д��������Ҫ��������ֽڹؼ�Ҫ������������ܿ�numch
	sta = (addr >> 3) /*+ 3*/;//��ʼchar
	end = ((addr+num-1) >> 3) /*+ 3*/;//����char
	sf = addr & 0x07;
	esf = ((addr+num-1) & 0x07) +1;
	
	numCh = end - sta;//�����
	
	if(0x0001 >num || num> 0x07B0 ||((num>>3)+(num&0x07?1:0)) != rxBuf[6]){
		return 3;
	} else if(addr > (COIL_CHN<<3) || addr + num > (COIL_CHN<<3)) {
		return 2;
	}

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
	*cmdLen = 6;
	NEW08U((*cmdBuf) , *cmdLen);
	memcpy((*cmdBuf), rxBuf , 6);
  return 0;	
}
			
/** @brief  ����������� д��Ĵ���
  * @param  rxBuf     �������ַ���ָ��
  *         rxLen     �������ַ����ĳ���
  * @retval 
  * @note   �ⲿ���ø�������cmdBuf
  */
int WriteMultiRegisterReqCmd(CDV_INT08U* rxBuf,CDV_INT08U rxLen, CDV_INT08U** cmdBuf,CDV_INT08U* cmdLen){//��ȡλ״̬
	
	CDV_INT08U i;
	CDV_INT16U addr, num , numCh , data;
		
	ASSERT(NULL != cmdBuf);
	ASSERT(NULL != cmdLen);
	addr = (rxBuf[2]<<8) + rxBuf[3];

	num = (rxBuf[4]<<8) + rxBuf[5];
	numCh = rxBuf[6];
	
	if(0x0001 >num || num> 0x007B ||numCh != num*2){
		return 3;
	} else if(addr > REG_N || addr + num > REG_N) {
		return 2;
	}
	
	for( i = 0; i< num ; i++) {
		data = (rxBuf[7+2*i]<<8) + rxBuf[8+2*i];
		MODBUS_REG_ADDR(addr+i) = data;
	}
	
	*cmdLen = 6;
	NEW08U((*cmdBuf) , *cmdLen);
	memcpy((*cmdBuf), rxBuf , rxLen-2);
	ValToFlash((addr>>1), (num>>1));
 
  return 0;	
}


  /************************************************************
	********************���ڷ�����������*************************
	************************************************************/

/** @brief  �������� ��Ȧ
  * @param  buf     �������ַ���ָ��
            len     �������ַ����ĳ���
            bit     ��ȡ�ڼ�����Ȧ
            var     ���ĺñ���
  * @retval 
  * @note
  */
RET_STATUS ReadCoilReqToVar(CDV_INT08U* buf,CDV_INT08U len, CDV_INT08U bit, CDV_INT32U var){//��ȡλ״̬
	CDV_INT08U sta, sf;
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U* p_bit = buf + 3;
	RET_STATUS ret = OPT_FAILURE;
	
	if(0x01 != fc || len < numCh + 3)
		return ret;
	
	sta = (bit >> 3);
	sf = bit & 0x07;
	
	ValSet(var, (p_bit[sta] >> sf) & 0x01);
	
	ret = OPT_SUCCESS;
	return ret;
}
/** @brief  �������� ֻ����Ȧ
  * @param  buf     �������ַ���ָ��
            len     �������ַ����ĳ���
            bit     ��ȡ�ڼ�����Ȧ
            var     ���ĺñ���
  * @retval 
  * @note
  */
RET_STATUS ReadInCoilReqToVar(CDV_INT08U* buf,CDV_INT08U len, CDV_INT08U bit, CDV_INT32U var){//��ȡλ״̬
	CDV_INT08U sta, sf;
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT08U* p_bit = buf + 3;
	RET_STATUS ret = OPT_FAILURE;
	
	if(0x02 != fc || len < numCh + 3)
		return ret;
	
	sta = (bit >> 3);
	sf = bit & 0x07;
	
	ValSet(var, (p_bit[sta] >> sf) & 0x01);
	
	ret = OPT_SUCCESS;
	return ret;
}
/** @brief  �������� �Ĵ���
  * @param  buf     �������ַ���ָ��
            len     �������ַ����ĳ���
            reg     ��ȡ�ڼ����Ĵ���
            var     ���ĺñ���
  * @retval 
  * @note
  */
RET_STATUS ReadRegReqToVar(CDV_INT08U* buf,CDV_INT08U len, CDV_INT08U reg, CDV_INT32U var){//��ȡλ״̬
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT16U* p_reg = (CDV_INT16U*)(buf + 3);
	RET_STATUS ret = OPT_FAILURE;
	CDV_INT32S num;
	
	if(0x03 != fc || len < numCh + 3)
		return ret;
	
	if(numCh > 2)
	{
		num = ((ENDIAN_TF16U(p_reg[reg + 1]))<<16)| ENDIAN_TF16U(p_reg[reg]);
	}
	else
	{
		num = ENDIAN_TF16U(p_reg[reg]);
	}
	
	
	ValSet(var, num);
	
	ret = OPT_SUCCESS;
	return ret;
}
/** @brief  �������� ֻ���Ĵ���
  * @param  buf     �������ַ���ָ��
            len     �������ַ����ĳ���
            reg     ��ȡ�ڼ����Ĵ���
            var     ���ĺñ���
  * @retval 
  * @note
  */
RET_STATUS ReadInRegReqToVar(CDV_INT08U* buf,CDV_INT08U len, CDV_INT08U reg, CDV_INT32U var){//��ȡλ״̬
	CDV_INT08U fc = buf[1];
	CDV_INT08U numCh = buf[2];
	CDV_INT16U* p_reg = (CDV_INT16U*)(buf + 3);
	RET_STATUS ret = OPT_FAILURE;
	
	if(0x04 != fc || len < numCh + 3)
		return ret;
	
	ValSet(var, ENDIAN_TF16U(p_reg[reg]));
	
	ret = OPT_SUCCESS;
	return ret;
}

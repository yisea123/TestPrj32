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


/*GPIO����*/

void NPC_GpioConfig(void){
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO, ENABLE );	  //ʹ��GPIOA GPIOB GPIOC GPIODͨ��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	 	  // �˿�����PA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_7|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_15;	 	// �˿�����PA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	 	// �˿�����PB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	 	// �˿�����PB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	 	// �˿�����PC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;	 	// �˿�����PC
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 		 //����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);	    //�رյ��� �˿�����ӳ��  ʹ�÷���������ʱ�������ô���  SWJ ��ȫ����
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //�رյ��� �˿�����ӳ��  ʹ�÷���������ʱ�������ô���  JTAG-DP ���� + SW-DP ʹ��
	GPIO_SetBits(GPIOA,GPIO_Pin_All);
	GPIO_SetBits(GPIOB,GPIO_Pin_All);
	GPIO_SetBits(GPIOC,GPIO_Pin_All);
	GPIO_SetBits(GPIOD,GPIO_Pin_All);
}

void NPC_IOInit(void) {
	
//	O��ʼ��
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
	
//	I��ʼ��

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
		
//	���뿪�س�ʼ��
	
	g_dipSW[0].port = PORT_A;
	g_dipSW[0].pin  = PIN_0;
	g_dipSW[1].port = PORT_A;
	g_dipSW[1].pin  = PIN_1;
	g_dipSW[2].port = PORT_A;
	g_dipSW[2].pin  = PIN_7;
	
}

/**
  * @brief  I��������
  *  
  * @param  no     Ҫ���õ�I��
  *         flag   ���õĲ������
						buf    ����buf
  *   
  * @retval ��
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
  * @brief  I������ȡ
  *  
  * @param  no     Ҫ���õ�I��
  *         flag   ���õĲ������
						buf    ����buf
  *   
  * @retval ��
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
  * @brief  CDV ��I 30
  *  
  * @param  no     Ҫ����I��
  *   
  * @retval CDV_INT08U��1�ߵ�ƽ��0�͵�ƽ
  *
  * @note   ˢ��modbus��Ȧ
  */
CDV_INT08U ExIRead(CDV_INT32U no) {
	CDV_INT08U val;
//	val = FpgaIRead(no);
	return val;
}
/**
  * @brief  CDV ��O 34
  *  
  * @param  no     Ҫ����O��
  *   
  * @retval CDV_INT08U��1�ߵ�ƽ��0�͵�ƽ
  *
  * @note   ˢ��modbus��Ȧ
  */
CDV_INT08U ExORead(CDV_INT32U no) {
	CDV_INT08U val;
//	val = FpgaORead(no);
  return val;
}
/**
  * @brief  CDV O��� 34
  *  
  * @param  no     Ҫ����I��
  *         val    CDV_INT08U��1�ߵ�ƽ��0�͵�ƽ
  *   
  * @retval ��
  *
  * @note   ˢ��modbus��Ȧ
  */
void ExOWrite(CDV_INT08U no , CDV_INT08U val) {
//	FpgaOWrite(no , val);
}
/**
  * @brief  CDV ��I(����EXI
  *  
  * @param  no     Ҫ����I��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ˢ��modbus��Ȧ
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
  * @brief  CDV ��I(����EXI
  *  
  * @param  no     Ҫ����I��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ��ȡ����npc�˿ڵ�bitֵ��bit1��Ӧ1��bit0��Ӧ0
  */
CDV_INT08U IBitRead(CDV_INT32U no) {
	CDV_INT08U val;
	IO_VAL bit = IRead(no);
	val = (BIT_1 == bit ? 1 : 0);
	return val;
}

/**
  * @brief  CDV ��I,����(����EXI
  *  
  * @param  no     Ҫ����I��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ˢ��modbus��Ȧ
20151225:�������ж�cdv״̬������������У����˳�while��û���ж���ͣ
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
		
		if (cnt > 0) {//�ж�ǰ������ǲ�����ȵ�
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
  * @brief  CDV ��O������EXO
  *  
  * @param  no     Ҫ����O��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ˢ��modbus��Ȧ
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
  * @brief  CDV ��O������EXO
  *  
  * @param  no     Ҫ����O��
  *   
  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *
  * @note   ��ȡ����npc�˿ڵ�bitֵ��bit1��Ӧ1��bit0��Ӧ0
  */
CDV_INT08U OBitRead(CDV_INT32U no) {
	CDV_INT08U val;
	IO_VAL bit = ORead(no);
	val = (BIT_1 == bit ? 1 : 0);
	return val;
}
/**
  * @brief  CDV O���(����EXO
  *  
  * @param  no     Ҫ����I��
  *         val    IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
  *   
  * @retval ��
  *
  * @note   ˢ��modbus��Ȧ
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
//  * @brief  CDV �����뿪��
//  *  
//  * @param  no     Ҫ����DIP��
//  *   
//  * @retval IO_VALö��ֵ��BIT_1�ߵ�ƽ��BIT_0�͵�ƽ
//  *
//  * @note   ˢ��modbus��Ȧ
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
  * @brief  I�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��
            OPT_SUCCESS����ִ�����
            OPT_FAILURE����δִ����ɣ�����ȴ�δ�ȵ�
  *
  * @note   ��Դ�ţ�00-FF��	00�ȴ�	ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	�ַ���
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
//	//strLen = rxLen - 3;//���ĳ���
 // OS_ERR err;

	no  = *(CDV_INT32U*)(rxBuf);//rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	num  = *(CDV_INT32U*)(rxBuf + 6);

	switch(opt) {
		case 0x00:/*�ȴ�*/
//			  num = Arithmetic((char*)rxBuf + 6, rxLen - 6);
		    bit = (num)?BIT_1:BIT_0;
		    if(bit != IRead(no))
				  ret = OPT_FAILURE;
			break;
		case 0x01:/*�����ȴ�*/
		    bit = (num)?BIT_1:BIT_0;
		    if(bit != ShakeIRead(no))
				ret = OPT_FAILURE;
			break;
		case 0x02:/*��ѯ*/
			bit = IRead(no);
		  val = (BIT_1 == bit ? 1 : 0);
//			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg);
		  break;
		case 0x03:/*������ѯ*/
			bit = ShakeIRead(no);
			val = (BIT_1 == bit ? 1 : 0);
//			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg);
			break;
		case 0x04:/*��ȡ������*/
			if(0xFE == type) {
				bit = IRead(no);
			  val = (BIT_1 == bit ? 1 : 0);
//				ResVarSet(num, val, arg);
			}
			break;
		case 0x05:/*��������*/
			if(0xFF == type) {
			  IParWrite(no , num , rxBuf + 10);
			}
			break;
		case 0x06:/*������ȡ*/			
			if(0xFF == type) {
			  IParRead(no , num , tmpPar , &tmpLen);
//			  ResRequest(arg->buf, arg->len, tmpPar, tmpLen, arg);
			}
			break;
		case 0x07:/*����ȴ�*/
//			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
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
		case 0x08:/*������ȴ�*/
//			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
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
		case 0x09:/*������ȴ�*/
//			OSSemPend(&PLUSE_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err); //�����ź���
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
		case 0x0A:/*ȥ����ȡ������*/
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
  * @brief  O�ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   ��Դ�ţ�00-FF��	00���	ö�ٺţ�00-03	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	�ַ���
		                                FE	�������Ÿ�λ	�������ŵ�λ							

  */

RET_STATUS OCmd(CDV_INT08U* rxBuf, CDV_INT08U rxLen, CMD_ARG *arg) { 
//	OS_ERR err;
  //CDV_INT08U no , opt, type, val  /*, strLen*/;
	CDV_INT08U opt, type;
	CDV_INT32U num , no, val;
	IO_VAL bit;
	//OS_ERR err;

//	//strLen = rxLen - 3;//���ĳ���
//	no   = rxBuf[0];
//	opt  = rxBuf[1];
//	type = rxBuf[2];	
	no  = *(CDV_INT32U*)(rxBuf);//rxBuf[0];
	opt = rxBuf[4];
	type = rxBuf[5];
	num  = *(CDV_INT32U*)(rxBuf + 6);

	switch(opt) {
		case 0x00:/*���*/			
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
		case 0x01:/*�ȴ�*/
//			num = Arithmetic((char*)rxBuf + 6, rxLen - 6);
		  bit = (num) ? BIT_1 : BIT_0;
		  if(bit != ORead(no))
				return OPT_FAILURE;
		  break;
		case 0x04:/*��ѯ*/
			bit = ORead(no);
		  val = (BIT_1 == bit ? 1 : 0);
//			ResRequest(arg->buf, arg->len, (CDV_INT08U*)(&val), 4, arg);
		  break;
		case 0x02:/*��ȡ������*/
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
//����I��״̬��ȡ
//num:�ڼ���I�ڣ�1-20
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
//����O��״̬��ȡ
//num:�ڼ���I�ڣ�1-22
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

//�������
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




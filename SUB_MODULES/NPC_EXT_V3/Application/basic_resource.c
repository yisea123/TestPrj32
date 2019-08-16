/**
  ******************************************************************************
  * @file    /cdv_resource.c 
  * @author  MMY
  * @version V2.0.0
  * @date    27-June-2015
  * @brief   user function
  * 
@verbatim  

               CDV������Դ���ƺ�����ǣ��������ʱmodbus���ѻ�����ʱֱ�Ӳ���������modbus�������ļ��Ĺ���
							 20151221��DisplayCmd�иı����ĳ��ȵ��жϹ��򣬸������һ���ֽ��ж�
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
	
	
	
	/*CDV��Դȫ�ֱ�������CDVInit�г�ʼ��*/


//CDV_INT08U resReqed = 0;
//CDV_INT08U g_resUart = 0;/*��ǵ���cmdparseʱ�Ĵ��ڣ����ڻظ�*/
/**
  * @brief  CDV��Դ��ʼ��
  *  
  * @param  ��
  *   
  * @retval ��
  *
  * @note   ��������CDV��Դ�ı���
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
	/*modbus��ʼ��*/
	
//#ifndef	CDV_V1
    /*FPGA������������*/
//	CH_CDV_DIP = 0;
//	for(i = 0; i < CDV_DIP_NUM ; i++){
//		CH_CDV_DIP |= ((DIP_ON == READ_DIP_SW(i) ? 1 : 0)<< i);/*���뿪�ش�1��4�ֱ����fpga0-3�����س���*/
//	}
	
ClearUniversalCnt();

    //FpgaRecvCtl();	
//	if(DIP_OFF == READ_DIP_SW(5) && DIP_OFF == READ_DIP_SW(4)/* && DIP_OFF == READ_DIP_SW(0)*/) {
//	/*������Ӷ�ȡfpga������Ϣ��Ȼ����в�ͬ������*/
//		//FpgaInit(RW_CFG_FPGA);
////	  for( i = 1; i<= 2 ; i++) {
////		  if(dip & (0x01 << i))
////		  {
////			  Fpga_Config(i, FPGA_MOTOR);//������������
////			  Fpga_Down(i, FPGA_MOTOR);
////		  } else {
////			  Fpga_Config(i, FPGA_IO);//�ڶ���
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
  * @brief  �����ַ�����ִ������
  *  
  * @param  rxBuf     �ַ���
  *         rxLen     �ַ�������     
  *         debug      0�ѻ�����
                       1�ѻ�����
                       2��������
  *   
  * @retval ����ֵ˵��
            OPT_SUCCESS����ִ�����
            OPT_FAILURE����δִ����ɣ�����ȴ�δ�ȵ�
  *
  * @note   
  */

RET_STATUS ResParse(CDV_INT08U* rxBuf, CDV_INT16U rxLen , CMD_ARG *arg) {
	RET_STATUS ret = OPT_SUCCESS;
	
	switch(rxBuf[0]){
		case 0x05:/*����*/
			ret = ValCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x08:/*�߼���Դ*/
			ret = LogicScript(rxBuf+1, rxLen - 1, arg);
			break;
		case 0x00:/*��������*/
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
		case 0x03:/*��ʱ��*/
			ret = TimeCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x04:/*��Ϣ*/
			ret = MsgCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x0B:
			ret = AdcCmd(rxBuf + 1,rxLen - 1, arg);
		  break;
		case 0x0C:
			ret = DacCmd(rxBuf + 1,rxLen - 1, arg);
			break;
		case 0x0D:/*��ʱ��*/
			ret = ClockCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x0E:/*����*/
			ret = WorkerCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0x10:/*������*/
			ret = LineCmd(rxBuf + 1, rxLen - 1, arg);
			break;
		case 0xFE:/*ϵͳ����*/
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
//  * @brief  ��ʾ�ű�����
//  *  
//  * @param  scriptNo     ���������˺�  
//  *   
//  * @retval ����ֵ˵��
//            OPT_SUCCESS����ִ�����
//  *
//  * @note   ʹ�õ�UCOS�������ƣ���֤ÿ��ֻ��һ���ڲ�������tx����
//						��ʾ��Դ����	��ʾ��Դ��	�кţ�00-FF��																															�ַ���
//						00�����ᣩ		��00-FF��		��00-FF��									
//						01��I)				��00-FF��		��00-FF��	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	
//						02��O��				��00-FF��		��00-FF��	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	
//						03����ʱ����	��00-FF��		��00-FF��									
//						04����Ϣ��		��00-FF��		��00-FF��	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	ö��ֵ	
//						05��������		��00-FF��		��00-FF��									
//						FF��ע�ͣ�		��00-FF��									
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
//	strLen = rxBuf[rxLen - 1];//��ȡ���һ���ֽڣ���ʾ����ǰ������Ĵ�����
//	switch(rxBuf[0]) {
//		case 0x09:
//			//strLen = rxLen - 2;//���ĳ���
//			len = rxLen/*strLen + 4*/;//����APP���ַ�������
//			row = rxBuf[2];//�ڼ�����ʾ
//		    type = 0x05;//ע������
//			break;
////		case 0x00:
////			//strLen = rxLen - 3;//���ĳ���
////			len = strLen + 8;//����APP���ַ�������
////			row = rxBuf[2];//�ڼ�����ʾ
////		  type = 0xFF;//��ֵ����
////		  //num = R_MOTOR_POS(SCRIPT_DISPLAY_RES_NO(scriptNo));
//////		  READ_MOTOR_POS(rxBuf[1] , num);//ȡ��Ҫ��ʾ��intֵ
////			break;
////		case 0x03:
////			//strLen = rxLen - 3;//���ĳ���
////			len = strLen + 8;//����APP���ַ�������
////			row = rxBuf[2];//�ڼ�����ʾ
////		  type = 0xFF;//��ֵ����
//////		  num = RW_TIMER(rxBuf[1]);//ȡ��Ҫ��ʾ��intֵ
////			break;
////		case 0x04:
////			//strLen = rxLen - 3;//���ĳ���
////			len = strLen + 4;//����APP���ַ�������
////			row = rxBuf[2];//�ڼ�����ʾ
////		  type = READ_MSG(rxBuf[1]);//ȡ��Ҫ��ʾ��intֵ
////			break;
//		case 0x01:
//			//strLen = rxLen - 3;//���ĳ���
//			len = strLen + 12;//����APP���ַ�������
//			row = rxBuf[2];//�ڼ�����ʾ
//		    type = IRead(rxBuf[1]);//ȡ��Ҫ��ʾ��intֵ
//			break;
//		case 0x02:
//			//strLen = rxLen - 3;//���ĳ���
//			len = strLen + 12;//����APP���ַ�������
//			row = rxBuf[2];//�ڼ�����ʾ
//		    type = ORead(rxBuf[1]);//ȡ��Ҫ��ʾ��intֵ
//			break;
//		case 0x05:
//			//strLen = rxLen - 3;//���ĳ���
//			len = strLen + 8;//����APP���ַ�������
//			row = rxBuf[2];//�ڼ�����ʾ
//		    type = 0xFF;//��ֵ����
//		    num = VarGet(rxBuf[1]);//ȡ��Ҫ��ʾ��intֵ
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
//	/*����ʾ���ݼ��ص�����tx����*/
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
//	//OSTimeDlyHMSM(0,0,0,USART_SEND_GAP,OS_OPT_TIME_HMSM_STRICT,&err);   //��ʱ 10ms
//	return OPT_SUCCESS;
//}


/**
  * @brief  ��ʱ���ű�����
  *  
  * @param  scriptNo     ���������˺�  
  *   
  * @retval ����ֵ˵��OPT_SUCCESS��OPT_FAILURE
  *
  * @note   ��Դ�ţ�00-FF��	00��ʱ	FF	uint32��ֵ��λ	uint32��ֵ	uint32��ֵ	uint32��ֵ��λ					�ַ���
		                                FE	�������Ÿ�λ	�������ŵ�λ							
  */



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//************************************************
//�����ǰ/����˶�
//no������ţ�0-7��
//num����ǰ����
//type��1��ǰ��0���
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
//	if(1 != MODBUS_READ_CFG_FPGA(no>>1))//�ж����õ��ǲ��ǵ��
//		return OPT_SUCCESS;
//  fpgaPar.acc = RW_MOTOR(no).acc;
//	fpgaPar.dec = RW_MOTOR(no).dec;
//	fpgaPar.maxSpd = RW_MOTOR(no).maxSpeed;
//	MODBUS_RESET_MOTOR_END(no);
////	if(0 != num){
//			fpgaPar.len = num;
////		if(0 != fpgaPar.len) {
//			if(MODBUS_READ_RUN_DIR(no)) {//����û��趨�˷�ת����
//				dir = dir? 0 :1;
//			}
//			/*����ƶ�*/
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
  * @brief  �����������
  *  
  * @param  no     Ҫ�����ĵ����
  *         num    0    ����
  *                ���� �����˶�Ŀ��λ��(����)
  *   
  * @retval 
  *
  * @note   ���Ƶ���������˶��������˶��������
  *         ��modbus�Ĵ����ж�RW_MOTOR��maxSpeed��acc��dec��upLimit��downLimit��minSpeed��������״̬ʱ�趨�ã�
  *                           R_MOTOR_POS (�����Ҫά��ֵ���ɶ�д��ÿ�η����˶�����ɹ�����ĸ�ֵ)
  *         ���������FPGA
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
//	if(1 != MODBUS_READ_CFG_FPGA(no>>1))//�ж����õ��ǲ��ǵ��
//		return OPT_SUCCESS;
//  fpgaPar.acc = RW_MOTOR(no).acc;
//	fpgaPar.dec = RW_MOTOR(no).dec;
//	fpgaPar.maxSpd = RW_MOTOR(no).maxSpeed;
//	MODBUS_RESET_MOTOR_END(no);
//	if(0 != num){
//		if(dir) {//�趨Ҫ�ߵĲ���
//			fpgaPar.len	= num - FPGA_POS;
//		} else {
//			fpgaPar.len = FPGA_POS - num;
//		}
//		if(0 != fpgaPar.len) {
//			if(MODBUS_READ_RUN_DIR(no)) {//����û��趨�˷�ת����
//				dir = dir? 0 :1;
//			}
//			/*�����ƶ�*/
//			FpgaMotoRun(no ,1,dir ,&fpgaPar);
//			//WRITE_MOTOR_POS(no , num);
//		}
//		}else {
//		if(MODBUS_READ_ZERO_DIR(no)) {//����û��趨�˷�ת����
//			dir = dir? 0 :1;
//		}
//		
//		/*����*/
//		//__nop();
//		FpgaMotoRun(no ,0,dir ,&fpgaPar);
//		CLEAR_MOTOR_POS(no);
//		//memset( R_MOTOR_PPOS(no) , 0 , 4);
//	}

//#endif
//	return OPT_SUCCESS;
//}	

/**
  * @brief  ��������������
  *  
  * @param  no     Ҫ�����ĵ����
  *         num    0    ����
  *                ���� �����˶�Ŀ��λ��(����)
  *   
  * @retval 
  *
  * @note   �����MotorStepRefresh();	��cdvˢ���߳�
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
  * @brief  ���ʵ�ʾ�������
  *  
  * @param  no     Ҫ�����ĵ����
  *         num    0    ����
  *                ���� �����˶�Ŀ��λ��(����)
  *   
  * @retval 
  *
  * @note   �����MotorStepRefresh();	��cdvˢ���߳�
  */
RET_STATUS ActualMotorRun(CDV_INT08U no /*, CDV_INT08U opt*/ , CDV_INT32S num) {	
//#ifndef	CDV_V1
////	WRITE_DST_POS(no ,num);
//	Moto_Solute_Run(no,num);
//#endif
	return OPT_SUCCESS;
}

/**
  * @brief  �����ͣ
  *  
  * @param  no     Ҫ�����ĵ����
  *         num    0    ����
  *                ���� �����˶�Ŀ��λ��(����)
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
//	if(1 != MODBUS_READ_CFG_FPGA(no>>1))//�ж����õ��ǲ��ǵ��
//		return OPT_SUCCESS;
//	
//    fpgaPar.acc = RW_MOTOR(no).acc;
//	fpgaPar.dec = RW_MOTOR(no).dec;
//	fpgaPar.maxSpd = RW_MOTOR(no).maxSpeed;
//	MODBUS_RESET_MOTOR_END(no);
//	fpgaPar.len	= 0;
//	/*�����ƶ�*/
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
//  * @brief  ���Բ������
//  *  
//  * @param  no     Ҫ�����ĵ����
//  *   
//  * @retval 
//  *
//  * @note   ���Ƶ���������˶��������˶��������
//	*         ��modbus�Ĵ����ж�RW_MOTOR��maxSpeed��acc��dec��upLimit��downLimit��minSpeed��������״̬ʱ�趨�ã�
//	*                           R_MOTOR_POS (�����Ҫά��ֵ���ɶ�д��ÿ�η����˶�����ɹ�����ĸ�ֵ)
//	*         ���������FPGA
//  */
//RET_STATUS MotorArc(CDV_INT08U no , CDV_INT32U x , CDV_INT32U y) {
//	
//	
//	
//	return OPT_SUCCESS;
//}
///**
//  * @brief  �����ѯ
//  *  
//  * @param  no     Ҫ�����ĵ����0-7
//	*         opt    0 �ȴ��������
//	*                1 
//	*                2  
//  *   
//  * @retval 
//  *
//  * @note   ��ѯ�����λ���ٶȣ����ٶȵ�
//	*         ��modbus�Ĵ����ж�RW_MOTOR��maxSpeed��acc��dec��upLimit��downLimit��minSpeed��������״̬ʱ�趨�ã�
//	*                           R_MOTOR_POS (�����Ҫά��ֵ���ɶ�д��ÿ�η����˶�����ɹ�����ĸ�ֵ)
//	*         ���������FPGA
//  */
//RET_STATUS MotorChk(CDV_INT08U no , CDV_INT08U opt) {
////	CDV_INT08U val;
//	RET_STATUS ret =OPT_SUCCESS;
//#ifndef	CDV_V1
//	if(1 != MODBUS_READ_CFG_FPGA(no>>1))//�ж����õ��ǲ��ǵ��
//		return OPT_SUCCESS;
//	
//	switch (opt) {
//		case 0:
//			/*�ȴ��������*/
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
//  * @brief  CDV Iˢ�¡�
//  *  
//  * @param  ��
//  *   
//  * @retval ��
//  *
//  * @note   ��ȡ��������I��ֵ��ˢ��modbus��Ȧ
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
////		SF0_CS1_DISENABLE;//����ѵڶ���FPGA��Ϊ��չIO
////		SF0_CS1_DISENABLE;
////		SF0_CS4_DISENABLE;
////		SF0_CS2_DISENABLE;//����ֻʹ���˵ڶ���FPGA
////		SF0_CS2_ENABLE;
//		ExIRead(i);
//	}
//}
///**
//  * @brief  CDV Oˢ�¡�
//  *  
//  * @param  ��
//  *   
//  * @retval ��
//  *
//  * @note   ��ȡ����modbus��Ȧ��O��ֵ��ˢ������
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
//  * @brief  CDV ���ˢ�¡�
//  *  
//  * @param  ��
//  *   
//  * @retval ��
//  *
//  * @note   ��ȡ����modbus��Ȧ�ĵ����ֵ��ˢ������
//  */
//void MotorRefresh(void) {
//	CDV_INT08U i;
////	CDV_INT32S pos;
//	//OS_ERR err;
//	
////	if(1 == MODBUS_READ_RETURN) {//�������
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
//	  if(OPT_SUCCESS == MotorChk(i,0)) {//�жϹ����Ƿ����
////		    if(R_MOTOR_POS(i) != R_MOTOR_STEP(i)) {//�ж�Ŀ���ַ�Ƿ��뵱ǰλ��һ��
////				MotorRun(i , R_MOTOR_STEP(i));
////			}
//		}
//	}
//	for( i = 0; i< (CDV_FPGA_NUM<<1); i++) {
//	  if(1 == MODBUS_READ_MOTOR_END(i)) {//�жϹ����Ƿ����
////		    if(R_MOTOR_POS(i) != R_DST_POS(i)) {//�ж�Ŀ���ַ�Ƿ��뵱ǰλ��һ��
//			   // OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_NON_STRICT,&err); //��ʱ	
//				//MotorRun(i , R_DST_POS(i));
//				//OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_NON_STRICT,&err); //��ʱ	
////			}
//		}
//	}
//	
//}
			
///**
//  * @brief  CDV ���Ŀ�경��ˢ�¡�
//  *  
//  * @param  ��
//  *   
//  * @retval ��
//  *
//  * @note   ��ȡ����modbus��Ȧ�ĵ����ֵ��ˢ������
//  */
//void MotorStepRefresh(void) {
//	CDV_INT08U i;
//	CDV_INT32S pos;
//	if(1 == MODBUS_READ_RETURN) {//�������
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
//  * @brief  ����������ʾ
//  *  
//  * @param  valNo     ����ʾ�ı�����
//  *   
//  * @retval ����ֵ˵��
//  *
//  * @note   ��flash�л�ȡ��������Ϣ��������ֵ����ö������֯����
//  */
//void DisplayVar(const CDV_INT08U valNo, const CDV_INT08U row, const CDV_INT08U uartNo) {
//	//CDV_INT08U i = 0;
//	CDV_INT08U *txBuf = NULL;
//	CDV_INT08U len = 5;//, row ;
//	CDV_INT08U no , parNo , type , strLen, parLen;
//	CDV_INT32S num;
//	
//	CDV_INT32U size = 0;//�ű���С������ת
//	CDV_INT32U shift = 0;//ƫ����ʼ��ַ���ֽ���, ��ѯ����ʱ����ʱshift
//	CDV_INT32U strAddr, parAddr;//endAddr = 0, 
//	CDV_INT32U addr;//�õ������ű��ĵ�ַ

//	
//		no = valNo;

//		num = VarGet(no);

//		//ValSet(no , num);
//		//��ȡ����
//		addr = GetResAddr(0x05, no, 0x00, 2);//��ȡno�����ĵ�ַ
//		if(!addr)
//			return;
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, addr, 2);//��ȡ��Դ�ű���С
//	  shift = 2;
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&strLen, addr + shift, 1);//��ȡ��Դ���Ƴ���
//		shift += 1;
//		strAddr = addr + shift;//���Ƶ�ַ����
//		
//		
//		
//		
//		
//		
//		//��ȡ����ֵ
//		addr = GetResAddr(0x05, no, 0x00, 3);//ֵ����
//		if(!addr)
//			return;
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&size, addr, 2);//��ȡ��Դ������С
////		if(size != 1)
////			return;
////		
//		shift = 2;
//		parAddr = addr + shift + 1;//������ַ����
//		parLen = size - 1;//�������ȱ���
//		
//		size = 0;
//		SPI_Flash_Read((CDV_INT08U*)&parNo, addr + shift, 1);//��ȡ��������
//		
//			
//			if(parNo == 0x01) {//��ֵ
//				type = 0xff;
//				len = 8 + strLen;
//			}
//			else if(parNo == 0x00) {//ö��
//				if(parLen%2  || (parLen >>1)>0x20)//ö��ֵ���������
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
//		txBuf[2] = row;//�ڼ�����������
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
  *�Ѽ���U8ת����һ��U32
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

/** @brief  ��Դ����(�ɰ�)���������
  * @param  rxBuf     ԭʼ�ַ���
  *         rxLen     ������
	*         para      ���Ӳ����ַ���
	*         paraLen   ������
  * @retval ��
  * @note   ���շ��͵��ַ�������
	*         ��rxBuf ��ΪNULL
	*       ��  3byte  �Щ���rxLen byte������   paraLen   ��   2byte  ��
	*                                                     ��          ��
	*     rxBuf-6    rxBuf                para            ��    CRC   ��                                    
	*       ��         ��                  ��             ��          ��                                                             
	*                                                                                      
	*         ��rxBuf ΪNULL
	*       ��     paraLen   ��   2byte  ��
	*       ��               ��          ��
	*      para              ��    CRC   ��                                    
	*       ��               ��          ��                                                                                     
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
/** @brief  ��Դ����(�°�)��
  * @param  rxBuf     ԭʼ�ַ���
  *         rxLen     ������
	*         para      ���Ӳ����ַ���
	*         paraLen   ������
  * @retval ��
  * @note   ���շ��͵��ַ�������
	*                 ������rxLen byte������   paraLen   ��   2byte  ��
	*                                                    ��          ��
	*               rxBuf                para            ��    CRC   ��                                    
	*                 ��                  ��             ��          ��                                                                                                                                             
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
/** @brief  ��Դ����(�°�)��
  * @param  rxBuf     ԭʼ�ַ���
  *         rxLen     ������
	*         para      ���Ӳ����ַ���
	*         paraLen   ������
            ctrl      0�� 1 crc
  * @retval ��
  * @note   ���շ��͵��ַ�������
	*                 ������rxLen byte������   paraLen   ��   2byte  ��
	*                                                    ��          ��
	*               rxBuf                para            ��    CRC   ��                                    
	*                 ��                  ��             ��          ��                                                                                                                                             
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
	if(ctrl) // crc �ݶ�
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

/** @brief  �������е�������ӵ����ڡ�
  * @param  
  * @retval ��
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

/** @brief  ���������¼��
  * @param  
  * @retval ��
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


///** @brief  ����д����Դ���á�
//  * @param  
//  * @retval ��
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

/** @brief  ����д����Դ���á�
  * @param  
  * @retval ��
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


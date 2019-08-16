#include "Logic.h"





//LOGIC HeadInfo;
RET_STATUS LogicScript(CDV_INT08U* Rbuf,CDV_INT32U Addr, CDV_INT08U debug){
	CDV_INT08U no , opt , type;
	CDV_INT16U value=0;
	CDV_INT32U FlashAddr=0;
	RET_STATUS ret =OPT_SUCCESS;
	no = Rbuf[0];       //�ű���Դ��:��Ӧ�Ľű���
	opt = Rbuf[1];      //������2	
	type = Rbuf[2];		//������3(����)
	value = Rbuf[3]; 
	FlashAddr = LOGICADDR(no);//��Դ����ʼ��ַ
	switch(opt){

		case 0x00:
			switch(type){

				case 0xfe:
					Log_JudgeByIf(no,value,FlashAddr,debug);
					break;
				default:
					break;	
			}
			break;

		case 0x01:
			switch(type){

				case 0xfe:
					Log_JudgeByNo(no,value,FlashAddr,debug);
					break;
				default:
					break;	
			}
			break;
		case 0x10:
			switch(type){
		
				case 0xfe:
					Log_LoopByIf(no,value,FlashAddr,debug);
					break;
				default:
					break;	
			}
			break;
			
		case 0x11:
			switch(type){
		
				case 0xfe:
					Log_LoopByNo(no,value,FlashAddr,debug);
					break;
				default:
					break;	
			}
			break;

		default:
			break;

	}

	return ret;

}



//���ݷ�֧��ִ�и÷�֧
//ret:0��ʾû������ķ�֧��  ��������ʾҪִ�еķ�֧����ʼ��ַ

/*
  *����         ���ݷ�֧�Ž����ж�                     
  *ResNo            ��Դ��
  *ValueNo         ��ȡ�����õı�����
  *Addr               ����߼���Դ����ʼ��ַ
  *����ֵ    �� 
  *                   
*/

void Log_JudgeByNo(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug){
	
	CDV_INT08U BranshNo,BranshHead[13];
	CDV_INT32U FlashAddr=0,ret,i;
	LOGIC head;
	BranshNo=g_modbusReg.TYPE.var[(ValueNo)].valL;
	
	head = ReadLogHead(ResNo,Addr);        //��ȡ����Դ��ͷ��Ϣ
	FlashAddr = LOGICADDR(ResNo);
	FlashAddr = FlashAddr+head.HeadLen[0];

	for(i=0;i<head.BranshNum[0];i++){
		ret = ReadLogBranshNo(FlashAddr,BranshNo,i,head.BranshNum[0],head.BranshByte);

		if(0 == ret){ 
			continue;
		}
		else{
			SPI_Flash_Read(BranshHead,ret-13,13);
			head.CmdNum[0] = BranshHead[9];
	    	head.FlagBit[0]=BranshHead[12];
			Script_ToRun(ret,head.CmdNum[0],debug);//ִ��
			if(head.FlagBit[0]==1||head.FlagBit[0]==2){
				return;
			}	
		}
	}
	return;

}





/*
  *����         ���ݷ�֧���������ж�                     
  *ResNo            ��Դ��
  *ValueNo         ��ȡ�����õı�����
  *Addr               ����߼���Դ����ʼ��ַ
  *����ֵ    �� 
  *                   
*/

void Log_JudgeByIf(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug){

	CDV_INT32U Condition=0;
	CDV_INT08U BranshHead[13]={0},BranshNum=0,CmdNum=0;
	CDV_INT32U FlashAddr,ret=0,i,Byte[10]={0};
	LOGIC head;
	Condition = g_modbusReg.TYPE.var[(ValueNo)].valL;
    head = ReadLogHead(ResNo,Addr);        //��ȡ����Դ��ͷ��Ϣ
	BranshNum = head.BranshNum[0];
	memcpy(Byte,head.BranshByte,10);
	FlashAddr = LOGICADDR(ResNo);//Addr+ResNo*LogAddOff;//+HeadInfo.HeadLen[0];
	FlashAddr = FlashAddr+head.HeadLen[0];
	for(i=0;i<head.BranshNum[0];i++){
		ret = ReadLogBranshCdt(FlashAddr,Condition,i,BranshNum,Byte);
		if(0 == ret){ 
			continue;
		}
		else{
			SPI_Flash_Read(BranshHead,ret-13,13);
			CmdNum = BranshHead[9];
	    	head.FlagBit[0]=BranshHead[12];
			Script_ToRun(ret,CmdNum,debug);//ִ��
			if(head.FlagBit[0]==1||head.FlagBit[0]==2){
				return;
			}	
		}

	}
	return;
}


/*
  *����         ���ݷ�֧����������ѭ��                     
  *ResNo            ��Դ��
  *ValueNo         ��ȡ�����õı�����
  *Addr               ����߼���Դ����ʼ��ַ
  *����ֵ    �� 
  *                   
*/


void Log_LoopByNo(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug){

	CDV_INT08U BranshNo,BranshHead[13],CmdNum=0;
	CDV_INT32U FlashAddr=0,ret,i,count;
	LOGIC head;
	head = ReadLogHead(ResNo,Addr);							//��ȡ����Դ��ͷ��Ϣ
	FlashAddr = LOGICADDR(ResNo);//Addr+ResNo*LogAddOff;//+HeadInfo.HeadLen[0];//��һ����֧�ĵ�ַ
	FlashAddr = FlashAddr+head.HeadLen[0];
	while(1){
		count = 0;
		BranshNo=g_modbusReg.TYPE.var[(ValueNo)].valL;
		for(i=0;i<head.BranshNum[0];i++){
			ret = ReadLogBranshNo(FlashAddr,BranshNo,i,head.BranshNum[0],head.BranshByte);
			if(0 == ret){ 
				count++;
				continue;
			}
			else{
				SPI_Flash_Read(BranshHead,ret-13,13);
				CmdNum = BranshHead[9];
	    		head.FlagBit[0]=BranshHead[12];
				Script_ToRun(ret,CmdNum,debug);   //ִ��
				if(head.FlagBit[0]==1){ 				  // �����»�ȡ���������ж�
					break;
				}
				else if(head.FlagBit[0]==2){			  //����ִ�������Դ
					return;
				}
			}
		}
		if(count == head.BranshNum[0]) break;
		
	}

	return; 
}




/*
  *����         ���ݷ�֧��������ѭ��                     
  *ResNo            ��Դ��
  *ValueNo         ��ȡ�����õı�����
  *Addr               ����߼���Դ����ʼ��ַ
  *����ֵ    �� 
  *                   
*/
void Log_LoopByIf(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug){

	
	CDV_INT32U Condition;
	CDV_INT32U FlashAddr=0,ret,i,count;
	CDV_INT08U BranshHead[13];
	LOGIC head;
	head = ReadLogHead(ResNo,Addr);							//��ȡ����Դ��ͷ��Ϣ
	FlashAddr = LOGICADDR(ResNo);//Addr+ResNo*LogAddOff;//+HeadInfo.HeadLen[0];//��һ����֧�ĵ�ַ
	FlashAddr = FlashAddr+head.HeadLen[0];
	while(1){
		count = 0;
		Condition=g_modbusReg.TYPE.var[(ValueNo)].valL;
		for(i=0;i<head.BranshNum[0];i++){
			ret = ReadLogBranshCdt(FlashAddr,Condition,i,head.BranshNum[0],head.BranshByte);
			if(0 == ret){ 
				count++;
				continue;
			}
			else{
				SPI_Flash_Read(BranshHead,ret-13,13);
				head.CmdNum[0]= BranshHead[9];
	    	    head.FlagBit[0]=BranshHead[12];
				Script_ToRun(ret,head.CmdNum[0],debug);//ִ��
				if(head.FlagBit[0]==1){ 			   // �����»�ȡ���������ж�
					break;
				}
				else if(head.FlagBit[0]==2){			//����ִ�������Դ
					return;
				}
			}
			
		}
		if(count == head.BranshNum[0]) break;
		
	}
	return; 
}



/*
  *����          ��ȡ����Դ�ĵ�ͷ��Ϣ
  *                      ��֧����ͷ���ȣ���֧�ֽ���
  *ResNo            ��Դ��
  *Count             �ڼ�����֧
  *Addr               ����߼���Դ����ʼ��ַ
  *����ֵ    ��֧���� 
  *                   
*/

LOGIC ReadLogHead(CDV_INT08U ResNo,CDV_INT32U Addr){

	CDV_INT32U FlashAddr,i;
	CDV_INT08U NameLen[1]={0},BranshByte[2]={0};
	LOGIC head;
	FlashAddr = LOGICADDR(ResNo);
	SPI_Flash_Read(NameLen,FlashAddr+1,1);   //��ȡ�߼����Ƴ���
	SPI_Flash_Read(head.BranshNum,FlashAddr+(2+NameLen[0]),1); //��ȡ��֧��
	head.HeadLen[0]=3+NameLen[0];  
	
	FlashAddr = FlashAddr+head.HeadLen[0]+10;
	for(i=0;i<head.BranshNum[0];i++){/*�����з�֧���ֽ���������*/
		SPI_Flash_Read(BranshByte,FlashAddr,2);
		head.BranshByte[i] = CalculateForAll(BranshByte,0,2);
		FlashAddr = FlashAddr+head.BranshByte[i]+13;
	}
	return head;

} 



/*
  *����:        ��ȡ����Դ�ĵڼ�����֧������
  *BranshNo       ��֧��
  *Count            �ڼ�����֧
  *FlashAddr      ��֧����ʼ��ַ(��һ��ִ�����)
  *����ֵ  0:��ʾû���ҵ��÷�֧�� 
  *                    ��0:���������ķ�֧����ִ��������ʼ��ַ
*/

CDV_INT32U ReadLogBranshNo(CDV_INT32U FlashAddr,CDV_INT08U BranshNo,CDV_INT32U Count,CDV_INT08U BranshNum,CDV_INT32U* 	BranshByte){


	CDV_INT32S i;
	CDV_INT08U BranshHead[13]={0},No;
	if(Count>=BranshNum||Count>=10)	return 0;
	for(i=0;i<Count;i++){
		FlashAddr = FlashAddr+13+BranshByte[i];
	}//�õ��˵ڼ�����֧�ĵ�ַ
	SPI_Flash_Read(BranshHead,FlashAddr,13);
	No = BranshHead[0];
	FlashAddr = FlashAddr + 13;//
	
	if(	No == BranshNo){
		return FlashAddr;//(���ص�ַ����������)
	}
	else{
		return 0;
	}

}



/*
  *����:        ��ȡ����Դ�ĵڼ�����֧������
  *BranshCdt       ��֧����
  *Count            �ڼ�����֧
  *FlashAddr      ��֧����ʼ��ַ(��һ��ִ�����ĵ�ַ)
  *����ֵ  0:��ʾû���ҵ��÷�֧�� 
  *                    ��0:���������ķ�֧����ִ��������ʼ��ַ(������������)
*/

CDV_INT32U ReadLogBranshCdt(CDV_INT32U FlashAddr,CDV_INT32U BranshCdt,CDV_INT32U Count,CDV_INT08U BranshNum,CDV_INT32U* BranshByte){


	CDV_INT32S i,CdtMax,CdtMin;
	CDV_INT08U BranshHead[13]={0};
	if(Count>=BranshNum||Count>=10)	return 0;
	for(i=0;i<Count;i++){
		FlashAddr = FlashAddr+13+BranshByte[i];
	}
	SPI_Flash_Read(BranshHead,FlashAddr,13);
	CdtMin = CalculateForAll(BranshHead,1,4);
	CdtMax = CalculateForAll(BranshHead,5,4);
	FlashAddr = FlashAddr + 13;
	if((BranshCdt>=CdtMin&&BranshCdt<CdtMax)||(BranshCdt==CdtMin&&BranshCdt==CdtMax)){

		return FlashAddr;//(���ص�ַ����������)
	}
	else{
		return 0;
	}

}


/*
  *����:        ִ�з��������µĲ������
  *Addr              �÷�֧��һ��������ʼ��ַ
  *CDMNum  	   �÷�֧�µ��ж��������
  *����ֵ   ��
  *                 
*/

void Script_ToRun(CDV_INT32U Addr,CDV_INT08U CmdNum,CDV_INT08U debug){

	CDV_INT32U i,FlashAddr;
	CDV_INT08U *Sbuf=NULL,LeftLen,TempSbuf[30]={0};
	FlashAddr = Addr;
	for(i=0;i<CmdNum;i++){
		SPI_Flash_Read(&LeftLen,FlashAddr+2,1);//��ȡһ�����ĳ���
		//Sbuf = (CDV_INT08U *)malloc(sizeof(CDV_INT08U)*LeftLen);
		SPI_Flash_Read(TempSbuf,FlashAddr+3,LeftLen);
		while (OPT_SUCCESS != CmdParse( TempSbuf , LeftLen , debug));
		//CmdParse( Sbuf , LeftLen , debug);

		if(Sbuf!=NULL){
			free(Sbuf);
			Sbuf = NULL;
		}
		FlashAddr = FlashAddr+3+LeftLen;
	}
	return ;
}





//��ȡ��Ӧ��֧�ŵ���Ϣ
/*CDV_INT32U ReadLogBranshNo(CDV_INT32U FlashAddr,CDV_INT08U BranshNo){

	CDV_INT32S i,j;
	CDV_INT08U BranshHead[10]={0},CmdNum,CmdLen,No,BranshNum;

	BranshNum=HeadInfo.BranshNum[0];   //��֧��
	for(i=0;i<BranshNum;i++){
		SPI_Flash_Read(BranshHead,FlashAddr,10);
		No = BranshHead[0];
		CmdNum = BranshHead[9];
		FlashAddr = FlashAddr + 10;
		if(No == BranshNo) {

			HeadInfo.CmdNum[0] = CmdNum;
			return FlashAddr;//(���ص�ַ����������)
		}
		for(j=0;j<CmdNum;j++){//Ѱ����һ����֧����ʼ��ַ
			SPI_Flash_Read(&CmdLen,FlashAddr+2,1);
			FlashAddr = FlashAddr +CmdLen;
		}
	}

	return 0;
}*/





//�������������ĸ���֧������
/*CDV_INT32U ReadLogBranshCdt(CDV_INT32U FlashAddr,CDV_INT08U BranshCdt){

	CDV_INT32S i,j,CdtMax,CdtMin;
	CDV_INT08U BranshHead[10]={0},BranshNum,CmdNum,CmdLen;

	if(HeadInfo.BranshNum[0]==0){
		BranshNum=1;
	}
	else{
		BranshNum=HeadInfo.BranshNum[0];
	}
	for(i=0;i<BranshNum;i++){
		SPI_Flash_Read(BranshHead,FlashAddr,10);
		CdtMin = CalculateForAll(BranshHead,1,4);
		CdtMax = CalculateForAll(BranshHead,5,4);
		CmdNum = BranshHead[9];
		FlashAddr = FlashAddr + 10;
		if((BranshCdt>=CdtMin&&BranshCdt<CdtMax)||(BranshCdt==CdtMin&&BranshCdt==CdtMax)){

			HeadInfo.CmdNum[0] = CmdNum;
			return FlashAddr;
		}
		for(j=0;j<CmdNum;j++){//Ѱ����һ����֧����ʼ��ַ
			SPI_Flash_Read(&CmdLen,FlashAddr+2,1);
			FlashAddr = FlashAddr +CmdLen;
		}
	}


	return 0;

}*/








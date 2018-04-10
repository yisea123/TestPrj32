#include "Logic.h"





//LOGIC HeadInfo;
RET_STATUS LogicScript(CDV_INT08U* Rbuf,CDV_INT32U Addr, CDV_INT08U debug){
	CDV_INT08U no , opt , type;
	CDV_INT16U value=0;
	CDV_INT32U FlashAddr=0;
	RET_STATUS ret =OPT_SUCCESS;
	no = Rbuf[0];       //脚本资源号:对应的脚本号
	opt = Rbuf[1];      //功能码2	
	type = Rbuf[2];		//功能码3(类型)
	value = Rbuf[3]; 
	FlashAddr = LOGICADDR(no);//资源的起始地址
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



//根据分支号执行该分支
//ret:0表示没有相符的分支号  ，其他表示要执行的分支的起始地址

/*
  *作用         根据分支号进行判断                     
  *ResNo            资源号
  *ValueNo         提取变量用的变量号
  *Addr               存放逻辑资源的起始地址
  *返回值    无 
  *                   
*/

void Log_JudgeByNo(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug){
	
	CDV_INT08U BranshNo,BranshHead[13];
	CDV_INT32U FlashAddr=0,ret,i;
	LOGIC head;
	BranshNo=g_modbusReg.TYPE.var[(ValueNo)].valL;
	
	head = ReadLogHead(ResNo,Addr);        //获取该资源的头信息
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
			Script_ToRun(ret,head.CmdNum[0],debug);//执行
			if(head.FlagBit[0]==1||head.FlagBit[0]==2){
				return;
			}	
		}
	}
	return;

}





/*
  *作用         根据分支条件进行判断                     
  *ResNo            资源号
  *ValueNo         提取变量用的变量号
  *Addr               存放逻辑资源的起始地址
  *返回值    无 
  *                   
*/

void Log_JudgeByIf(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug){

	CDV_INT32U Condition=0;
	CDV_INT08U BranshHead[13]={0},BranshNum=0,CmdNum=0;
	CDV_INT32U FlashAddr,ret=0,i,Byte[10]={0};
	LOGIC head;
	Condition = g_modbusReg.TYPE.var[(ValueNo)].valL;
    head = ReadLogHead(ResNo,Addr);        //获取该资源的头信息
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
			Script_ToRun(ret,CmdNum,debug);//执行
			if(head.FlagBit[0]==1||head.FlagBit[0]==2){
				return;
			}	
		}

	}
	return;
}


/*
  *作用         根据分支号条件进行循环                     
  *ResNo            资源号
  *ValueNo         提取变量用的变量号
  *Addr               存放逻辑资源的起始地址
  *返回值    无 
  *                   
*/


void Log_LoopByNo(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug){

	CDV_INT08U BranshNo,BranshHead[13],CmdNum=0;
	CDV_INT32U FlashAddr=0,ret,i,count;
	LOGIC head;
	head = ReadLogHead(ResNo,Addr);							//获取该资源的头信息
	FlashAddr = LOGICADDR(ResNo);//Addr+ResNo*LogAddOff;//+HeadInfo.HeadLen[0];//第一个分支的地址
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
				Script_ToRun(ret,CmdNum,debug);   //执行
				if(head.FlagBit[0]==1){ 				  // 需重新获取变量再做判断
					break;
				}
				else if(head.FlagBit[0]==2){			  //不再执行这个资源
					return;
				}
			}
		}
		if(count == head.BranshNum[0]) break;
		
	}

	return; 
}




/*
  *作用         根据分支条件进行循环                     
  *ResNo            资源号
  *ValueNo         提取变量用的变量号
  *Addr               存放逻辑资源的起始地址
  *返回值    无 
  *                   
*/
void Log_LoopByIf(CDV_INT08U ResNo,CDV_INT16U ValueNo,CDV_INT32U Addr,CDV_INT08U debug){

	
	CDV_INT32U Condition;
	CDV_INT32U FlashAddr=0,ret,i,count;
	CDV_INT08U BranshHead[13];
	LOGIC head;
	head = ReadLogHead(ResNo,Addr);							//获取该资源的头信息
	FlashAddr = LOGICADDR(ResNo);//Addr+ResNo*LogAddOff;//+HeadInfo.HeadLen[0];//第一个分支的地址
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
				Script_ToRun(ret,head.CmdNum[0],debug);//执行
				if(head.FlagBit[0]==1){ 			   // 需重新获取变量再做判断
					break;
				}
				else if(head.FlagBit[0]==2){			//不再执行这个资源
					return;
				}
			}
			
		}
		if(count == head.BranshNum[0]) break;
		
	}
	return; 
}



/*
  *作用          读取该资源的的头信息
  *                      分支数，头长度，分支字节数
  *ResNo            资源号
  *Count             第几个分支
  *Addr               存放逻辑资源的起始地址
  *返回值    分支个数 
  *                   
*/

LOGIC ReadLogHead(CDV_INT08U ResNo,CDV_INT32U Addr){

	CDV_INT32U FlashAddr,i;
	CDV_INT08U NameLen[1]={0},BranshByte[2]={0};
	LOGIC head;
	FlashAddr = LOGICADDR(ResNo);
	SPI_Flash_Read(NameLen,FlashAddr+1,1);   //读取逻辑名称长度
	SPI_Flash_Read(head.BranshNum,FlashAddr+(2+NameLen[0]),1); //读取分支数
	head.HeadLen[0]=3+NameLen[0];  
	
	FlashAddr = FlashAddr+head.HeadLen[0]+10;
	for(i=0;i<head.BranshNum[0];i++){/*把所有分支的字节数读出来*/
		SPI_Flash_Read(BranshByte,FlashAddr,2);
		head.BranshByte[i] = CalculateForAll(BranshByte,0,2);
		FlashAddr = FlashAddr+head.BranshByte[i]+13;
	}
	return head;

} 



/*
  *作用:        读取该资源的第几个分支的内容
  *BranshNo       分支号
  *Count            第几个分支
  *FlashAddr      分支的起始地址(第一条执行语句)
  *返回值  0:表示没有找到该分支号 
  *                    非0:符合条件的分支，其执行语句的起始地址
*/

CDV_INT32U ReadLogBranshNo(CDV_INT32U FlashAddr,CDV_INT08U BranshNo,CDV_INT32U Count,CDV_INT08U BranshNum,CDV_INT32U* 	BranshByte){


	CDV_INT32S i;
	CDV_INT08U BranshHead[13]={0},No;
	if(Count>=BranshNum||Count>=10)	return 0;
	for(i=0;i<Count;i++){
		FlashAddr = FlashAddr+13+BranshByte[i];
	}//得到了第几条分支的地址
	SPI_Flash_Read(BranshHead,FlashAddr,13);
	No = BranshHead[0];
	FlashAddr = FlashAddr + 13;//
	
	if(	No == BranshNo){
		return FlashAddr;//(返回地址，命令条数)
	}
	else{
		return 0;
	}

}



/*
  *作用:        读取该资源的第几个分支的内容
  *BranshCdt       分支条件
  *Count            第几个分支
  *FlashAddr      分支的起始地址(第一条执行语句的地址)
  *返回值  0:表示没有找到该分支号 
  *                    非0:符合条件的分支，其执行语句的起始地址(包括命令条数)
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

		return FlashAddr;//(返回地址，命令条数)
	}
	else{
		return 0;
	}

}


/*
  *作用:        执行符合条件下的操作语据
  *Addr              该分支第一条语句的起始地址
  *CDMNum  	   该分支下的有多少条语句
  *返回值   无
  *                 
*/

void Script_ToRun(CDV_INT32U Addr,CDV_INT08U CmdNum,CDV_INT08U debug){

	CDV_INT32U i,FlashAddr;
	CDV_INT08U *Sbuf=NULL,LeftLen,TempSbuf[30]={0};
	FlashAddr = Addr;
	for(i=0;i<CmdNum;i++){
		SPI_Flash_Read(&LeftLen,FlashAddr+2,1);//读取一条语句的长度
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





//读取对应分支号的信息
/*CDV_INT32U ReadLogBranshNo(CDV_INT32U FlashAddr,CDV_INT08U BranshNo){

	CDV_INT32S i,j;
	CDV_INT08U BranshHead[10]={0},CmdNum,CmdLen,No,BranshNum;

	BranshNum=HeadInfo.BranshNum[0];   //分支数
	for(i=0;i<BranshNum;i++){
		SPI_Flash_Read(BranshHead,FlashAddr,10);
		No = BranshHead[0];
		CmdNum = BranshHead[9];
		FlashAddr = FlashAddr + 10;
		if(No == BranshNo) {

			HeadInfo.CmdNum[0] = CmdNum;
			return FlashAddr;//(返回地址，命令条数)
		}
		for(j=0;j<CmdNum;j++){//寻找下一个分支的起始地址
			SPI_Flash_Read(&CmdLen,FlashAddr+2,1);
			FlashAddr = FlashAddr +CmdLen;
		}
	}

	return 0;
}*/





//根据条件分析哪个分支的内容
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
		for(j=0;j<CmdNum;j++){//寻找下一个分支的起始地址
			SPI_Flash_Read(&CmdLen,FlashAddr+2,1);
			FlashAddr = FlashAddr +CmdLen;
		}
	}


	return 0;

}*/








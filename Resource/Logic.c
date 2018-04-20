/**
  ******************************************************************************
  * @file    /logic.c 
  * @author  zzy
  * @version V1.0.0
  * @date    2017-3-2
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
	
	#include"logic.h"


RET_STATUS Log_BreakIf(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg);
RET_STATUS Log_ContinueIf(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg);
RET_STATUS Log_If(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg);
RET_STATUS Log_ElseIf(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg);
RET_STATUS Log_Else(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg);
RET_STATUS Log_EndIf(CMD_ARG *arg);
RET_STATUS Log_Loop(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg);
RET_STATUS Log_EndLoop(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg);

/*****************新驱动*******/

CDV_INT08U Log_Relation(CDV_INT08U ret1,CDV_INT08U ret2,CDV_INT08U relation);
CDV_INT08U Log_Compare(CDV_INT32S value,CDV_INT08U compare,CDV_INT32S Num);
CDV_INT32U Result(CDV_INT08U* p, CDV_INT08U ExpLen,CMD_ARG *arg);
//void Log_LoopUp(CDV_INT32U *p);
CDV_INT08U BackResultToKFC(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg);

//void Log_If1(CDV_INT08U *Rbuf,CDV_INT32U *pAddr);
//void Log_Loopxu(CDV_INT32U *pAddr);
//void Log_ContinueXu(CDV_INT32U * paddr,CMD_ARG *arg);
//void Log_LoopEndXu(CDV_INT32U *p);
//void Log_LoopUp(CDV_INT32U *p);



RET_STATUS LogicScript(CDV_INT08U* Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){
	CDV_INT08U opt , type;
	RET_STATUS ret =OPT_SUCCESS;
	//no = Rbuf[0];           //脚本资源号:对应的脚本号
	opt = Rbuf[4];      //功能码2	
	type = Rbuf[5];		//功能码3(类型)
	//value = Rbuf[3]; 
	switch(opt){
		case 0x00:
			switch(type){
				case 0xed:
					ret=Log_If(Rbuf,rxLen,arg);
					break;
				default:
					break;	
			}
			break;
		case 0x01:
			switch(type){
				case 0xed:
					ret=Log_ElseIf(Rbuf,rxLen,arg);
					break;
				default:
					break;	
			}
			break;
		case 0x02:
			switch(type){
				case 0xed:
					ret=Log_EndIf(arg);
					break;
				default:
					break;	
			}
			break;
		case 0x03:
			switch(type){
				case 0xed:
					ret=Log_Else(Rbuf,rxLen,arg);
					break;
				default:
					break;	
			}
			break;
		case 0x10:
			switch(type){
				case 0xed:
					ret=Log_Loop(Rbuf,rxLen,arg);
					break;
				default:
					break;	
			}
			break;
		case 0x11:
			switch(type){
				case 0xed:
					ret=Log_EndLoop(Rbuf,rxLen,arg);
					break;
				default:
					break;	
			}
			break;
		case 0x12:
			switch(type){
				case 0xed:
					//ret=Log_If(Rbuf,*WorkNo,arg);
					ret=Log_BreakIf(Rbuf,rxLen,arg);
					break;
				default:
					break;	
			}
			break;	
		case 0x13:
			switch(type){
				case 0xed:
					//ret=Log_If(Rbuf,*WorkNo,arg);
					ret=Log_ContinueIf(Rbuf,rxLen,arg);
					break;
				default:
					break;	
			}
			break;	
		case 0x20:
			switch(type){
				case 0xed:
					//ret=Log_If(Rbuf,*WorkNo,arg);
					ret= (RET_STATUS)BackResultToKFC(Rbuf,rxLen,arg);
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


RET_STATUS Log_If(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){

	CDV_INT08U ExpLen=0,ResNo=0,Action=0,*Jump = NULL/*[2]={0}*/,*Calcu = NULL/*[100]={0}*/; 
	CDV_INT16U JumpCmd=0;
	CDV_INT32S Times=0;//公式中有多少个条件 
//	CDV_INT32U *paddr=NULL;
	CDV_INT32U Addr=0;
	ExpLen = rxLen - 6 - 2;
	JumpCmd = *(CDV_INT16U*)(Rbuf+6+ExpLen);//CalculateForAll(Rbuf,6+ExpLen,2);
	//Times = ((ExpLen-9)/10+1);
	if(Result(Rbuf+6,ExpLen,arg)){
		return OPT_SUCCESS;
	}
	else{
		ToWorkerCmd(arg->ptrWorker, JumpCmd);//Mem_ToScriptCmd(WorkNo,JumpCmd);
		
		while(1){
	
			Addr = ((DEBUG_SCRIPT*)(arg->ptrWorker))->cmdPos + 2;//Addr = g_run.cmdPos[WorkNo];
			
//#ifdef  _DEBUG_NPC_
//			Mem_Read(&ResNo,Addr + 4,1);
//			Mem_Read(&Action,Addr + 9,1);
//#else
	    ResNo = *(CDV_INT08U*)(SCRIPT_GETADDR(Addr + 4));
	    Action = *(CDV_INT08U*)(SCRIPT_GETADDR(Addr + 9));
//#endif	
			if(ResNo == 0x08 &&((Action == 0x03)||(Action == 0x02))){
				//g_run.cmdPos[WorkNo] = *paddr;
				return OPT_SUCCESS;
			}
//#ifdef  _DEBUG_NPC_
//			Mem_Read(&ExpLen,Addr,1);
//#else
	    ExpLen = *(CDV_INT08U*)(SCRIPT_GETADDR(Addr));
//#endif	
			ExpLen = ExpLen - 12;
			
			//Mem_Read(Jump,Addr+11+ExpLen,2);
			
	    JumpCmd = *(CDV_INT16U*)(SCRIPT_GETADDR(Addr+11+ExpLen));
			
//			Mem_Read_Ptr(&Jump,Addr+11+ExpLen);

//			JumpCmd = CalculateForAll(Jump,0,2);
			ASSERT(JumpCmd < ((DEBUG_SCRIPT*)(arg->ptrWorker))->cmdNum);

			//Mem_Read(Calcu,Addr+11,ExpLen);
			Mem_Read_Ptr(&Calcu,Addr+11);

			if(Result(Calcu,ExpLen,arg)){
				
				return OPT_SUCCESS;
			}
			ToWorkerCmd(arg->ptrWorker, JumpCmd);//Mem_ToScriptCmd(WorkNo,JumpCmd);
		}
		//return OPT_FAILURE;
	}	
}

RET_STATUS Log_ElseIf(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){

	CDV_INT08U ExpLen=0; 
	CDV_INT16U JumpCmd=0;
//	CDV_INT32S Times=0;//公式中有多少个条件 
	ExpLen = rxLen - 6 - 2;
	JumpCmd = *(CDV_INT16U*)(Rbuf+6+ExpLen);//CalculateForAll(Rbuf,6+ExpLen,2);
//	Times = ((ExpLen-6)/7+1);
//	if(Result(Rbuf+4,Times)){
//		return OPT_SUCCESS;
//	}
//	else{
		ToWorkerCmd(arg->ptrWorker, JumpCmd);//Mem_ToScriptCmd(WorkNo,JumpCmd);
		return OPT_FAILURE;
//	}	
}

RET_STATUS Log_Else(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){

	CDV_INT16U JumpCmd=0;	
	JumpCmd = *(CDV_INT16U*)(Rbuf+6);//CalculateForAll(Rbuf,6,2);	
	ToWorkerCmd(arg->ptrWorker, JumpCmd);//Mem_ToScriptCmd(WorkNo,JumpCmd);
	return OPT_FAILURE;
	
}

RET_STATUS Log_EndIf(CMD_ARG *arg){
	return OPT_SUCCESS;
}


RET_STATUS Log_Loop(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){

	CDV_INT08U ExpLen=0; 
	CDV_INT16U JumpCmd=0;
	CDV_INT32S Times=0;//公式中有多少个条件 
	
	ExpLen = rxLen - 6 - 2;
	JumpCmd = *(CDV_INT16U*)(Rbuf+6+ExpLen);//CalculateForAll(Rbuf,6+ExpLen,2);
//	Times = ((ExpLen-9)/10+1);
	if(Result(Rbuf+6,ExpLen,arg)){
		return OPT_SUCCESS;
	}
	else{
		ToWorkerCmd(arg->ptrWorker, JumpCmd);//Mem_ToScriptCmd(WorkNo,JumpCmd);
		return OPT_SUCCESS;
	}	

}

RET_STATUS Log_EndLoop(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){

	CDV_INT16U JumpCmd=0;	
	JumpCmd = *(CDV_INT16U*)(Rbuf+6);//CalculateForAll(Rbuf,6,2);	
	ToWorkerCmd(arg->ptrWorker, JumpCmd);//Mem_ToScriptCmd(WorkNo,JumpCmd);
	return OPT_FAILURE;
	

}



RET_STATUS Log_BreakIf(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){

	CDV_INT08U ExpLen=0; 
	CDV_INT16U JumpCmd=0;
	CDV_INT32S Times=0;//公式中有多少个条件 
	
	ExpLen = rxLen - 6 - 2;
	JumpCmd = *(CDV_INT16U*)(Rbuf+6+ExpLen);//CalculateForAll(Rbuf,6+ExpLen,2);
//	Times = ((ExpLen-9)/10+1);
	if(Result(Rbuf+6,ExpLen,arg)){
		ToWorkerCmd(arg->ptrWorker, JumpCmd);//Mem_ToScriptCmd(WorkNo,JumpCmd);
		return OPT_SUCCESS;
	}
	else{
		
		return OPT_SUCCESS;
	}	
}

RET_STATUS Log_ContinueIf(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){

	CDV_INT08U ExpLen=0;
	CDV_INT16U JumpCmd=0;
	CDV_INT32S Times=0;//公式中有多少个条件
	
	ExpLen = rxLen - 6 - 2;
	JumpCmd = *(CDV_INT16U*)(Rbuf+6+ExpLen);//CalculateForAll(Rbuf,6+ExpLen,2);
//	Times = ((ExpLen-9)/10+1);
	if(Result(Rbuf+6,ExpLen,arg)){
		ToWorkerCmd(arg->ptrWorker, JumpCmd);//Mem_ToScriptCmd(WorkNo,JumpCmd);
		return OPT_FAILURE;
	}
	else{
		return OPT_SUCCESS;
	}
}
/*
 **功能:条件判断的结果
  *
  *
  *
  *返回值:1成立  0不成立
  *warnning: 
*/
CDV_INT32U ExpResult(CDV_INT08U* p,CDV_INT32S len,CMD_ARG *arg) {
	CDV_INT32S ret = ArithmeticEx((char*) p, (short )len, arg);
	
	return ret ? 1 : 0;
}

/*
 **功能:条件判断的结果
  *
  *
  *
  *返回值:1成立  0不成立
  *warnning: 
*/
CDV_INT32U Result(CDV_INT08U* p, CDV_INT08U ExpLen,CMD_ARG *arg){
//	CDV_INT08U compare=0,num[4]={0},relation[10]={0},resultC[10]={0};
//	CDV_INT32U valueno=0;
//	CDV_INT32S value=0, Num=0,i=0;
//	
//	CDV_INT32S Times;
//  Times	= ((ExpLen-9)/10+1);
////所有行预判断
//		for(i=0;i<Times;i++){
////			valueno = *p;
////			compare = *(p+1);
////			memcpy(num,p+2,4);
////			relation[i] = *(p+6);
////			value = VarGet(valueno);
////			Num = CalculateForAll(num,0,4);
////			resultC[i] = Log_Compare(value,compare,Num);
////			p = p+7;
//			memcpy(&valueno,p,4);
//			compare = *(p+4);
//			memcpy(num,p+5,4);
//			relation[i] = *(p+9);
//			value = VarGet(valueno);
//			Num = CalculateForAll(num,0,4);
//			resultC[i] = Log_Compare(value,compare,Num);
//			p = p+10;
//		}
//			//行与行之间的判断
//		for(i=0;i<(Times-1);i++){
//			resultC[0]= Log_Relation(resultC[0],resultC[i+1],relation[i]);
//		}
//		if(resultC[0]==1)return 1;
//		else return 0;
	return ExpResult(p, ExpLen, arg);
}


/*
**功能:比较两个值的关系是否成立
  *
  *
  *
  *返回值:1成立  0不成立
  *warnning: 
*/

CDV_INT08U Log_Compare(CDV_INT32S value,CDV_INT08U compare,CDV_INT32S Num){

	CDV_INT32S OtherV;
	switch(compare){
		case 0xA1:
			if(value == Num)return 1;
			break;
		case 0xA2:
			if(value > Num)return 1;
			break;
		case 0xA3:
			if(value < Num)return 1;
			break;
		case 0xA4:
			if(value >= Num)return 1;
			break;
		case 0xA5:
			if(value <= Num)return 1;
			break;
		case 0xA6:
			if(value != Num)return 1;
			break;
		case 0xB1:
			OtherV = VarGet(Num);
			if(value == OtherV)return 1;
			break;
		case 0xB2:
			OtherV = VarGet(Num);
			if(value > OtherV)return 1;
			break;
		case 0xB3:
			OtherV = VarGet(Num);
			if(value < OtherV)return 1;
			break;
		case 0xB4:
			OtherV = VarGet(Num);
			if(value >= OtherV)return 1;
			break;
		case 0xB5:
			OtherV = VarGet(Num);
			if(value <= OtherV)return 1;
			break;
		case 0xB6:
			OtherV = VarGet(Num);
			if(value != OtherV)return 1;
			break;
		case 0xf1:
			return 1;
		case 0xf2:
			return 0;
		default:
			break;

	}
	return 0;
}


/*
**功能:比较两个关系式  的关系是否成立
  *
  *
  *
  *返回值:1成立  0不成立 2error
  *warnning: 
*/

CDV_INT08U Log_Relation(CDV_INT08U ret1,CDV_INT08U ret2,CDV_INT08U relation){

	switch(relation){
		case 0x90:
			if(ret1 && ret2)return 1;
			else return 0;			
		case 0x91:
			if(ret1 || ret2)return 1;
			else return 0;
		case 0x92:
			if((ret1&&ret2)||(!(ret1||ret2)))return 0;
			else return 1;
		default:
			return 2;
	}

}



/*
**功能:返回公式的计算结果给开发层
  *
  *
  *
  *返回值:1成立  0不成立 2error
  *warnning: 
*/
CDV_INT08U BackResultToKFC(CDV_INT08U *Rbuf, CDV_INT08U rxLen,CMD_ARG *arg){

	CDV_INT08U ExpLen=0; 
	CDV_INT32S Times=0;//公式中有多少个条件 
	CDV_INT08U Ret=0;
	ExpLen = rxLen - 6;
	if ( ExpLen > arg->len && ExpLen > 0)
		return 2;
//	Times = ((ExpLen-9)/10+1);
	Ret = Result(Rbuf+6,ExpLen,arg);
	
	ResRequest(arg->buf, arg->len, &Ret, 1, arg);//ResParaRequest(Rbuf,6,&Ret,1, arg->uart);
	return Ret;

}


//void Log_If(CDV_INT08U ValueNo,CDV_INT32U *paddr,CDV_INT32S Max,CDV_INT32S Min,CDV_INT16U Byte,CMD_ARG *arg){
//	CDV_INT08U ResNo=0,Action=0,max[4]={0},min[4]={0},byte[2]={0},nest=0;
//	CDV_INT32S value=0;
//	value = DW_CDV_VAR(ValueNo);
//	if((value>=(int)Min && value<(int)Max)||(value==Min&&value==Max)){
//		return;
//	}
//	else{
//		//指针偏移+ Byte
//		while(1){
//			*paddr = *paddr+Byte;
//			Mem_Read(&ResNo,*paddr+3,1);
//			Mem_Read(&Action,*paddr+5,1);	
//			if(ResNo == 0x08 && Action == 0x00)nest++;
//			if((ResNo == 0x08 && Action == 0X03 && nest == 0)||(ResNo == 0x08 && Action == 0X02 && nest == 0))break;
//			if(ResNo == 0x08 && Action == 0X02 && nest > 0)nest--;
//			Mem_Read(&ValueNo,*paddr+7,1);
//			Mem_Read(min,*paddr+8,4);
//			Mem_Read(max,*paddr+12,4);
//			Mem_Read(byte,*paddr+16,2);
//			value = DW_CDV_VAR(ValueNo);
//			Min = CalculateForAll(min,0,4);
//			Max = CalculateForAll(max,0,4);
//			Byte = CalculateForAll(byte,0,2);
//			if((value>=Min && value<Max)||(value==Min&&value==Max)){
//				break;
//			}
////			else{
////				*paddr = *paddr+Byte;
////			}
//		}
//		return;
//	}
//}



//void Log_If1(CDV_INT08U *Rbuf,CDV_INT32U *pAddr){

//	CDV_INT08U ResNo=0,Action=0,ExpLen=0,LeftLen=0,nest=0,flag=0; 
//	CDV_INT32S Times=0;//公式中有多少个条件 
//	CDV_INT32U *p = NULL,*p1 = NULL;
//	p = pAddr;
//	while(1){
//		Mem_Read(&LeftLen,*p +2,1);
//		Mem_Read(&ResNo,*p+3,1);
//		Mem_Read(&Action,*p+5,1);
//		Mem_Read(&ExpLen,*p +7,1);
//		if(ResNo != 0x08){
//			p = p+(LeftLen+3);
//			continue;
//		}
//		if(ResNo == 0x08 && Action == 0x00&& flag == 1){
//			nest++;
//			p = p+(LeftLen+3);
//			continue;
//		}
//		if((ResNo == 0x08 && Action == 0X03 && nest == 0)
//							||(ResNo == 0x08 && Action == 0X02 && nest == 0)){
//			pAddr = p;
//			break;
//		}
//		if(ResNo == 0x08 && Action == 0X02 && nest > 0){
//			nest--;
//			p = p+(LeftLen+3);
//			continue;
//		}
//		
//		Times = ((ExpLen-6)/7+1);
//		p1 = p+8;
//		if(Result(p1,Times)){
//			pAddr = p;
//			break;
//		}
//		else{
//			flag=1;//第二次找到IF的时候有起作用
//			p = p+(LeftLen+3);
//			continue;
//		}
//	}
//	
//}





//void Log_ElseIf(CDV_INT32U *paddr,CMD_ARG *arg){
//	CDV_INT08U ResNo=0,Action=0,nest=0,LeftLen=0;
//	while(1){
//		Mem_Read(&LeftLen,*paddr+2,1);
//		Mem_Read(&ResNo,*paddr+3,1);
//		Mem_Read(&Action,*paddr+5,1);
//		if(ResNo == 0x08 && Action == 0x00)nest++;
//		if(ResNo == 0x08 && Action == 0x02){
//			if(nest==0)break;
//			else nest--;
//		}
//		*paddr = *paddr+LeftLen+3;
//	}
//	return;
//}

//void Log_Else(CDV_INT32U *paddr,CMD_ARG *arg){
//	CDV_INT08U ResNo=0,Action=0,nest=0,LeftLen=0;

//	while(1){
//		Mem_Read(&LeftLen,*paddr+2,1);
//		Mem_Read(&ResNo,*paddr+3,1);
//		Mem_Read(&Action,*paddr+5,1);
//		if(ResNo == 0x08 && Action == 0x00)nest++;
//		if(ResNo == 0x08 && Action == 0x02){
//			if(nest==0)break;
//			else nest--;
//		}
//		*paddr = *paddr+LeftLen+3;
//	}
//	return;
//}

//void Log_EndIf(CMD_ARG *arg){
//	return;
//}





//void Log_Break(CDV_INT32U *paddr,CMD_ARG *arg){
//	//把指针指到循环的末尾(endif)

//	CDV_INT08U ResNo=0,Action=0,LeftLen=0,nest=0;
//	while(1){
//		Mem_Read(&LeftLen,*paddr+2,1);
//		Mem_Read(&ResNo,*paddr+3,1);
//		Mem_Read(&Action,*paddr+5,1);
//		if(ResNo == 0x08 && Action == 0X10) nest++;
//		if(ResNo == 0x08 && Action == 0X11){
//			if(nest==0)break;
//			else nest--;
//		} 
//			*paddr = *paddr+LeftLen+3;
//	}
//	return;
//}

//void Log_Continue(CDV_INT32U *paddr,CMD_ARG *arg){
//	CDV_INT08U ResNo=0,Action=0,byte[2]={0},LeftLen=0,nest=0;
//	CDV_INT16U Byte=0;
//	while(1){
//		Mem_Read(&LeftLen,*paddr+2,1);
//		Mem_Read(&ResNo,*paddr+3,1);
//		Mem_Read(&Action,*paddr+5,1);
//		if(ResNo == 0x08 && Action == 0X10) nest++;
//		if(ResNo == 0x08 && Action == 0X11){
//			if(nest==0){
//				Mem_Read(byte,*paddr+16,2);
//				Byte = CalculateForAll(byte,0,2);
//				*paddr = *paddr-Byte;//这里有问题
//				break;
//			}
//			else nest--;
//		} 
//			*paddr = *paddr+LeftLen+3;
//	}
//	return;
//}


//void Log_ContinueXu(CDV_INT32U * paddr,CMD_ARG *arg){

//	Log_LoopUp(paddr);
//	return ;
//}


////void Log_Loop(CDV_INT08U ValueNo,CDV_INT32U *paddr,CDV_INT32S Max,CDV_INT32S Min,CDV_INT16U Byte,CMD_ARG *arg){
////	CDV_INT32S value=0;
////	value = DW_CDV_VAR(ValueNo);

////	if((value<Min && value>Max)||(value==Min&&value==Max)){
////		return;
////	}
////	else{
////		//指针偏移+ Byte
////		*paddr = *paddr+Byte;
////		return;
////	}
//}
////void Log_Loop1(CDV_INT08U ValueNo,CDV_INT32U *paddr,CDV_INT32S Max,CDV_INT32S Min,CDV_INT16U Byte,CMD_ARG *arg){
////	CDV_INT08U ResNo=0,Action=0,LeftLen=0,nest=0;
////	CDV_INT32S value=0;
////	value = DW_CDV_VAR(ValueNo);
////	if((value<=(int)Min && value>(int)Max)||(value==Min&&value==Max)){
////		return;
////	}
////	Mem_Read(&LeftLen,*paddr+2,1);
////	*paddr = *paddr+LeftLen+3;
////	while(1){
////		Mem_Read(&LeftLen,*paddr+2,1);
////		Mem_Read(&ResNo,*paddr+3,1);
////		Mem_Read(&Action,*paddr+5,1);
////		if(ResNo == 0x08 && Action == 0X10) nest++;
////		if(ResNo == 0x08 && Action == 0X11){
////			if(nest==0)break;
////			else nest--;
////		} 
////			*paddr = *paddr+LeftLen+3;
////	}
////	return;

////}

//void Log_Loopxu(CDV_INT32U *pAddr){
//	CDV_INT08U ResNo=0,Action=0,ExpLen=0,LeftLen=0,nest=0; 
//	CDV_INT32S Times=0;
//	CDV_INT32U *p = NULL,*p1 = NULL;
//	p = pAddr;

//	Mem_Read(&LeftLen,*p +2,1);
//	Mem_Read(&ExpLen,*p +7,1);
//	Times = ((ExpLen-6)/7+1);
//	p1 = p+8;
//	if(Result(p1,Times)){
//		pAddr = p;
//	}
//	else{
//		p = p+LeftLen+3;
//		while(1){
//			Mem_Read(&LeftLen,*p+2,1);
//			Mem_Read(&ResNo,*p+3,1);
//			Mem_Read(&Action,*p+5,1);
//			if(ResNo == 0x08 && Action == 0X10) nest++;
//			if(ResNo == 0x08 && Action == 0X11){
//				if(nest==0){
//					pAddr = p; 					
//					break;
//				}
//				else nest--;
//			} 
//				p = p+LeftLen+3;
//		}

//	}

//}


//void Log_LoopEnd(CDV_INT08U ValueNo,CDV_INT32U *paddr,CDV_INT32S Max,CDV_INT32S Min,CDV_INT16U Byte,CMD_ARG *arg){
//	CDV_INT32S value=0;
//	value = DW_CDV_VAR(ValueNo);
//	if((value<=(int)Min && value>(int)Max)||(value==Min&&value==Max)){
//		//指针偏移- Byte
//		*paddr = *paddr-Byte;
//		return;
//	}
//	else{
//		//*paddr = *paddr-Byte;
//		return;
//	}
//}


//void Log_LoopEndXu(CDV_INT32U *p){
//	CDV_INT08U EXLeftLen=0;//上一条剩余字节
//	Mem_Read(&EXLeftLen,*p+1,1);
//	p = p-EXLeftLen-1;
//	Log_LoopUp(p);

//}





///*
//**功能:往上遍历
//  *EXLeftLen : 上一条剩余字节
//  *ResNo :资源号
//  *Action :操作
//  *nest : 找到endwhile的次数
//  *warnning: 使用这个功能函数的时候，指针往往指在endwhile,需要使用者把指针指向当前条的上一条
//  */
//void Log_LoopUp(CDV_INT32U *p){

//	CDV_INT08U EXLeftLen=0;//上一条剩余字节
//	CDV_INT08U ResNo=0,Action=0,nest=0; 
//	while(1){
//		Mem_Read(&EXLeftLen,*p+1,1);
//		Mem_Read(&ResNo,*p+3,1);
//		Mem_Read(&Action,*p+5,1);
//		if(ResNo == 0x08 && Action == 0X11)nest++;
//		if(ResNo == 0x08 && Action == 0X10){
//			if(nest==0){
//				break;
//			}
//			else nest--;
//		}
//		p = p-EXLeftLen-1;
//	}
//}
















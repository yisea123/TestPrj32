#include"Tray.h"

#if ENABLE_FPGA_DOWN
TrayData job1;//料盘脚本信息存放的结构体
CDV_INT08U ExAxisNum[10];//轴数量
CDV_INT32U Point[10][3]={0};//点的坐标
CDV_INT32U ExGonggeNum[10]; //对应资源号的宫格总数
CDV_INT32U StartPoint[10]={0};//第几个宫格为初始点坐标
CDV_INT08U AxisFlag[10][3]={0},ValueFlag[7],ReadFlag[7];//被覆盖的数据为电机号
CDV_INT32U AddrBuf[10]={SCRIP_TRAY,SCRIP_TRAY+ONETRAY,SCRIP_TRAY+2*ONETRAY,SCRIP_TRAY+3*ONETRAY,SCRIP_TRAY+4*ONETRAY,SCRIP_TRAY+5*ONETRAY,SCRIP_TRAY+6*ONETRAY,SCRIP_TRAY+7*ONETRAY,SCRIP_TRAY+8*ONETRAY,SCRIP_TRAY+9*ONETRAY};

//CDV_INT32U AddrBegin=0;
CDV_INT08U Flag[10]={0};
/*
  *
  * 料盘资源
  * Rbuf:操作脚本地址
  * Addr:flash脚本读取地址
  */

RET_STATUS TrayScript(CDV_INT08U* Rbuf,CDV_INT32U Addr, CDV_INT08U debug,CDV_INT08U uartNo){
	CDV_INT08U no , opt , type;
	CDV_INT32U condition=0;
	CDV_INT16U value=0;
	RET_STATUS ret =OPT_SUCCESS;
	OS_ERR err;

	no = Rbuf[0];       //脚本资源号:对应的脚本号
	opt = Rbuf[1];      //功能码2	
	type = Rbuf[2];		//功能码3(类型)
	switch(opt){
		case 0x00:         //移动
			switch(type){
				case 0xFF://判断数值，找到坐标	
					//TODO:获取操作脚本的条件and FLASH读取地址
				  
					condition=CalculateForAll(Rbuf,3,4);
					TrayHandle(no,condition,Addr, debug);
					break;		
				case 0xFE://判断变量，找到坐标
					//TODO:获取操作脚本的变量代号and FLASH读取地址
					value=CalculateForAll(Rbuf,3,2);
					ValueHandle(no,value,Addr, debug);
					break;
				default:
					break;
			}
			break;
			
		case 0x01:        //按宫格号移动
			switch(type){
				case 0xFF:
					condition=CalculateForAll(Rbuf,3,4);//减1
					TrayGoToPoint(no,condition,Addr,debug);
					break;
				case 0xFE:
					value=CalculateForAll(Rbuf,3,2);
					TrayGoToPointV(no,condition,Addr, debug);
					break;
				default:
					break;					
			}
			break;
			
		case 0x02:	      //当前点移动
			switch(type){
				case 0xED:
					TrayGoCurrPoint(no,debug);
					break;
				default:
					break;					
			}
			break;
			
		case 0x03:		  //当前点的下一个点移动
			switch(type){
				case 0xED:
					TrayGoNextPoint(no,AddrBuf[no], debug);
					//MotorToRun(AxisNum,AddrBuf+8,debug);
					break;
				default:
					break;					
			}
			break;
			
		case 0x04:	      //等待
			switch(type){
				case 0xED:
					ret = TrayWait(no,debug);
					break;
				default:
					break;					
			}		
			break;

		case 0x05:	      //初始点
			switch(type){
				case 0xED:
					//ret = TrayWait(no,debug);
					TrayGoStartPoint(no,Addr,debug);
					break;
				default:
					break;					
			}		
			break;			
		case 0x06:			//当前点的上一点
			switch(type){
				case 0xED:
					TrayGoLastPoint(no,AddrBuf[no], debug);
//					MotorToRun(AxisNum,AddrBuf+8,debug);
					break;
				default:
					break;					
				}	
			break;
				
		case 0x07:			//宫格参数设置

				//ret = TrayWait(debug);
				Tray_Wirte(Rbuf, Addr);
			break;
				
		case 0x08:			//宫格参数读取
					//ret = TrayWait(debug);
				Tray_Read(Rbuf, uartNo);
				break;	

		case 0x09:			//料盘参数设置
					//ret = TrayWait(debug);
				HeadInfo_Wirte(Rbuf);
				break;
				
		case 0x0A:			//料盘参数读取

				HeadInfo_Read(Rbuf, uartNo);	
				break;	
				
		default:
			break;
	}

	return ret;

}

/*
 *料盘的初始化
 *
 *
 */
RET_STATUS Tray_Init(void){
	int i;
	memset(Flag,0,10);
	for(i=0;i<10;i++){
		if(Flag[i]!=0)
			return OPT_FAILURE;
	}
	return OPT_SUCCESS;
}


/*
  *
  *从FLASH里读取脚本
  *Addr:	FLASH中的脚本起始地址
  *
  */

void GetScript(CDV_INT32U Addr)
{

	SPI_Flash_Read(job1.TrayNo,Addr,1);
	SPI_Flash_Read(job1.LatticeNo,Addr+1,2);
	SPI_Flash_Read(job1.ValueNum,Addr+3,2);
	SPI_Flash_Read(job1.Axis,Addr+5,1);
	
	switch(AxisHandle(job1.TrayNo[0],job1.Axis[0])){
		case 1:
			SPI_Flash_Read(job1.X,Addr+6,4);
			SPI_Flash_Read(job1.ConditionMin,Addr+10,4);
			SPI_Flash_Read(job1.ConditionMax,Addr+14,4);
		  break;
		case 2:
			SPI_Flash_Read(job1.X,Addr+6,4);
			SPI_Flash_Read(job1.Y,Addr+10,4);
			SPI_Flash_Read(job1.ConditionMin,Addr+14,4);
			SPI_Flash_Read(job1.ConditionMax,Addr+18,4);
			break;
		case 3:
			SPI_Flash_Read(job1.X,Addr+6,4);
			SPI_Flash_Read(job1.Y,Addr+10,4);
			SPI_Flash_Read(job1.Z,Addr+14,4);
			SPI_Flash_Read(job1.ConditionMin,Addr+18,4);
			SPI_Flash_Read(job1.ConditionMax,Addr+22,4);
		  break;
	}
  return;
}



/**************************************************
  *                                                                                                
  *作用:  提取对应宫格的坐标(根据数值)
  *TrayNo:料盘号									       
  *condition:数值条件
  *Addr:料盘脚本的起始地址
  *
  *************************************************/



void TrayHandle(CDV_INT08U TrayNo, CDV_INT32U Condition,CDV_INT32U Addr, CDV_INT08U debug)
{

	CDV_INT32U sum=0;
	CDV_INT32S CondiMin=0,CondiMax=0;
	CDV_INT08S AxisNum;
	GetScript(Addr);
	Addr = Addr+TrayNo*26*TRAYNUM;
	while(1){
		GetScript(Addr);
		CondiMin = CalculateForAll(job1.ConditionMin,0,4);
		CondiMax = CalculateForAll(job1.ConditionMax,0,4);
		if(Condition>=CondiMin&&Condition<=CondiMax)
		 {
			break;
		 }
		sum++;
		if(sum>999){

			return ;//没有找到对应条件的宫格
		}
		Addr=Addr+26;
	}
	AxisNum = AxisHandle(TrayNo,job1.Axis[0]);
	ExAxisNum[TrayNo] = AxisNum;
	AddrBuf[TrayNo]= Addr;
	//Addr=Addr+8;
	//AddrBuf=Addr;
	MotorToRun(TrayNo,AxisNum,debug);
	return;
}




/*********************************************
  *
  *提取对应宫格的坐标(根据变量)
  *TrayNo:料盘号
  *ValueNo:变量代号
  *Addr:从FLASH读取料盘脚本的起始地址
  *
  *********************************************/

void ValueHandle(CDV_INT08U TrayNo,CDV_INT16U ValueNo,CDV_INT32U Addr, CDV_INT08U debug)
{

	CDV_INT16U Value=0;
	CDV_INT32U sum=0;
	CDV_INT32S CondiMin=0,CondiMax=0;
	CDV_INT08S AxisNum;
	Value = VarGet(ValueNo);//g_modbusReg.TYPE.var[(ValueNo)].valL;
	//AddrBegin=Addr;
	Addr = Addr+TrayNo*26*TRAYNUM;
	while(1){
		GetScript(Addr);
		CondiMin = CalculateForAll(job1.ConditionMin,0,4);
		CondiMax = CalculateForAll(job1.ConditionMax,0,4);
		if(Value>=CondiMin&&Value<=CondiMax)
		 {
			break;
		 }
		sum++;
		if(sum>999){

			return ;//没有找到对应条件的宫格
		}
		Addr=Addr+26;
	}
	AxisNum=AxisHandle(TrayNo,job1.Axis[0]);
	ExAxisNum[TrayNo] = AxisNum;
	AddrBuf[TrayNo]=Addr;
	//AddrBuf=Addr;
	MotorToRun(TrayNo,AxisNum,debug);
	return;
}



void TrayGoToPoint(CDV_INT08U TrayNo,CDV_INT32U GongGeNo,CDV_INT32U Addr, CDV_INT08U debug)
{


	CDV_INT08U AxisNum,TempBuf[2];
	GongGeNo = GongGeNo-1;//宫格号 上面是从1开始
	SPI_Flash_Read(TempBuf,SCRIP_GONGGE+2*TrayNo,2);
	ExGonggeNum[TrayNo]=CalculateForAll(TempBuf,0,2);
	if(GongGeNo>ExGonggeNum[TrayNo]){
		GongGeNo=0;//暂时这么处理
	}
	Addr = Addr+TrayNo*26*TRAYNUM+GongGeNo*26;
	GetScript(Addr);
	AxisNum=AxisHandle(TrayNo,job1.Axis[0]);
	ExAxisNum[TrayNo]= AxisNum;
	AddrBuf[TrayNo]=Addr;
	MotorToRun(TrayNo,AxisNum,debug);
	return;
}


void TrayGoToPointV(CDV_INT08U TrayNo,CDV_INT16U ValueNo,CDV_INT32U Addr, CDV_INT08U debug)
{

	CDV_INT08U AxisNum,TempBuf[2];
	CDV_INT16U GongGeNo=VarGet(ValueNo)-1;//g_modbusReg.TYPE.var[(ValueNo)].valL-1;//减1
	SPI_Flash_Read(TempBuf,SCRIP_GONGGE+2*TrayNo,2);
	ExGonggeNum[TrayNo]=CalculateForAll(TempBuf,0,2);
	if(GongGeNo>ExGonggeNum[TrayNo]){
		GongGeNo=0;//暂时这么处理
	}
	Addr = Addr+TrayNo*26*TRAYNUM+GongGeNo*26;
	GetScript(Addr);
	AxisNum=AxisHandle(TrayNo,job1.Axis[0]);
	ExAxisNum[TrayNo] = AxisNum;//全局的轴数量，用于走到当前点
	AddrBuf[TrayNo]=Addr;
	MotorToRun(TrayNo,AxisNum,debug);
	return;
}

void TrayGoCurrPoint(CDV_INT08U TrayNo,CDV_INT08U debug)
{
	CDV_INT08U AxisNum;
	AxisNum=ExAxisNum[TrayNo];
	if(Flag[TrayNo]==0){

		TrayGoToPoint(TrayNo,0,AddrBuf[TrayNo]+26,debug);
		return;

	}
	if(ExAxisNum[TrayNo]==0)return;
	
	switch(ExAxisNum[TrayNo]){
		
		case 0x01:
			if(1==debug){
				VirtualMotorRun(AxisFlag[TrayNo][0],Point[TrayNo][0]);
			}
			else{
				ActualMotorRun(AxisFlag[TrayNo][0],Point[TrayNo][0]);
	        }
			break;
		case 0x02:
			if(1==debug){
				VirtualMotorRun(AxisFlag[TrayNo][AxisNum-1],Point[TrayNo][0]);
				VirtualMotorRun(AxisFlag[TrayNo][AxisNum-2],Point[TrayNo][1]);
			}
			else{
				ActualMotorRun(AxisFlag[TrayNo][AxisNum-1],Point[TrayNo][0]);
				ActualMotorRun(AxisFlag[TrayNo][AxisNum-2],Point[TrayNo][1]);
	        }
			break;
		case 0x03:
			if(1==debug){
				VirtualMotorRun(AxisFlag[TrayNo][AxisNum-1],Point[TrayNo][0]);
				VirtualMotorRun(AxisFlag[TrayNo][AxisNum-2],Point[TrayNo][1]);
				VirtualMotorRun(AxisFlag[TrayNo][AxisNum-3],Point[TrayNo][2]);
			}
			else{
				ActualMotorRun(AxisFlag[TrayNo][AxisNum-1],Point[TrayNo][0]);
				ActualMotorRun(AxisFlag[TrayNo][AxisNum-2],Point[TrayNo][1]);
				ActualMotorRun(AxisFlag[TrayNo][AxisNum-3],Point[TrayNo][2]);
	        }

			break;
		default:
			break;
	}
	return;
}


void TrayGoNextPoint(CDV_INT08U TrayNo,CDV_INT32U Addr, CDV_INT08U debug)
{
	CDV_INT08U AxisNum,TempBuf[2];
	SPI_Flash_Read(TempBuf,SCRIP_GONGGE+2*TrayNo,2);
	ExGonggeNum[TrayNo]=CalculateForAll(TempBuf,0,2);
	if(Flag[TrayNo] == 0||Addr>=((ExGonggeNum[TrayNo]-1)*26+SCRIP_TRAY+26*TrayNo*TRAYNUM)){
		Addr = SCRIP_TRAY+TrayNo*26*TRAYNUM;
		GetScript(Addr);
		AxisNum = AxisHandle(TrayNo,job1.Axis[0]);
		ExAxisNum[TrayNo]=AxisNum;
		AddrBuf[TrayNo]= Addr;
		MotorToRun(TrayNo,AxisNum,debug);	
		return;
	}
	//Addr = Addr+26;
	AddrBuf[TrayNo]= Addr+26;
	GetScript(AddrBuf[TrayNo]);
	AxisNum = AxisHandle(TrayNo,job1.Axis[0]);
	ExAxisNum[TrayNo]=AxisNum;
	MotorToRun(TrayNo,AxisNum,debug);
	return;

}

void TrayGoLastPoint(CDV_INT08U TrayNo,CDV_INT32U Addr, CDV_INT08U debug){
	CDV_INT08U AxisNum;
	if(Flag[TrayNo]==0||Addr<=(SCRIP_TRAY+26*TrayNo*TRAYNUM)){//这里可以合并
		
		Addr = SCRIP_TRAY+TrayNo*26*TRAYNUM;
		
	}
	else{
	
		Addr = Addr-26;
		
	}
	GetScript(Addr);
	AxisNum = AxisHandle(TrayNo,job1.Axis[0]);
	ExAxisNum[TrayNo] = AxisNum;
	AddrBuf[TrayNo] = Addr;
	MotorToRun(TrayNo,AxisNum,debug);
	return;
	
}


void TrayGoStartPoint(CDV_INT08U TrayNo,CDV_INT32U Addr,CDV_INT08U debug){
	
	CDV_INT08U AxisNum,TempBuf[2];
	SPI_Flash_Read(TempBuf,SCRIP_GONGGE+20+2*TrayNo,2);
	StartPoint[TrayNo]=CalculateForAll(TempBuf,0,2);
	Addr = Addr+TrayNo*TRAYNUM*26+StartPoint[TrayNo]*26;
	GetScript(Addr);
	AxisNum = AxisHandle(TrayNo,job1.Axis[0]);
	ExAxisNum[TrayNo] = AxisNum;
	AddrBuf[TrayNo] = Addr;
	MotorToRun(TrayNo,AxisNum,debug);
	return;
}


RET_STATUS TrayWait(CDV_INT08U TrayNo,CDV_INT08U debug ) {
	CDV_INT08U i,AxisNum;
	AxisNum= ExAxisNum[TrayNo] ;
	
//	if(debug) {
//		;
//	} else {
		for( i = 0 ; i < AxisNum ; i++) {
			if(OPT_FAILURE == MotorChk(AxisFlag[TrayNo][AxisNum-1-i] , 0)) {
				return OPT_FAILURE;
			}
		}
//	}
	
	return OPT_SUCCESS;
}


/*
  *
  *找出有几个要控制的轴
  *轴0-7
  *
  */

CDV_INT32S AxisHandle(CDV_INT08U TrayNo,CDV_INT08U Axis)//获取轴信息(当前结构体中的信息)
{
	CDV_INT32S AxisNum=0,i=0;
	CDV_INT08U TempNum=0;
	for(i=0;i<8;i++){
		TempNum=(Axis>>(7-i))<<7;
		if(128==TempNum){
			AxisFlag[TrayNo][AxisNum]=7-i;
			AxisNum++;
		}
	}
	return AxisNum;	
}
CDV_INT32S AxisRead(CDV_INT08U Axis)//获取轴信息(当前结构体中的信息)
{
	CDV_INT32S AxisNum=0,i=0;
	CDV_INT08U TempNum=0;
	for(i=0;i<8;i++){
		TempNum=(Axis>>(7-i))<<7;
		if(128==TempNum){
			ReadFlag[AxisNum]=7-i;
			AxisNum++;
		}
	}
	return AxisNum;	
}



CDV_INT32S ValueSum(CDV_INT16U ValueNum)
{
	CDV_INT32S AxisNum=0,i=0;
	CDV_INT16U TempNum=0;
	for(i=0;i<16;i++){
		TempNum=(ValueNum>>(15-i))<<15;
		if(32768==TempNum){
			ValueFlag[AxisNum]=15-i;
			AxisNum++;
		}
	}
	return AxisNum;	
}



	
///*
//  *
//  *把几个U8转换成一个U32
//  *
//  *
//  */

//CDV_INT32S CalculateForAll(CDV_INT08U* buf,CDV_INT32S addr,CDV_INT32S lenth)
//{
//	CDV_INT32S i,x=0;
//	for(i=0;i<lenth;i++)
//	{
//		x|=buf[addr++]<<8*((lenth-1)-i);
//	}
//	return x;
//}



void MotorToRun(CDV_INT08U TracyNo,CDV_INT08U AxisNum,CDV_INT08U debug)
{
	Flag[TracyNo]=1;
	switch(AxisNum){

		case 1:
			Point[TracyNo][0]=CalculateForAll(job1.X,0,4);
			if(1 == debug){
				VirtualMotorRun(AxisFlag[TracyNo][0],CalculateForAll(job1.X,0,4));
			}
			else {
				ActualMotorRun(AxisFlag[TracyNo][0],CalculateForAll(job1.X,0,4));
			}
			//一个轴运动

			break;
		case 2:					
			Point[TracyNo][0]=CalculateForAll(job1.X,0,4);
			Point[TracyNo][1]=CalculateForAll(job1.Y,0,4);
			if(1 == debug) {
				VirtualMotorRun(AxisFlag[TracyNo][AxisNum-1],CalculateForAll(job1.X,0,4));
				VirtualMotorRun(AxisFlag[TracyNo][AxisNum-2],CalculateForAll(job1.Y,0,4));
			} 
			else {
				ActualMotorRun(AxisFlag[TracyNo][AxisNum-1],CalculateForAll(job1.X,0,4));
				ActualMotorRun(AxisFlag[TracyNo][AxisNum-2],CalculateForAll(job1.Y,0,4));
			}
			//两个轴运动到指定位置

			break;
		case 3:
			Point[TracyNo][0]=CalculateForAll(job1.X,0,4);
			Point[TracyNo][1]=CalculateForAll(job1.Y,0,4);
			Point[TracyNo][2]=CalculateForAll(job1.Z,0,4);	
			if(1 == debug) {
        VirtualMotorRun(AxisFlag[TracyNo][AxisNum-1],CalculateForAll(job1.X,0,4));
				VirtualMotorRun(AxisFlag[TracyNo][AxisNum-2],CalculateForAll(job1.Y,0,4));
				VirtualMotorRun(AxisFlag[TracyNo][AxisNum-3],CalculateForAll(job1.Z,0,4));
			} 
			else {
				ActualMotorRun(AxisFlag[TracyNo][AxisNum-1],CalculateForAll(job1.X,0,4));
				ActualMotorRun(AxisFlag[TracyNo][AxisNum-2],CalculateForAll(job1.Y,0,4));
				ActualMotorRun(AxisFlag[TracyNo][AxisNum-3],CalculateForAll(job1.Z,0,4));
			}
			break;
		default:
			break;		
	}	
	return;
	
}

void Tray_Wirte(CDV_INT08U *buf,CDV_INT32U Addr){
	CDV_INT08U ResNo;
	CDV_INT16U GongGeNo;
	CDV_INT32U AixsNum,Write_Addr;//ValueNum,;
	CDV_INT08U *WriteBuf=NULL;
	ResNo = buf[0];
	GongGeNo = CalculateForAll(buf,2,2);//减1
	//GongGeNo = GongGeNo-1;//宫格号 上面是从1开始
	if(GongGeNo>999){
		return;
	}
	//ValueNum = ValueSum(CalculateForAll(buf,4,2));  //有几个变量
	AixsNum = AxisHandle(ResNo,buf[6]);//有几个轴
	ExAxisNum[ResNo]=AixsNum;
	WriteBuf = (CDV_INT08U *)malloc(sizeof(CDV_INT08U)*(6+AixsNum*4+8));
	Write_Addr = Addr+ResNo*26*TRAYNUM+GongGeNo*26;
	WriteBuf[0]=buf[0];
	memcpy(WriteBuf+1, buf+2,5+AixsNum*4+8);
	SPI_Flash_Write(WriteBuf,Write_Addr,6+AixsNum*4+8);
	if(WriteBuf!=NULL){
		free(WriteBuf);
		WriteBuf=NULL;
	}
	return;

}

void Tray_Read(CDV_INT08U *buf,CDV_INT08U uartNo){
	CDV_INT08U ResNo,*ReadBuf=NULL;
	CDV_INT16U GongGeNo,ValueNum,AxisNum,valueflag;
	CDV_INT32U i,Addr;
	ResNo = buf[0];
	GongGeNo = CalculateForAll(buf,2,2);//减1
	GongGeNo = GongGeNo-1;//宫格号 上面是从1开始
	Addr = SCRIP_TRAY+ResNo*26*TRAYNUM+GongGeNo*26;
	valueflag = CalculateForAll(buf,4,2);
	ValueNum=ValueSum(valueflag);
	SPI_Flash_Read(buf+6,Addr+5,1);
	AxisNum = AxisRead(buf[6]);
	ReadBuf = (CDV_INT08U *)malloc(sizeof(CDV_INT08U)*(AxisNum*4+(ValueNum-1)*4)+1);
	SPI_Flash_Read(ReadBuf,Addr+5,1);
	if(ValueFlag[ValueNum-1]==0){
		for(i=0;i<AxisNum;i++){
			SPI_Flash_Read(ReadBuf+1+4*i,Addr+6+4*i,4);
		}
	}
	
	if(ValueFlag[ValueNum-2]==1){
		SPI_Flash_Read(ReadBuf+1+4*(AxisNum),Addr+6+4*(AxisNum),4);
	}
	
	if(ValueFlag[ValueNum-3]==2){
		SPI_Flash_Read(ReadBuf+4*(AxisNum)+5,Addr+10+4*(AxisNum),4);
	}

    ResParaRequest(buf,6 , ReadBuf,(9+AxisNum*4) , uartNo);
	//把这里的ReadBuf返回出去；
	if(ReadBuf!=NULL){
		free(ReadBuf);
		ReadBuf=NULL;
	}
	return;

}

void HeadInfo_Wirte(CDV_INT08U *buf){

	CDV_INT08U ResNo,ValueNum,i;
	CDV_INT16U Value;
	ResNo = buf[0];
	Value = CalculateForAll(buf,2,2);
	ValueNum = ValueSum(Value);
	for(i=0;i<ValueNum;i++){
		switch(ValueFlag[ValueNum-1-i]){
			case 0:
				ExGonggeNum[ResNo]=CalculateForAll(buf,4,2);
				SPI_Flash_Write(buf+4,SCRIP_GONGGE+2*ResNo,2);
				break;
			case 1:
				StartPoint[ResNo]=CalculateForAll(buf,6,2);//这里需要减去1，
				//StartPoint[ResNo] = StartPoint[ResNo] -1;
				SPI_Flash_Write(buf+6,SCRIP_GONGGE+20+2*ResNo,2);
				break;
			case 2:
				SPI_Flash_Write(buf+8,SCRIP_GONGGE+40+ResNo,1);//形状
				break;
			case 3:
				SPI_Flash_Write(buf+9,SCRIP_GONGGE+60+ResNo,1);//规格
				break;	
			case 4:
				SPI_Flash_Write(buf+10,SCRIP_GONGGE+80+ResNo,1);
				break;						
			default :
				break;
		}

	}
	return;
}


void HeadInfo_Read(CDV_INT08U *buf,CDV_INT08U uartNo){
	
	CDV_INT08U ResNo,ValueNum,tempbuf[7],i;
	CDV_INT16U Value;
	ResNo = buf[0];
	Value = CalculateForAll(buf,2,2);
	ValueNum = ValueSum(Value);
	for(i=0;i<ValueNum;i++){
		switch(ValueFlag[ValueNum-1-i]){
			case 0:
				SPI_Flash_Read(tempbuf,SCRIP_GONGGE+2*ResNo,2);
				ExGonggeNum[ResNo]=CalculateForAll(tempbuf,0,2);
				break;
			case 1:
				SPI_Flash_Read(tempbuf+2,SCRIP_GONGGE+20+2*ResNo,2);
				StartPoint[ResNo]=CalculateForAll(tempbuf,0,2);
				break;	
			case 2:
				SPI_Flash_Read(tempbuf+4,SCRIP_GONGGE+40+ResNo,1);
				break;	
			case 3:
				SPI_Flash_Read(tempbuf+5,SCRIP_GONGGE+60+ResNo,1);
				break;
			case 4:
				SPI_Flash_Read(tempbuf+6,SCRIP_GONGGE+80+ResNo,1);
				break;				
			default :
				break;
		}

	}
	ResParaRequest(buf,4,tempbuf,7, uartNo);
	return;

}

#endif

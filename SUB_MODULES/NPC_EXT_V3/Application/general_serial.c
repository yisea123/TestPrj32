#include "general_serial.h"

CDV_INT08U general_serial_buf[64]={0};
u32 general_serial_count=0;
u32 general_serial_count2=0;
CDV_INT08U internal_cache_type=0x02;//�ڲ���������,Ĭ��16λ
CDV_INT08U external_cache_type=0x02;//�ⲿ�������ͣ�Ĭ��16λ

CDV_INT08U internal_cache_size;//�ڲ����泤��
CDV_INT08U external_cache_size;//�ⲿ���泤��
u8 *external_cache_P8;
u16 *external_cache_P16;
u32 *external_cache_P32;

u8 *internal_cache_P8;
u16 *internal_cache_P16;
u32 *internal_cache_P32;

u8 cache_pose=0;
//*****************************************************************
//ͨ�ô�����Դ
//
//
//*****************************************************************
RET_STATUS General_Serial(CDV_INT08U* rxBuf, CDV_INT08U rxLen) 
{ 
	RET_STATUS ret =OPT_SUCCESS;
	OS_ERR err;
	
	u8 camera_buf[]={0,0};
	u8 i,j;
	CDV_INT08U opt; //strLen;
	u16 pflag;
	u8 *bufP8=NULL;
	u8 empty[2]={0,0};
	OSSemPend(&GENERAL_SERIAL_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
	//no   = rxBuf[0];//��Դ��
	opt  = rxBuf[1]-0x30;//������2
	//type = rxBuf[2];//������3
	
    pflag= rxBuf[3];//�������ţ����泤�ȣ������ʽ
	switch(opt) 
	{
		case 0x01://���ⲿ����
			      switch(external_cache_type)//�ж���������
				  {
					 case 00: //8λ�޷���
							   
					 case 01://8λ�з���
							 if(external_cache_P8!= NULL) 
							 { 
								  NEW08U(bufP8,external_cache_size+1);
								  //bufP8=(u8*)malloc(sizeof(u8)*(external_cache_size+1));
								  *bufP8=external_cache_size;
								  memcpy(bufP8+1,external_cache_P8, external_cache_size); 
									general_serial_qequest(rxBuf,rxLen,bufP8,(CDV_INT08U)external_cache_size+1);
								 // free(bufP8);
								   DELETE(bufP8);
								  //bufP8=NULL;
							 }
							 else
							 {
                  general_serial_qequest(rxBuf,rxLen,empty,2);
							 }
							 break;
					 case 02://16λ�޷���
						  
							   //break;
					 case 03://16λ�з���
							 if(external_cache_P16!= NULL) 
							 { 
								  NEW08U(bufP8,sizeof(u16)*(external_cache_size)+1);
								 // bufP8=(u8*)malloc(sizeof(u16)*(external_cache_size)+1);
								  *bufP8=external_cache_size;
								   for(i=0;i<external_cache_size;i++)
								   { 
										  for(j=1;j<3;j++)
										  {
											 *(bufP8+i*2+j)=( (((*(external_cache_P16+i))&0x00ff)<<8)+(((*(external_cache_P16+i))&0xff00)>>8) )>>(8*(j-1));
										  }		  
								   }
								   general_serial_qequest(rxBuf,rxLen,(CDV_INT08U*)bufP8,(CDV_INT08U)external_cache_size*2+1);
								   DELETE(bufP8);
								   // bufP8=NULL; 
							 }
							 else
							 {
								 general_serial_qequest(rxBuf,rxLen,empty,2);
							 }
							   break;
					 case 04://32λ�з���
							 if(external_cache_P32!= NULL) 
							 { 
								 NEW08U(bufP8,sizeof(u32)*(external_cache_size)+1);
								  //bufP8=(u8*)malloc(sizeof(u32)*(external_cache_size)+1);
								  *bufP8=external_cache_size;
								   for(i=0;i<external_cache_size;i++)
								   { 
										  for(j=1;j<5;j++)
										  {
											 *(bufP8+i*4+j)=( (((*(external_cache_P32+i))&0x000000ff)<<24) +(((*(external_cache_P32+i))&0x0000ff00)<<8)+(((*(external_cache_P32+i))&0x00ff0000)>>8)+(((*(external_cache_P32+i))&0xff000000)>>24))>>(8*(j-1));
										  }		  
								  }
								  general_serial_qequest(rxBuf,rxLen,(CDV_INT08U*)bufP8,(CDV_INT08U)external_cache_size*4+1);
								  //free(bufP8);
								  //bufP8=NULL;
								  DELETE(bufP8);
							 }
							 else
							 {
							   general_serial_qequest(rxBuf,rxLen,empty,2);
							 }
							 break;
					 default:
							   //OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err);
							   //return OPT_FAILURE;
							   break;
	               }						 
			       break;
		case 0x02://д�ⲿ���� 
		         external_cache_size=pflag;//���滺�泤�ȣ��Ǹ����������ֽ���
				 switch(external_cache_type)//�ж���������
				 {
							 case 00: //8λ�޷���
								       
							 case 01://8λ�з���
								    if(external_cache_P8!=NULL)//�ⲿ�������û��ȡ�߽��ᱻ����
								    {
										DELETE(external_cache_P8);
									  //external_cache_P8=NULL;
								    }	
									
									NEW08U(external_cache_P8,sizeof(u8)*pflag);								  
								    // external_cache_P8=(u8*)malloc(sizeof(u8)*pflag);
							        if(external_cache_P8!= NULL) 
								    { 
									    for(i=0;i<pflag;i++)
									    {
											*(external_cache_P8+i)=rxBuf[4+i];
									    }	 
	                                }
								    else//�����ڴ�ʧ��
								    {
										OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
										return OPT_FAILURE;
									}
								    break;
							 case 02://16λ�޷���
								  
								       //break;
							 case 03://16λ�з���
								     if(external_cache_P16!=NULL)//�ⲿ�������û��ȡ�߽��ᱻ����
									 {
										   DELETE(external_cache_P16);
											//free(external_cache_P16);
										  //external_cache_P16=NULL;
									 }
									 
									  NEW08U(external_cache_P16,sizeof(u16)*pflag);
								       //external_cache_P16=(u16*)malloc(sizeof(u16)*pflag);
							          if(external_cache_P16!= NULL) 
									  { 
										  for(i=0;i<pflag;i++)
										  {
												*(external_cache_P16+i)=(rxBuf[4+i*2]<<8)+rxBuf[5+i*2];
										  }	 
	                                  }
									  else
									  {
										  OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err);
		                                  return OPT_FAILURE;
	                                  }
								       break;
							 case 04://32λ�з���
								    if(external_cache_P32!=NULL)//�ⲿ�������û��ȡ�߽��ᱻ����
								    {
										DELETE(external_cache_P32);
									  //external_cache_P32=NULL;
								    }
									NEW08U(external_cache_P32,sizeof(u32)*pflag);	 
								       //external_cache_P32=(u32*)malloc(sizeof(u32)*pflag);
								    if(external_cache_P32!= NULL) 
								    { 
									   for(i=0;i<pflag;i++)
									   {
											*(external_cache_P32+i)=(rxBuf[4+i*4]<<24)+(rxBuf[5+i*4]<<16)+(rxBuf[6+i*4]<<8)+rxBuf[7+i*4];
									   }	 
	                                }
								    else
								    {
									   OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err);
		                               return OPT_FAILURE;
	                                }
								    break;
							 default:
								       //OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err); 
								       //return OPT_FAILURE;
								       break;
                  }						 
			      break;
		case 0x03://���û����ʽ
				 external_cache_type = pflag;
				 break;
		case 0x04://�������ʽ
				 general_serial_qequest(rxBuf,rxLen ,&external_cache_type ,1);
				 break;
		case 0x05://���ڲ����浽����
				 if(internal_cache_size>0) //�ж��ڲ�����������
				 {	
					 switch(internal_cache_type)//�ж���������
					 {
						 case 00: //8λ�޷���
										 
						 case 01://8λ�з���
								ValSet(rxBuf[3],*(internal_cache_P8+cache_pose));
								cache_pose++;
								internal_cache_size--; 	//�ڲ����������һ					
								break;
						 case 02://16λ�޷���
										 
								//break;
						 case 03://16λ�з���
								ValSet(rxBuf[3],(*(internal_cache_P16+cache_pose)));
								cache_pose=cache_pose+1;
								internal_cache_size--;
								 break;
						 case 04://32λ�з���													 
								ValSet(rxBuf[3],(*(internal_cache_P32+cache_pose)));
								cache_pose=cache_pose+1;
								internal_cache_size--;
								break;
						 default:
								break;
								   //OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err);
										 //return OPT_FAILURE;
				    }	
				 }
                 else//����ڲ�����û���ݣ���ʩ�ŵ��ڲ�������ڴ棬�ٴ��ⲿ���濽�����ڲ�����
				 {
					cache_pose=0;
					if(internal_cache_P32!=NULL)
					{
						DELETE(internal_cache_P32);
						//internal_cache_P32=NULL;
					}	
					if(internal_cache_P16!=NULL)
					{
						DELETE(internal_cache_P16);
					  //internal_cache_P16=NULL;
					}
					if(internal_cache_P8!=NULL)
					{
					   DELETE(internal_cache_P8);
					  //internal_cache_P8=NULL;
					}
					
					if(External2Internal())//��������ⲿ���浽�ڲ�����ɹ�
					{ 
						internal_cache_size=external_cache_size;//ˢ���ڲ��������
						internal_cache_type=external_cache_type;//ˢ���ڲ������ʽ
						
						if(internal_cache_type<=0x01)
						{
							ValSet(rxBuf[3],(*(internal_cache_P8+cache_pose)));
							cache_pose=cache_pose+1;
							internal_cache_size--;															 
							DELETE(external_cache_P8);//ȡ����ͷŵ��ⲿ����
							//external_cache_P8=NULL;
							external_cache_size=0;//����������
						}	
						else if(internal_cache_type<=0x03)
						{
							ValSet(rxBuf[3],(*(internal_cache_P16+cache_pose)));
							cache_pose=cache_pose+1;
							internal_cache_size--;															 
							DELETE(external_cache_P16);
							//external_cache_P16=NULL;
							
						}
						else if(internal_cache_type<=0x04)
						{
							ValSet(rxBuf[3],(*(internal_cache_P32+cache_pose)));
							cache_pose=cache_pose+1;
							internal_cache_size--;															 
							DELETE(external_cache_P32);
							//external_cache_P32=NULL;
							
						}
						else
						{
							OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err);											
							return OPT_FAILURE;		
						}	
					}
					else
					{	
						ValSet(rxBuf[3],0);
						OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err);
						return OPT_FAILURE;
					}	
				 }							
			     break;
		case 0x06://���ڲ�����ʣ�೤�ȵ�����
				 if(internal_cache_size>0)//�ڲ�ʣ�໺�泤�ȴ���0ֱ�ӷ���ʣ�೤��
				 {   
					  ValSet(rxBuf[3],internal_cache_size);
				 }
				 else//�ڲ�ʣ�໺�泤�ȵ���0���ȶ�ȡ�ⲿ���棬�����Ϊ�շ��س��ȣ����򷵻�0
				 {  
				    if(External2Internal())
						{ 	
								ValSet(rxBuf[3],external_cache_size);
						} 
				    else
				    {    
								ValSet(rxBuf[3],0);
				    }		
				 }   							 
		     break;
		case 0x07://�����ݵ����
			       camera_buf[0]=pflag;//�����
             USART3_Send(camera_buf,sizeof(camera_buf));
             break;		
		default:
			    break;
		
	}
    OSSemPost (&GENERAL_SERIAL_SEM,OS_OPT_POST_1,&err);
	return ret;
}
//*********************************************************
//ȡ�ⲿ���浽�ڲ�����
//
//����0ȡʧ�ܣ�����1ȡ�ɹ�
//*********************************************************
u8 External2Internal(void)
{
	switch(external_cache_type)//�ж���������
	{
	 case 00: //8λ�޷���
					 
	 case 01://8λ�з���
		     if(external_cache_P8==NULL)//�ⲿ����û���ݾ�ֱ�ӷ���
			 {
				 return 0;
	         }
			 
	         if(internal_cache_P8!= NULL) 
			 { 
				 DELETE(internal_cache_P8);
			 }
			 NEW08U(internal_cache_P8,sizeof(u8)*external_cache_size);
			 //internal_cache_P8=(u8*)malloc(sizeof(u8)*external_cache_size);
			 
			 if(internal_cache_P8!= NULL) 
			 { 
					memcpy(internal_cache_P8,external_cache_P8, external_cache_size);
			 }
			 else
			 {
					return 0;
			 }
			 break;
	 case 02://16λ�޷���
			
					 //break;
	 case 03://16λ�з���
		     if(external_cache_P16==NULL)
			 {	 
				 return 0;
			 }
			 
			 if(internal_cache_P16!= NULL) 
			 { 
				  DELETE(internal_cache_P16);
			 }
			 //internal_cache_P16=(u16*)malloc(sizeof(u16)*external_cache_size);
			 NEW08U(internal_cache_P16,sizeof(u16)*external_cache_size);
			 
			 if(internal_cache_P16!= NULL) 
			 { 
					memcpy(internal_cache_P16,external_cache_P16, external_cache_size*2);
			 }
			 else
			 {
					return 0;
			 }
			 break;
	 case 04://32λ�з���
		     if(external_cache_P32==NULL)return 0;
	         if(internal_cache_P32!= NULL) 
			 { 
				 DELETE(internal_cache_P32);
			 }
			 
			 //internal_cache_P32=(u32*)malloc(sizeof(u32)*external_cache_size);
			 NEW08U(internal_cache_P32,sizeof(32)*external_cache_size);
			 
			 if(internal_cache_P32!= NULL) 
			 { 
					memcpy(internal_cache_P32,external_cache_P32, external_cache_size*4); 
			 }
			 else
			 {
					return 0;
			 }
			 break;
	 default:
					 return 0;
					 //break;
 }
 return 1;	
}	

//**************************************
//ͨ�ô��ڷ���
//
//
//**************************************
void general_serial_qequest(CDV_INT08U* rxBuf, CDV_INT08U rxLen , CDV_INT08U* para, CDV_INT08U paraLen)
{
	//OS_ERR err;
	u16 crc;
	u8 rxRealLen = rxLen + 3;
	u8 txLen = paraLen + rxRealLen+2;
	u8 *txBuf = NULL;
	
	txBuf=(u8*)malloc(sizeof(u8)*txLen);

	//CDV_INT08U *txBuf = (CDV_INT08U*)malloc(sizeof(CDV_INT08U)*(txLen));
	memcpy(txBuf , rxBuf - 3 , rxRealLen);
	memcpy(txBuf + rxRealLen , para , paraLen);
	
	crc = getCRC16(txBuf,txLen-2);
	*(txBuf+txLen-1) = (crc >> 8) & 0xff;
	*(txBuf+txLen-2) = crc & 0xff;  
	

	USART3_Send(txBuf,(u32)txLen);
	DELETE(txBuf);
	//txBuf = NULL;

}
/**************************************
ͨ�ô���ԭ������


**************************************/
void general_serial_return(CDV_INT08U* rxBuf, CDV_INT08U rxLen)
{
	if( (*(rxBuf+4)==0x32) || (*(rxBuf+4)==0x33) )//��������������ԭ������
	{	
	
		USART3_Send(rxBuf,(u32)rxLen);
	}	
	
}


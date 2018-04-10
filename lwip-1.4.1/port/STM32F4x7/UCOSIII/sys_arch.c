/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
/*  Porting by Michael Vysotsky <michaelvy@hotmail.com> August 2011   */

#define SYS_ARCH_GLOBALS

/* lwIP includes. */
#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "includes.h"
#include "delay.h"
#include "arch/sys_arch.h"
//#include "malloc.h"
#include "os_cfg_app.h"

//����Ϣָ��Ϊ��ʱ,ָ��һ������pvNullPointer��ָ���ֵ.
//��UCOS�����OSQPost()�е�msg==NULL�᷵��һ��OS_ERR_POST_NULL
//����,����lwip�л����sys_mbox_post(mbox,NULL)����һ������Ϣ,����
//�ڱ������а�NULL���һ������ָ��0Xffffffff
const void * const pvNullPointer = (mem_ptr_t*)0xffffffff;
 

//����һ����Ϣ����
//*mbox:��Ϣ����
//size:�����С
//����ֵ:ERR_OK,�����ɹ�
//         ����,����ʧ��
err_t sys_mbox_new( sys_mbox_t *mbox, int size)
{
	OS_ERR err;
	if(size>MAX_QUEUE_ENTRIES)size=MAX_QUEUE_ENTRIES;		//��Ϣ�����������MAX_QUEUE_ENTRIES��Ϣ��Ŀ 
	OSQCreate((OS_Q*		)mbox,				//��Ϣ����
              (CPU_CHAR*	)"LWIP Quiue",		//��Ϣ��������
              (OS_MSG_QTY	)size,				//��Ϣ���г���
              (OS_ERR*		)&err);				//������
	if(err==OS_ERR_NONE) return ERR_OK;
	return ERR_MEM;
} 
//�ͷŲ�ɾ��һ����Ϣ����
//*mbox:Ҫɾ������Ϣ����
void sys_mbox_free(sys_mbox_t * mbox)
{
	OS_ERR err;
	
#if OS_CFG_Q_FLUSH_EN > 0u  
	OSQFlush(mbox,&err);
#endif
	
	OSQDel((OS_Q*	)mbox,
           (OS_OPT	)OS_OPT_DEL_ALWAYS,
           (OS_ERR*	)&err);
	LWIP_ASSERT( "OSQDel ",err == OS_ERR_NONE ); 
}
//����Ϣ�����з���һ����Ϣ(���뷢�ͳɹ�)
//*mbox:��Ϣ����
//*msg:Ҫ���͵���Ϣ
void sys_mbox_post(sys_mbox_t *mbox,void *msg)
{    
		OS_ERR err;
	CPU_INT08U i=0;
	if(msg==NULL)msg=(void*)&pvNullPointer;	//��msgΪ��ʱ msg����pvNullPointerָ���ֵ 
	//������Ϣ
    while(i<10)	//��10��
	{
		OSQPost((OS_Q*		)mbox,		
			    (void*		)msg,
			    (OS_MSG_SIZE)strlen(msg),
			    (OS_OPT		)OS_OPT_POST_FIFO,
			    (OS_ERR*	)&err);
		if(err==OS_ERR_NONE) break;
		i++;
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ5ms
	}
	LWIP_ASSERT( "sys_mbox_post error!\n", i !=10 );  	
}
//������һ����Ϣ���䷢����Ϣ
//�˺��������sys_mbox_post����ֻ����һ����Ϣ��
//����ʧ�ܺ󲻻᳢�Եڶ��η���
//*mbox:��Ϣ����
//*msg:Ҫ���͵���Ϣ
//����ֵ:ERR_OK,����OK
// 	     ERR_MEM,����ʧ��
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg)
{ 
	OS_ERR err;
	if(msg==NULL)msg=(void*)&pvNullPointer;//��msgΪ��ʱ msg����pvNullPointerָ���ֵ 
	OSQPost((OS_Q*		)mbox,		
			(void*		)msg,
			(OS_MSG_SIZE)sizeof(msg),
			(OS_OPT		)OS_OPT_POST_FIFO,
			(OS_ERR*	)&err);
	if(err!=OS_ERR_NONE) return ERR_MEM;
	return ERR_OK;
}

//�ȴ������е���Ϣ
//*mbox:��Ϣ����
//*msg:��Ϣ
//timeout:��ʱʱ�䣬���timeoutΪ0�Ļ�,��һֱ�ȴ�
//����ֵ:��timeout��Ϊ0ʱ����ɹ��Ļ��ͷ��صȴ���ʱ�䣬
//		ʧ�ܵĻ��ͷ��س�ʱSYS_ARCH_TIMEOUT
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{ 
	OS_ERR err;
	OS_MSG_SIZE size;
	u32_t ucos_timeout,timeout_new;
	void *temp;
	if(timeout!=0)
	{
		ucos_timeout=(timeout*OS_CFG_TICK_RATE_HZ)/1000; //ת��Ϊ������,��ΪUCOS��ʱʹ�õ��ǽ�����,��LWIP����ms
		if(ucos_timeout<1)ucos_timeout=1;//����1������
	}else ucos_timeout = 0; 
	timeout = OSTimeGet(&err); //��ȡϵͳʱ�� 
	//������Ϣ
	temp=OSQPend((OS_Q*			)mbox,   
				(OS_TICK		)ucos_timeout,
                (OS_OPT			)OS_OPT_PEND_BLOCKING,
                (OS_MSG_SIZE*	)&size,		
                (CPU_TS*		)0,
                (OS_ERR*		)&err);
	if(msg!=NULL)
	{	
		if(temp==(void*)&pvNullPointer)*msg = NULL;   	//��Ϊlwip���Ϳ���Ϣ��ʱ������ʹ����pvNullPointerָ��,�����ж�pvNullPointerָ���ֵ
 		else *msg=temp;									//�Ϳ�֪�����󵽵���Ϣ�Ƿ���Ч
	}    
	if(err==OS_ERR_TIMEOUT)timeout=SYS_ARCH_TIMEOUT;  //����ʱ
	else
	{
		LWIP_ASSERT("OSQPend ",err==OS_ERR_NONE); 
		timeout_new=OSTimeGet(&err);
		if (timeout_new>=timeout) timeout_new = timeout_new - timeout;//���������Ϣ��ʹ�õ�ʱ��
		else timeout_new = 0xffffffff - timeout + timeout_new; 
		timeout=timeout_new*1000/OS_CFG_TICK_RATE_HZ + 1; 
	}
	return timeout; 
}
//���Ի�ȡ��Ϣ
//*mbox:��Ϣ����
//*msg:��Ϣ
//����ֵ:�ȴ���Ϣ���õ�ʱ��/SYS_ARCH_TIMEOUT
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)
{
	return sys_arch_mbox_fetch(mbox,msg,1);//���Ի�ȡһ����Ϣ
}
//���һ����Ϣ�����Ƿ���Ч
//*mbox:��Ϣ����
//����ֵ:1,��Ч.
//      0,��Ч
int sys_mbox_valid(sys_mbox_t *mbox)
{  
	if(mbox->NamePtr)  
		return (strcmp(mbox->NamePtr,"?Q"))? 1:0;
	else
		return 0;
} 
//����һ����Ϣ����Ϊ��Ч
//*mbox:��Ϣ����
void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
	if(sys_mbox_valid(mbox))
    sys_mbox_free(mbox);
} 
//����һ���ź���
//*sem:�������ź���
//count:�ź���ֵ
//����ֵ:ERR_OK,����OK
// 	     ERR_MEM,����ʧ��
err_t sys_sem_new(sys_sem_t * sem, u8_t count)
{  
	OS_ERR err;
	OSSemCreate ((OS_SEM*	)sem,
                 (CPU_CHAR*	)"LWIP Sem",
                 (OS_SEM_CTR)count,		
                 (OS_ERR*	)&err);
	if(err!=OS_ERR_NONE)return ERR_MEM; 
	LWIP_ASSERT("OSSemCreate ",sem != NULL );
	return ERR_OK;
} 
//�ȴ�һ���ź���
//*sem:Ҫ�ȴ����ź���
//timeout:��ʱʱ��
//����ֵ:��timeout��Ϊ0ʱ����ɹ��Ļ��ͷ��صȴ���ʱ�䣬
//		ʧ�ܵĻ��ͷ��س�ʱSYS_ARCH_TIMEOUT
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{ 
	OS_ERR err;
	u32_t ucos_timeout, timeout_new; 
	if(	timeout!=0) 
	{
		ucos_timeout = (timeout * OS_CFG_TICK_RATE_HZ) / 1000;//ת��Ϊ������,��ΪUCOS��ʱʹ�õ��ǽ�����,��LWIP����ms
		if(ucos_timeout < 1)
		ucos_timeout = 1;
	}else ucos_timeout = 0; 
	timeout = OSTimeGet(&err);  
	OSSemPend(sem,ucos_timeout,OS_OPT_PEND_BLOCKING,0,&err); //�����ź���
 	if(err == OS_ERR_TIMEOUT)timeout=SYS_ARCH_TIMEOUT;//����ʱ	
	else
	{     
 		timeout_new = OSTimeGet(&err); 
		if (timeout_new>=timeout) timeout_new = timeout_new - timeout;
		else timeout_new = 0xffffffff - timeout + timeout_new;
 		timeout = (timeout_new*1000/OS_CFG_TICK_RATE_HZ + 1);//���������Ϣ��ʹ�õ�ʱ��(ms)
	}
	return timeout;
}
//����һ���ź���
//sem:�ź���ָ��
void sys_sem_signal(sys_sem_t *sem)
{
	OS_ERR err;
	OSSemPost(sem,OS_OPT_POST_1,&err);//�����ź���
	LWIP_ASSERT("OSSemPost ",err == OS_ERR_NONE ); 
}
//�ͷŲ�ɾ��һ���ź���
//sem:�ź���ָ��
void sys_sem_free(sys_sem_t *sem)
{
	OS_ERR err;
	OSSemDel(sem,OS_OPT_DEL_ALWAYS,&err);
    LWIP_ASSERT("OSSemDel ",err==OS_ERR_NONE);
	sem = NULL;
} 
//��ѯһ���ź�����״̬,��Ч����Ч
//sem:�ź���ָ��
//����ֵ:1,��Ч.
//      0,��Ч
int sys_sem_valid(sys_sem_t *sem)
{
	if(sem->NamePtr)
		return (strcmp(sem->NamePtr,"?SEM"))? 1:0;
	else
		return 0;           
}

//����һ���ź�����Ч
//sem:�ź���ָ��
void sys_sem_set_invalid(sys_sem_t *sem)
{
	if(sys_sem_valid(sem))
     sys_sem_free(sem);
} 

//arch��ʼ��
void sys_init(void)
{ 
    //����,�����ڸú���,�����κ�����
}
extern CPU_STK * TCPIP_THREAD_TASK_STK;//TCP IP�ں������ջ,��lwip_comm��������
//LWIP�ں������������ƿ�
OS_TCB TcpipthreadTaskTCB;//����һ���½���
//*name:��������
//thred:����������
//*arg:�����������Ĳ���
//stacksize:��������Ķ�ջ��С
//prio:������������ȼ�
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	if(strcmp(name,TCPIP_THREAD_NAME)==0)//����TCP IP�ں�����
	{
		OS_CRITICAL_ENTER();	//�����ٽ���			 
		//������ʼ����
		OSTaskCreate((OS_TCB 	* )&TcpipthreadTaskTCB,			//������ƿ�
					 (CPU_CHAR	* )"TCPIPThread task", 			//��������
                     (OS_TASK_PTR )thread, 						//������
                     (void		* )0,							//���ݸ��������Ĳ���
                     (OS_PRIO	  )prio,     					//�������ȼ�
                     (CPU_STK   * )&TCPIP_THREAD_TASK_STK[0],	//�����ջ����ַ
                     (CPU_STK_SIZE)stacksize/10,				//�����ջ�����λ
                     (CPU_STK_SIZE)stacksize,					//�����ջ��С
                     (OS_MSG_QTY  )0,							//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                     (OS_TICK	  )0,							//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                     (void   	* )0,							//�û�����Ĵ洢��
                     (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                     (OS_ERR 	* )&err);					//��Ÿú�������ʱ�ķ���ֵ
		OS_CRITICAL_EXIT();	//�˳��ٽ���
	} 
	return 0;
} 
//lwip��ʱ����
//ms:Ҫ��ʱ��ms��
void sys_msleep(u32_t ms)
{
	delay_ms(ms);
}
//��ȡϵͳʱ��,LWIP1.4.1���ӵĺ���
//����ֵ:��ǰϵͳʱ��(��λ:����)
u32_t sys_now(void)
{
	OS_ERR err;
	u32_t ucos_time, lwip_time;
	ucos_time=OSTimeGet(&err);	//��ȡ��ǰϵͳʱ�� �õ�����UCSO�Ľ�����
	lwip_time=(ucos_time*1000/OS_CFG_TICK_RATE_HZ+1);//��������ת��ΪLWIP��ʱ��MS
	return lwip_time; 		//����lwip_time;
}












































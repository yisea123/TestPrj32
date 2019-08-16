/**
  ******************************************************************************
  * @file    /workerDebug.h 
  * @author  MMY
  * @version V1.0.0
  * @date    2018-12-12
  * @brief   
  * 
@verbatim  
		

@endverbatim        
  *
  ******************************************************************************
  * @attention
  *
  * COPYRIGHT 2018 CQT Quartz. Co., Ltd.
  *
  ******************************************************************************
  */
	
	#include "workerDebug.h"
	
	#if USE_WORKER_DEBUG == 1u
	
	OS_MUTEX DEBUG_SEM;
	
/** @brief  ��ʼ�����Կ��ƴ�
  * @param  
  * @retval 0�ɹ� ��0ʧ��
  * @note   
  */
  CDV_INT08S DebugOpen(DEBUG_CTRL* debug) {
		ASSERT(debug);
		if(debug->point)
			return -1;
		
		debug->point = CreateArray(sizeof(DEBUG_POINT));//��������DEBUG_POINT���͵�array
		//debug->state = 0;
		return 0;
	}
	
/** @brief  �رյ��Կ���
  * @param  
  * @retval 0�ɹ� ��0ʧ��
  * @note   
  */
  CDV_INT08S DebugClose(DEBUG_CTRL* debug) {
		ASSERT(debug);
		if(!debug->point)
			return -1;
		DeleteArray(&debug->point);
		return 0;
	}

/** @brief  ��ѯ�Ƿ�ϵ�
  * @param  pos    ����ѯ��λ��
  * @retval 1�Ƕϵ㣬0���Ƕϵ� 
  * @note   
  */
  BOOL IsDebugPoint(DEBUG_CTRL* debug, CDV_INT16U pos) {
		OS_ERR err;
		CDV_INT32U i;
		BOOL ret = FALSE;
		ASSERT(debug && debug->point);
		OSMutexPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		for(i = 0; i < debug->point->len; i++) {
		  DEBUG_POINT* debugPoint = (DEBUG_POINT*)ArrayGet(debug->point, i);
			if(pos == debugPoint->pos) {
				ret = TRUE;
				break;
			}
		}
		OSMutexPost (&DEBUG_SEM,OS_OPT_POST_NO_SCHED,&err);
		return ret;
	}
	
/** @brief  ��Ӷϵ�
  * @param  pos    ����ӵ�λ��
  * @retval 0�ɹ����Ѵ��� ��0ʧ��
  * @note   
  */
  CDV_INT08S AddDebugPos(DEBUG_CTRL* debug, CDV_INT16U pos) {
		OS_ERR err;
	  DEBUG_POINT debugPoint;
		CDV_INT08S ret = 0;
		CDV_INT32U i;
		ASSERT(debug);
		ASSERT(debug->point);
		debugPoint.pos = pos;
		debugPoint.buf = 0;
		debugPoint.len = 0;
		OSMutexPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		
		for(i = 0; i < debug->point->len; i++) {
		  DEBUG_POINT* debugPoint = (DEBUG_POINT*)ArrayGet(debug->point, i);
			if(pos == debugPoint->pos) {
				//OSSemPost (&DEBUG_SEM,OS_OPT_POST_1,&err);
				//return ret;
				ret = 1;
				break;
			}
		}
		if(!ret)
		  ret = ArrayAdd(debug->point, &debugPoint);
		else
			ret = 0;
		OSMutexPost (&DEBUG_SEM,OS_OPT_POST_NO_SCHED,&err);
		return ret;
	}
	
/** @brief  ��Ӷϵ�
  * @param  pos    ����ӵ�λ��
  * @retval 0�ɹ� ��0ʧ��
  * @note   
  */
  CDV_INT08S DeleteDebugPoint(DEBUG_CTRL* debug, CDV_INT32U no) {
		OS_ERR err;
		CDV_INT08S ret;
		ASSERT(debug);
		ASSERT(debug->point);
//		if(no >= debug->point->len)
//			return -1;
				
		OSMutexPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		ret = ArrayDelete(debug->point, no);
		OSMutexPost (&DEBUG_SEM,OS_OPT_POST_NO_SCHED,&err);
		return ret;
	}
	
	/** @brief  ��Ӷϵ�
  * @param  pos    ����ӵ�λ��
  * @retval 0�ɹ� ��0ʧ��
  * @note   
  */
  CDV_INT08S DeleteDebugPos(DEBUG_CTRL* debug, CDV_INT16U pos) {
		OS_ERR err;
		CDV_INT08S ret=0;
		CDV_INT32U i;
		ASSERT(debug);
		ASSERT(debug->point);
//		if(no >= debug->point->len)
//			return -1;
		OSMutexPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		
		for(i = 0; i < debug->point->len; i++) {
		  DEBUG_POINT* debugPoint = (DEBUG_POINT*)ArrayGet(debug->point, i);
			if(pos == debugPoint->pos) {
				ret = ArrayDelete(debug->point, i);
				break;
			}
		}
		
		OSMutexPost (&DEBUG_SEM,OS_OPT_POST_NO_SCHED,&err);
		return ret;
	}
	
	
/** @brief  �޸Ķϵ�
  * @param  no     ���޸ĵĶϵ���
	*         pos    ���޸ĵ�λ��
  * @retval 0�ɹ� ��0ʧ��
  * @note   
  */
  CDV_INT08S modifyDebugPoint(DEBUG_CTRL* debug, CDV_INT32U no, CDV_INT16U pos) {
		OS_ERR err;
		CDV_INT08S ret;
	  DEBUG_POINT debugPoint;
		ASSERT(debug);
		ASSERT(debug->point);
//		if(no >= debug->point->len)
//			return -1;
//		
		
		
		
		debugPoint.pos = pos;
		debugPoint.buf = 0;
		debugPoint.len = 0;
		OSMutexPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		ret = ArrayModify(debug->point, no, &debugPoint);
		OSMutexPost (&DEBUG_SEM,OS_OPT_POST_NO_SCHED,&err);
		return ret ;
	}
	
/** @brief  ����ϵ�
  * @param  
  * @retval RET_STATUS
  * @note   
  */
  CDV_INT08S ClearDebugPoint(DEBUG_CTRL* debug) {
		OS_ERR err;
		ASSERT(debug);
		if(!debug->point)
			return 0;
		OSMutexPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		ClearArray(debug->point);
		OSMutexPost (&DEBUG_SEM,OS_OPT_POST_NO_SCHED,&err);
		return 0;
	}
/** @brief  ��ѯ�ϵ�����
  * @param  
  * @retval 
  * @note   
  */
  CDV_INT32U QueryDebugPointNum(DEBUG_CTRL* debug) {
		ASSERT(debug);
		ASSERT(debug->point);
		return debug->point->len ;
	}
	
/** @brief  ��ѯ�ϵ�λ��
  * @param  
  * @retval 
  * @note   
  */
  CDV_INT08S QueryDebugPointPos(DEBUG_CTRL* debug, CDV_INT32U no, CDV_INT16U *pos) {
		OS_ERR err;
	  DEBUG_POINT *debugPoint;
		ASSERT(debug && pos);
		ASSERT(debug->point);

		OSMutexPend(&DEBUG_SEM, 0, OS_OPT_PEND_BLOCKING, 0, &err);
		debugPoint = ArrayGet(debug->point, no);
		OSMutexPost(&DEBUG_SEM, OS_OPT_POST_NO_SCHED, &err);
		
		if(debugPoint) {
			*pos = debugPoint->pos ;
			return 0;
		}
		
		return -1;
	}
		
/** @brief  ��ѯ���жϵ�λ��
  * @param  
  * @retval ����
  * @note   
  */
  CDV_INT32U QueryAllDebugPointPos(DEBUG_CTRL* debug, CDV_INT08U **pos) {
		OS_ERR err;
		CDV_INT32U num;
		CDV_INT32U i;
		ASSERT(debug && !*pos && pos);
		ASSERT(debug->point);
		

		OSMutexPend(&DEBUG_SEM, 0, OS_OPT_PEND_BLOCKING, 0, &err);
		num = QueryDebugPointNum(debug);
		if(num) {
			NEWCH(*pos, num * sizeof(CDV_INT16U));
			for( i = 0; i < num; i++) {
				DEBUG_POINT *debugPoint = ArrayGet(debug->point, i);
				ASSERT(debugPoint);
				MemCpy(*pos+i*sizeof(CDV_INT16U), &debugPoint->pos, sizeof(CDV_INT16U));
			}
		}
		OSMutexPost(&DEBUG_SEM, OS_OPT_POST_NO_SCHED, &err);
		
		return num;
	}
	
/** @brief  debug state set
  * @param  
  * @retval ����
  * @note   
  */
  void AllDebugStateSet(DEBUG_STATE state) {
		CDV_INT16U i;
		for( i = 0; i < WORKER_MAX_NUM; i++) {    
			g_threadInfo[i].debug.state = state;
		}
	}
/** @brief  �������
  * @param  
  * @retval 
  * @note   F1/F2 05
  */
  RET_STATUS WorkerDebugParse(CDV_INT08U* buf, CDV_INT08U len, CMD_ARG *arg)
	{
		RET_STATUS ret = OPT_FAILURE;
		CDV_INT08S err = 0;
		CDV_INT08U *tmp = NULL;
		CDV_INT16U i;
		CDV_INT16U no = *(CDV_INT16U*)(buf + 1);
		CDV_INT16U point ,pos;
		DEBUG_CTRL *ctrl = NULL;
		//DEBUG_STATE state;
		
		ASSERT(buf && len);
		
		for( i = 0; i < WORKER_MAX_NUM; i++) {                                   /*�жϹ����Ƿ��Ѿ�����*/
			
			if (no == g_threadInfo[i].which && IsTaskExist(WorkerTaskTCB[i])) {
				ret = OPT_SUCCESS;
				ctrl = &g_threadInfo[i].debug;
				break;
			}
		}
//		
//		if(ret == OPT_FAILURE) {
//			arg->buf[1]+=0x80;
//			ResRequest(arg->buf, arg->len, 0,0,arg, RC_CRC);
//			return ret;
//		}
	
		switch(buf[0]){
			
			case 0x00:
			{
				//DEBUG_STATE state;
				
				if(buf[1]) {
					ManagerControl(WORKER_STOP);
					//state = DEBUG_PAUSE;//DEBUG_ENABLE;//ʹ�ܶϵ�
				  AllDebugStateSet(DEBUG_PAUSE);
					ManagerControl(WORKER_ONCE);
				} else {
					AllDebugStateSet(DEBUG_DISABLE);
				}
				
				
//				for( i = 0; i < WORKER_MAX_NUM; i++) {    
//					g_threadInfo[i].debug.state = state;//ʹ�ܶϵ�
//				}
				
				break;
			}
			case 0x01:
				if(ctrl) {
					pos = *(CDV_INT16U*)(buf + 3);
					//state = ctrl->state;
					//ctrl->state = DEBUG_PAUSE;//ʧ�ܶϵ�
					//delay_ms(10);//��ֹ��ͻ
					err = AddDebugPos(ctrl, pos);
					//ctrl->state = state;
				} else {
					err = -1;
				}
				break;
				
			case 0x02:
				if(ctrl) {
					pos = *(CDV_INT16U*)(buf + 3);
					//state = ctrl->state;
					//ctrl->state = DEBUG_PAUSE;//ʧ�ܶϵ�
					//delay_ms(10);//��ֹ��ͻ
					err = DeleteDebugPos(ctrl, pos);
					//ctrl->state = state;
				} else {
					err = -1;
				}
				break;
				
			case 0x03:
				
				for( i = 0; i < WORKER_MAX_NUM; i++) {     
					ClearDebugPoint(&g_threadInfo[i].debug);
				}
				//state = ctrl->state;
				//ctrl->state = DEBUG_PAUSE;//ʧ�ܶϵ�
			  //delay_ms(10);//��ֹ��ͻ
				//err = ClearDebugPoint(ctrl);
			  //ctrl->state = state;
				break;
				
			case 0x04://����
				AllDebugStateSet(DEBUG_CONTINUE);
					
				//ctrl->state = DEBUG_CONTINUE;
				break;
					
			case 0x05://ֹͣ
				AllDebugStateSet(DEBUG_PAUSE);

				//ctrl->state = DEBUG_PAUSE;
				break;
					
			case 0x06://����
				if(ctrl) {
					ctrl->state = DEBUG_STEP;
				} else {
					err = -1;
				}
				break;
				
			case 0x07://��ѯ�ϵ�
				if(ctrl) {
					point = *(CDV_INT16U*)(buf + 3);
					err = QueryDebugPointPos(ctrl, point, &pos);
				
					if(!err) {
						ResRequest(arg->buf, arg->len, (CDV_INT08U*)&pos,2,arg, RC_CRC);
						return ret;
					}
				} else {
					err = -1;
				}
				break;
				
			case 0x08://��ѯ���жϵ�
				if(ctrl) {
					pos = QueryAllDebugPointPos(ctrl, &tmp);
				
					if(pos) {
						ResRequest(arg->buf, arg->len, tmp,2*pos,arg, RC_CRC);
						DELETE(tmp);
						return ret;
					} else {
						err = -1;
					}
				} else {
					err = -1;
				}
				break;
				
			case 0x09://��ѯλ��
				if(ctrl) {
					ret = WorkerQueryStep(no, &pos);
				
					if(OPT_SUCCESS == ret) {
						ResRequest(arg->buf, arg->len, (CDV_INT08U*)&pos, 2, arg, RC_CRC);
						return ret;
					} else {
						err = -1;
					}
				} else {
					err = -1;
				}
				break;
				
			default:
				break;
		}
		
		if(err != 0) {
			arg->buf[1]+=0x80;
		}
		
		ResRequest(arg->buf, arg->len, 0,0,arg, RC_CRC);
		
		return ret;
	}

#endif
	
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
	#if USE_WORKER_DEBUG == 1u
	#include "workerDebug.h"
	
	OS_SEM DEBUG_SEM;
	
/** @brief  初始化调试控制打开
  * @param  
  * @retval 0成功 非0失败
  * @note   
  */
  CDV_INT08S DebugOpen(DEBUG_CTRL* debug) {
		ASSERT(debug);
		if(debug->point)
			return -1;
		
		debug->point = CreateArray(sizeof(DEBUG_POINT));//创建报错DEBUG_POINT类型的array
		//debug->state = 0;
		return 0;
	}
	
/** @brief  关闭调试控制
  * @param  
  * @retval 0成功 非0失败
  * @note   
  */
  CDV_INT08S DebugClose(DEBUG_CTRL* debug) {
		ASSERT(debug);
		if(!debug->point)
			return -1;
		DeleteArray(&debug->point);
		return 0;
	}

/** @brief  查询是否断点
  * @param  pos    待查询的位置
  * @retval 1是断点，0不是断点 
  * @note   
  */
  BOOL IsDebugPoint(DEBUG_CTRL* debug, CDV_INT16U pos) {
		OS_ERR err;
		CDV_INT32U i;
		BOOL ret = FALSE;
		ASSERT(debug && debug->point);
		OSSemPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		for(i = 0; i < debug->point->len; i++) {
		  DEBUG_POINT* debugPoint = (DEBUG_POINT*)ArrayGet(debug->point, i);
			if(pos == debugPoint->pos) {
				ret = TRUE;
				break;
			}
		}
		OSSemPost (&DEBUG_SEM,OS_OPT_POST_1,&err);
		return ret;
	}
	
/** @brief  添加断点
  * @param  pos    待添加的位置
  * @retval 0成功或已存在 非0失败
  * @note   
  */
  CDV_INT08S AddDebugPos(DEBUG_CTRL* debug, CDV_INT16U pos) {
		OS_ERR err;
		CDV_INT08S ret = 0;
		CDV_INT32U i;
		ASSERT(debug);
		ASSERT(debug->point);
		DEBUG_POINT debugPoint = {pos,0,0};
		OSSemPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		
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
		OSSemPost (&DEBUG_SEM,OS_OPT_POST_1,&err);
		return ret;
	}
	
/** @brief  添加断点
  * @param  pos    待添加的位置
  * @retval 0成功 非0失败
  * @note   
  */
  CDV_INT08S DeleteDebugPoint(DEBUG_CTRL* debug, CDV_INT32U no) {
		OS_ERR err;
		CDV_INT08S ret;
		ASSERT(debug);
		ASSERT(debug->point);
//		if(no >= debug->point->len)
//			return -1;
				
		OSSemPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		ret = ArrayDelete(debug->point, no);
		OSSemPost (&DEBUG_SEM,OS_OPT_POST_1,&err);
		return ret;
	}
	
	/** @brief  添加断点
  * @param  pos    待添加的位置
  * @retval 0成功 非0失败
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
		OSSemPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		
		for(i = 0; i < debug->point->len; i++) {
		  DEBUG_POINT* debugPoint = (DEBUG_POINT*)ArrayGet(debug->point, i);
			if(pos == debugPoint->pos) {
				ret = ArrayDelete(debug->point, i);
				break;
			}
		}
		
		OSSemPost (&DEBUG_SEM,OS_OPT_POST_1,&err);
		return ret;
	}
	
	
/** @brief  修改断点
  * @param  no     待修改的断点编号
	*         pos    待修改的位置
  * @retval 0成功 非0失败
  * @note   
  */
  CDV_INT08S modifyDebugPoint(DEBUG_CTRL* debug, CDV_INT32U no, CDV_INT16U pos) {
		OS_ERR err;
		CDV_INT08S ret;
		ASSERT(debug);
		ASSERT(debug->point);
//		if(no >= debug->point->len)
//			return -1;
//		
		DEBUG_POINT debugPoint = {pos,0,0};
		OSSemPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		ret = ArrayModify(debug->point, no, &debugPoint);
		OSSemPost (&DEBUG_SEM,OS_OPT_POST_1,&err);
		return ret ;
	}
	
/** @brief  清除断点
  * @param  
  * @retval RET_STATUS
  * @note   
  */
  CDV_INT08S ClearDebugPoint(DEBUG_CTRL* debug) {
		OS_ERR err;
		ASSERT(debug);
		if(!debug->point)
			return 0;
		OSSemPend(&DEBUG_SEM , 0 , OS_OPT_PEND_BLOCKING , 0 , &err);
		ClearArray(debug->point);
		OSSemPost (&DEBUG_SEM,OS_OPT_POST_1,&err);
		return 0;
	}
/** @brief  查询断点数量
  * @param  
  * @retval 
  * @note   
  */
  CDV_INT32U QueryDebugPointNum(DEBUG_CTRL* debug) {
		ASSERT(debug);
		ASSERT(debug->point);
		return debug->point->len ;
	}
	
/** @brief  查询断点位置
  * @param  
  * @retval 
  * @note   
  */
  CDV_INT08S QueryDebugPointPos(DEBUG_CTRL* debug, CDV_INT32U no, CDV_INT16U *pos) {
		OS_ERR err;
		ASSERT(debug && pos);
		ASSERT(debug->point);

		OSSemPend(&DEBUG_SEM, 0, OS_OPT_PEND_BLOCKING, 0, &err);
		DEBUG_POINT *debugPoint = ArrayGet(debug->point, no);
		OSSemPost(&DEBUG_SEM, OS_OPT_POST_1, &err);
		
		if(debugPoint) {
			*pos = debugPoint->pos ;
			return 0;
		}
		
		return -1;
	}
		
/** @brief  查询所有断点位置
  * @param  
  * @retval 数量
  * @note   
  */
  CDV_INT32U QueryAllDebugPointPos(DEBUG_CTRL* debug, CDV_INT08U **pos) {
		OS_ERR err;
		CDV_INT32U num;
		CDV_INT32U i;
		ASSERT(debug && !*pos && pos);
		ASSERT(debug->point);
		

		OSSemPend(&DEBUG_SEM, 0, OS_OPT_PEND_BLOCKING, 0, &err);
		num = QueryDebugPointNum(debug);
		if(num) {
			NEWCH(*pos, num * sizeof(CDV_INT16U));
			for( i = 0; i < num; i++) {
				DEBUG_POINT *debugPoint = ArrayGet(debug->point, i);
				ASSERT(debugPoint);
				MemCpy(*pos+i*sizeof(CDV_INT16U), &debugPoint->pos, sizeof(CDV_INT16U));
			}
		}
		OSSemPost(&DEBUG_SEM, OS_OPT_POST_1, &err);
		
		return num;
	}
	
/** @brief  debug state set
  * @param  
  * @retval 数量
  * @note   
  */
  void AllDebugStateSet(DEBUG_STATE state) {
		CDV_INT16U i;
		for( i = 0; i < WORKER_MAX_NUM; i++) {    
			g_threadInfo[i].debug.state = state;
		}
	}
/** @brief  解析入口
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
		
		for( i = 0; i < WORKER_MAX_NUM; i++) {                                   /*判断工人是否已经运行*/
			
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
					//state = DEBUG_PAUSE;//DEBUG_ENABLE;//使能断点
				  AllDebugStateSet(DEBUG_PAUSE);
					ManagerControl(WORKER_ONCE);
				} else {
					AllDebugStateSet(DEBUG_DISABLE);
				}
				
				
//				for( i = 0; i < WORKER_MAX_NUM; i++) {    
//					g_threadInfo[i].debug.state = state;//使能断点
//				}
				
				break;
			}
			case 0x01:
				if(ctrl) {
					pos = *(CDV_INT16U*)(buf + 3);
					//state = ctrl->state;
					//ctrl->state = DEBUG_PAUSE;//失能断点
					//delay_ms(10);//防止冲突
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
					//ctrl->state = DEBUG_PAUSE;//失能断点
					//delay_ms(10);//防止冲突
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
				//ctrl->state = DEBUG_PAUSE;//失能断点
			  //delay_ms(10);//防止冲突
				//err = ClearDebugPoint(ctrl);
			  //ctrl->state = state;
				break;
				
			case 0x04://继续
				AllDebugStateSet(DEBUG_CONTINUE);
					
				//ctrl->state = DEBUG_CONTINUE;
				break;
					
			case 0x05://停止
				AllDebugStateSet(DEBUG_PAUSE);

				//ctrl->state = DEBUG_PAUSE;
				break;
					
			case 0x06://单步
				if(ctrl) {
					ctrl->state = DEBUG_STEP;
				} else {
					err = -1;
				}
				break;
				
			case 0x07://查询断点
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
				
			case 0x08://查询所有断点
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
				
			case 0x09://查询位置
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
	
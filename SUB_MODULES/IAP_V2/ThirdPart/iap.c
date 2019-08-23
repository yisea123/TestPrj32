/**
  ******************************************************************************
  * @file    STM32F4xx_IAP/src/menu.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   This file provides the software which contains the main menu routine.
  *          The main menu gives the options of:
  *             - downloading a new binary file, 
  *             - uploading internal flash memory,
  *             - executing the binary file already loaded 
  *             - disabling the write protection of the Flash sectors where the 
  *               user loads his binary file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/** @addtogroup STM32F4xx_IAP
  * @{
  */

/* Includes ------------------------------------------------------------------*/
//#include "common.h"
#include "flash_if.h"
#include "iap.h"
//#include "ymodem.h"
u8 SerialNo = MAIN_COM;
#define SerialPutString(str) AddTxNoCrc((u8*)str, strlen(str), SerialNo)
#define SerialPutCh(str,len) AddTx((u8*)str, len, SerialNo)
//#define USART_PRINT(str)
#define USART_PRINT(str) USARTSend((u8*)str, strlen(str), MAIN_COM)

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
uint32_t JumpAddress;
__IO uint32_t FlashProtection = 0;
uint8_t tab_1024[1024] =
  {
    0
  };
//uint8_t FileName[FILE_NAME_LENGTH];

/* Private function prototypes -----------------------------------------------*/
void IAP_LoadApp(u32 appxaddr);

/* Private functions ---------------------------------------------------------*/

	
	/**
  * @brief  接收脚本的环境初始化
  *  
  * @param  addr 保存的flash地址
            len 脚本的长度
  *   
  * @retval void
  *
  * @note   g_scriptRecv
  */
void IAP_RecvInit(CDV_INT32U addr , CDV_INT32U len, CDV_INT08U port) {
	CDV_INT08U i;
	g_scriptRecv.doPos = 0;
	//g_scriptRecv.no = 0;
	g_scriptRecv.rxPos = 0;
	g_scriptRecv.totalLen = len;
	g_scriptRecv.tmpLen = 0;
	g_scriptRecv.addr = addr;
	g_scriptRecv.crc = 0xFFFF;
	g_scriptRecv.err = 0;
	g_scriptRecv.port = port;
	
	for (i = 0; i < QUE_NUM; i++) {
		g_scriptRecv.buf[i] = UserMemPtr(CCM_RECV_BUF + i * QUE_LEN);
    g_scriptRecv.len[i] = 0;
	}
	FLASH_If_Init();									//解锁 
  FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间,必须禁止数据缓存
 		
	g_scriptRecv.err = FLASH_If_Erase2(addr, FLASH_Sector_8);
	
  g_cdvStat = CDV_RECV;
}

/**
  * @brief  接收脚本的环境释放
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   g_scriptRecv
  */
void IAP_RecvDeinit(void) {
	CDV_INT32U i ;
	
	g_scriptRecv.doPos = 0;
	//g_scriptRecv.no = 0;
	g_scriptRecv.rxPos = 0;
	g_scriptRecv.totalLen = 0;
	g_scriptRecv.tmpLen = 0;
	g_scriptRecv.addr = 0;
	g_scriptRecv.crc = 0;
	g_scriptRecv.port = 0;
	g_cdvStat = CDV_ONLINE;
	for (i = 0; i < QUE_NUM; i++) {
		g_scriptRecv.buf[i] = NULL;
    g_scriptRecv.len[i] = 0;
	}
	FLASH_DataCacheCmd(ENABLE);	//FLASH擦除结束,开启数据缓存
	FLASH_Lock();//上锁
}


/**
  * @brief  
  * @param  
  * @retval 0 success ; 1 failure
  */
u8 IAP_Flash_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{
	ASSERT(IS_FLASH_ADDRESS(WriteAddr) && !(WriteAddr%4) && !(NumByteToWrite%4));
	
	if (FLASH_If_Write(&WriteAddr, (uint32_t*) pBuffer, (uint16_t) NumByteToWrite/4)  == 0)
	{
		g_scriptRecv.crc = MODBUS_CRC16(pBuffer, NumByteToWrite/4, g_scriptRecv.crc);
		return 0;
	}
	else /* An error occurred while writing to Flash memory */
	{
		g_scriptRecv.crc = g_scriptRecv.crc;
		return 1;
	}
}

/**
  * @brief  接收脚本的控制
  *  
  * @param  void
  *   
  * @retval void
  *
  * @note   g_scriptRecv
  */
void IAP_RecvCtl(CDV_INT32U addr , CDV_INT32U len , CDV_INT08U port) {
	CDV_INT08U start = 0 ;
	CDV_INT32U cnt = 1 ;
	CDV_INT32U cnt_timeout = 3000/*~1ms*/ * 10000;
	CDV_INT32U lastRxPos = 0;
	u8 cmd[10] = {0xF3 , 0x06, 0x00, 0x00};
	if(0 == addr || 0 == len) {
		cmd[2] = 1;
		SerialPutCh(cmd, 3);
		return;
	}
	IAP_RecvInit(addr , len , port);
	//SerialPutString("\r\n接收初始化完成，准备接收");
	SerialPutCh(cmd, 3);
	while(!g_scriptRecv.err) {//检测是否用户开启了FPGA程序下载到flash的拨码开关
	  if(g_scriptRecv.tmpLen + g_scriptRecv.len[g_scriptRecv.doPos] >=  g_scriptRecv.totalLen) {//拨码开关拨到了停止下载FPGA程序的位置			
			if (0 != g_scriptRecv.len[g_scriptRecv.doPos]) {
				g_scriptRecv.err = IAP_Flash_Write(g_scriptRecv.buf[g_scriptRecv.doPos], g_scriptRecv.tmpLen + g_scriptRecv.addr, g_scriptRecv.totalLen - g_scriptRecv.tmpLen);
				g_scriptRecv.tmpLen += g_scriptRecv.len[g_scriptRecv.doPos];//可以比较一下有没有接多
			}
			break;
		} else if(SRP_QUE_HAD) {
			g_scriptRecv.err = IAP_Flash_Write(g_scriptRecv.buf[g_scriptRecv.doPos], g_scriptRecv.tmpLen + g_scriptRecv.addr, g_scriptRecv.len[g_scriptRecv.doPos]);
			//0.22s
			g_scriptRecv.tmpLen += g_scriptRecv.len[g_scriptRecv.doPos];
			g_scriptRecv.len[g_scriptRecv.doPos] = 0;//队列缓存长度清0
			MAX_SELF_ADD(g_scriptRecv.doPos, QUE_NUM);
		} else if(start){
			if(g_scriptRecv.len[g_scriptRecv.rxPos] != lastRxPos) {
				lastRxPos = g_scriptRecv.len[g_scriptRecv.rxPos];
				cnt = 1;
			} else {//超时判断
				cnt ++;
				if (cnt == 0x100000 )
				{
					g_scriptRecv.err = 1;
					break;
				}
			}
		} else if( 0 == start){
			if(0 !=g_scriptRecv.len[g_scriptRecv.rxPos]) {
				start = 1;
			} else {
				if(cnt_timeout-- == 0) {
					g_scriptRecv.err = 1;
					break;
				}
			}
		}
	}
	
	if(g_scriptRecv.err)
	  SerialPutString("\r\n接收出错");
	else
		SerialPutString("\r\n接收成功");
	
	IAP_RecvDeinit();
	
	if(!g_scriptRecv.err) {
		u8 in[3] = {0x00, 0X00, 0X00};
			u8 out[3] = {0x01, 0X01, 0X7F};
			WriteToInLed(in, 3);
			WriteToOutLed(out, 3);
			IAP_LoadApp(APPLICATION_ADDRESS);
			ResetCdv();
	}
}


/**
  * @brief  Download a file via serial port
  * @param  None
  * @retval None
  */
void SerialDownload(u32 fileSize, CDV_INT08U port)
{
  IAP_RecvCtl(APPLICATION_ADDRESS, fileSize, port);
}

/**
  * @brief  发送flash内容
  *  
  * @param  uartNo     串口号
  *   
  * @retval 返回值说明
  *
  * @note   
  */
  
//#define FLASH_SEND_LEN 0x200
void IAP_Send(const CDV_INT32U addr , const CDV_INT32U len, CDV_INT08U uartNo){
	CDV_INT32U sendPt = 0;
	CDV_INT08U* sendBuf = (CDV_INT08U*)addr;
	CDV_INT32U sendLen;
	
	sendLen = 0xEE == uartNo ? 0x1000 : 0x200;
	g_cdvStat = CDV_SEND;
	while(sendPt < len){
		if(sendPt + sendLen > len) {
			//SPI_Flash_Read(sendBuf, addr + sendPt, len - sendPt);
			AddTxNoCrc(sendBuf + sendPt , len - sendPt , uartNo );
		}
		else{
			//SPI_Flash_Read(sendBuf, addr + sendPt, FLASH_SEND_LEN);
			AddTxNoCrc(sendBuf + sendPt , sendLen , uartNo );
		}
		sendPt += sendLen;
	}
	g_cdvStat = CDV_ONLINE;
}
/**
  * @brief  Upload a file via serial port.
  * @param  None
  * @retval None
  */
void SerialUpload(void)
{
   IAP_Send(APPLICATION_ADDRESS, USER_FLASH_END_ADDRESS - APPLICATION_ADDRESS, SerialNo);
}


/**
  * @brief  jump to app
  * @param  None
  * @retval None
  */
void IAP_LoadApp(u32 appxaddr)
{
	 /* Test if user code is programmed starting from address "APPLICATION_ADDRESS" */
	if (((*(__IO uint32_t*)APPLICATION_ADDRESS) & 0x2FFE0000 ) == 0x20000000)
	{
		CDV_INT08C fields[5] = "";
		memset((void*)fields, 0, 3);
	   SPI_Flash_Write((CDV_INT08U*)fields, OTA_ADDR, 3);
		
		/* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (APPLICATION_ADDRESS + 4);
		Jump_To_Application = (pFunction) JumpAddress;
		/* Initialize user application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
		__set_CONTROL(0);
		Jump_To_Application();
	}
	else
	{
		
	}
}		 


/**
  * @brief  Display the Main Menu on HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(u8 opt, u32 fileSize, u8 uartNo)
{
  uint8_t key = 0;

  SerialNo = uartNo;

  if (1)
  {

    if (opt == 0x31)
    {
      /* Download user application in the Flash */
      SerialDownload(fileSize, uartNo);
    }
    else if (opt == 0x32)
    {
      /* Upload user application from the Flash */
      SerialUpload();
    }
    else if (opt == 0x33) /* execute the new program */
    {
			IAP_LoadApp(APPLICATION_ADDRESS);
    }
    else if ((opt == 0x34) && (FlashProtection == 1))
    {
      /* Disable the write protection */
      switch (FLASH_If_DisableWriteProtection())
      {
        case 1:
        {
          SerialPutString("Write Protection disabled...\r\n");
          FlashProtection = 0;
          break;
        }
        case 2:
        {
          SerialPutString("Error: Flash write unprotection failed...\r\n");
          break;
        }
        default:
        {
        }
      }
    }
    else
    {
//      if (FlashProtection == 0)
//      {
//        SerialPutString("Invalid Number ! ==> The number should be either 1, 2 or 3\r");
//      }
//      else
//      {
//        SerialPutString("Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
//      }
			SerialPutString("\r\n======================================================================");
			SerialPutString("\r\n=              (C) COPYRIGHT 2017 CQT Quartz. Co., Ltd.              =");
			SerialPutString("\r\n=                                                                    =");
			SerialPutString("\r\n=  NPC In-Application Programming Application  (Version 1.0.0)       =");
			SerialPutString("\r\n=                                                                    =");
			SerialPutString("\r\n=                                   By NPC Application Team          =");
			SerialPutString("\r\n======================================================================");
			SerialPutString("\r\n\r\n");

			/* Test if any sector of Flash memory where user application will be loaded is write protected */
			if (FLASH_If_GetWriteProtectionStatus() == 0)   
			{
				FlashProtection = 1;
			}
			else
			{
				FlashProtection = 0;
			}
			
			SerialPutString("\r\n================== Main Menu ============================\r\n\n");
			SerialPutString("  Download Image To the STM32F4xx Internal Flash ------- IAP:1 长度\r\n\n");
			SerialPutString("  Upload Image From the STM32F4xx Internal Flash ------- IAP:2\r\n\n");
			SerialPutString("  Execute The New Program ------------------------------ IAP:3\r\n\n");

			if(FlashProtection != 0)
			{
				SerialPutString("  Disable the write protection ------------------------- IAP:4\r\n\n");
			}

			SerialPutString("==========================================================\r\n\n");
			}
  }
}

/**
  * @brief  Online update
  * @param  None
  * @retval None
  */
void CentralizedControl_Ctrl(uint8_t* buf, uint8_t len, uint8_t uartNo)
{
  uint8_t key = buf[0];
  uint8_t opt = buf[1];
  u32 fileSize = *(u32*)(buf + 2);
  SerialNo = uartNo;

  if (0xF3 == key)
  {

    if (opt == 0x0)/* answer */
    {
      
      SerialPutString("OK\r\n");
    }else if (opt == 0x06)/* Download user application in the Flash */
    {
      
      SerialDownload(fileSize, uartNo);
    }
    else if (opt == 0x11)/* Upload user application from the Flash */
    {
      
      SerialUpload();
    }
    else if (opt == 0x3) /* execute the new program */
    {
			u8 in[3] = {0x00, 0X00, 0X00};
			u8 out[3] = {0x01, 0X01, 0X7F};
			WriteToInLed(in, 3);
			WriteToOutLed(out, 3);
			IAP_LoadApp(APPLICATION_ADDRESS);
    }
    else if (opt == 0x4) /* query status */
    {
      if(buf[3] == 0x01) {
				u8 ask[4] = {0xF3, 0X04, 0X00, 0X00};
				SerialPutCh(ask, 4);
			}else{
				u8 ask[4] = {0xF3, 0X04, 0X00, 0X00};
				SerialPutCh(ask, 4);
				delay_ms(500);
				ResetCdv();
			}
    }
    else if ((opt == 0x10) && (FlashProtection == 1))/* Disable the write protection */
    {
      
      switch (FLASH_If_DisableWriteProtection())
      {
        case 1:
        {
          SerialPutString("Write Protection disabled...\r\n");
          FlashProtection = 0;
          break;
        }
        case 2:
        {
          SerialPutString("Error: Flash write unprotection failed...\r\n");
          break;
        }
        default:
        {
        }
      }
    }
  }
}
/**
  * @brief  Display the Start Menu on Main Usart
  * @param  None
  * @retval None
  */

void StartMenu(void)
{
	#define STEP 1000
  uint8_t key = 0;
	u8 i = 0;
	u32 clk, start, des;
	start = ReadClock1ms();
	des = 0;
	USART_PRINT("\r\n======================================================================");
	USART_PRINT("\r\n=              (C) COPYRIGHT 2017 CQT Quartz. Co., Ltd.              =");
	USART_PRINT("\r\n=                                                                    =");
	USART_PRINT("\r\n=  NPC In-Application Programming Application  (Version 1.0.0)       =");
	USART_PRINT("\r\n=                                                                    =");
	USART_PRINT("\r\n=                                   By NPC Application Team          =");
	USART_PRINT("\r\n======================================================================");
	USART_PRINT("\r\n\r\n");
	USART_PRINT("\r\n================== Ready To Load Application =========================\r\n\n");
	while(1)
	{
		clk = CalcCount(ReadClock1ms(), start);
		if(clk > des)
		{
			des += STEP;
			
			if(des > STEP * 2)
			{
				USART_PRINT("\r\n====================== Start Loading =================================\r\n\n");
				{
					u8 in[3] = {0x00, 0X00, 0X00};
					u8 out[3] = {0x01, 0X01, 0X7F};
					WriteToInLed(in, 3);
					WriteToOutLed(out, 3);
				}
				IAP_LoadApp(APPLICATION_ADDRESS);
				while(1)
				{
					{
					u8 in[3] = {0x77, 0X51, 0X77};
					u8 out[3] = {0x31, 0X51, 0X57};
					WriteToInLed(in, 3);
					WriteToOutLed(out, 3);
					LED3 = LED_ON;
					LED1 = LED_OFF;
				  }
					DelayUS(1000000);
					{
					u8 in[3] = {0x00, 0X00, 0X00};
					u8 out[3] = {0x00, 0X00, 0X00};
					WriteToInLed(in, 3);
					WriteToOutLed(out, 3);
					LED3 = LED_OFF;
					LED1 = LED_ON;
					DelayUS(1000000);
				  }
				}
			}
			USART_PRINT("=");
      WriteToInLed(RomanNumerals[i], 3);
			i++;
		}
		
		if(g_uartRx.QUEUE.rxLen[g_uartRx.rxPos] && (0 == strncmp((CDV_INT08C*)g_uartRx.QUEUE.rxBuf[g_uartRx.rxPos],"IAP UPDATE",10)))
		{
			u8 in[3] = {0x24, 0X24, 0X3C};
			WriteToInLed(in, 3);
			break;
		}
	}
		
		
  USART_PRINT("\r\n================== Exit Load Application =============================\r\n\n");
	USART_PRINT("\r\n\r\n");

	/* Test if any sector of Flash memory where user application will be loaded is write protected */
	if (FLASH_If_GetWriteProtectionStatus() == 0)   
	{
		FlashProtection = 1;
	}
	else
	{
		FlashProtection = 0;
	}
	
	USART_PRINT("\r\n================== Main Menu ============================\r\n\n");
	USART_PRINT("  Download Image To the STM32F4xx Internal Flash ------- IAP:1 长度\r\n\n");
	USART_PRINT("  Upload Image From the STM32F4xx Internal Flash ------- IAP:2\r\n\n");
	USART_PRINT("  Execute The New Program ------------------------------ IAP:3\r\n\n");

	if(FlashProtection != 0)
	{
		USART_PRINT("  Disable the write protection ------------------------- IAP:4\r\n\n");
	}

	USART_PRINT("==========================================================\r\n\n");
}

/**
  * @}
  */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/

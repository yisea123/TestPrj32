/**
  ******************************************************************************
  * @file    STM32F4xx_IAP/inc/menu.h 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    10-October-2011
  * @brief   This file provides all the headers of the menu functions.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IAP_H
#define __IAP_H

/* Includes ------------------------------------------------------------------*/
#include "flash_if.h"
#include "cdv_include.h"

/* Private variables ---------------------------------------------------------*/
typedef  void (*pFunction)(void);

//extern u8 SerialNo ;
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Main_Menu(u8 opt, u32 fileSize, u8 uartNo);
void StartMenu(void);
void CentralizedControl_Ctrl(uint8_t* buf, uint8_t len, uint8_t uartNo);

#endif  /* __IAP_H */

/*******************(C)COPYRIGHT 2011 STMicroelectronics *****END OF FILE******/

/**
  ******************************************************************************
  * @file    stm324x9i_eval.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_MCU_UART_H
#define __BSP_MCU_UART_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "STM32_BD_F429_V01.h"
#include "Common_def.h"


typedef enum 
{
  EN_MCU_UART_1 = 0,
  EN_MCU_UART_2,
  EN_MCU_UART_3,
  EN_MCU_UART_MAX
}En_MCU_UART_PORT;  
 


COMMON_StatusTypeDef BSP_UartInit(En_MCU_UART_PORT enUartPort,uint32_t u32Baud);
uint8_t BSP_Uart1SendChar(uint8_t u8Ch);

/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM324X9I_EVAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    hal_uart.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-09-02 
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_UART_H
#define __HAL_UART_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "bsp_mcu_uart.h"
#include "common_all_macro_def.h"
#include "common_def.h"

/* Exported macros -----------------------------------------------------------*/
#define R_BUF_SIZE  1024
#define T_BUF_SIZE  1024


/* Exported types ------------------------------------------------------------*/
typedef enum {
    UART_1 = 0,
    UART_2,
    UART_3,
    UART_4,
    UART_5,
    UART_6,
    UART_7,
    UART_MAX,
}UART_PORT_E;



typedef struct HalUart{
   int (*HAL_UartInit)(void);
}HAL_UART_S;


typedef struct{
    uint32_t R_Front;
    uint32_t R_Rear;
    uint32_t R_Count;
    uint32_t R_Overflow;
    uint8_t R_Buf[R_BUF_SIZE];
    uint32_t T_Front;
    uint32_t T_Rear;
    uint32_t T_Count;
    uint32_t T_Disabled;
    uint8_t T_Buf[T_BUF_SIZE];
}Uart_Queue_S;

typedef struct {
    Uart_Queue_S PortQueue[UART_MAX];
}Uart_Port_S;

typedef void ( *TimerCb) (uint32_t u32StTimer,uint32_t u32TimerID);

/* Exported variables --------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/


/* Exported functions ------------------------------------------------------- */ 

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

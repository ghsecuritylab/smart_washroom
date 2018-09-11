/**
  ******************************************************************************
  * @file    hal_led.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26 
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_LED_H
#define __HAL_LED_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "bsp_dev_led.h"
#include "common_all_macro_def.h"
/* EVAL includes component */

/* FatFs includes component */

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void HAL_LedInit(void);
void HAL_LedOn(void);
void HAL_LedOff(void);
void HAL_LedTrogle(void);
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    hal_lcd.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26 
  * @brief   Header for hal_lcd.c file
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_LCD_H
#define __HAL_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

   
/* Includes ------------------------------------------------------------------*/
#include "common_def.h"
   
typedef struct
{
    const int8_t*   ( * const Name      ) ( void );     //Panel Name
    uint16_t        ( * const HStart    ) ( void );     //H start  
}St_PANEL_FUNC;
/* EVAL includes component */

/* FatFs includes component */

/* Exported types ------------------------------------------------------------*/


/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 

void HAL_LcdInit(void);     



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    hal_board_init.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26 
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_INIT_H
#define __HAL_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include "common_def.h"

typedef enum
{
	EN_PRE_INIT_STAGE_1 = 0,
	EN_PRE_INIT_STAGE_2,
	EN_PRE_INIT_STAGE_3,
	EN_PRE_INIT_STAGE_4,
	EN_PRE_INIT_STAGE_DONE,
}En_PRE_INIT_STATGE;
/* EVAL includes component */

/* FatFs includes component */

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/
En_PRE_INIT_STATGE g_enPreInitStage;
bool_t g_bPreInitDone;
/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void HAL_HardWareInit(void);
     
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

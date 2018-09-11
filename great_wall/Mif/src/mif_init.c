/**
  ******************************************************************************
  * @file    hal_board_init.c 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "hal_init.h"

#include "bsp_dev_init.h"
#include "hal_led.h"
//#include "hal_sdram.h"
//#include "hal_touch.h"
//#include "hal_lcd.h"
#include "serial_debug_log.h"


/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None 
  * @retval None
  */
void HAL_HardWareInit(void)
{
    /* Configure init clock */
    BSP_BoardInit();
    LOG(LEVEL_INFO,"BSP_BoardInit\n");
    
    /* Configure LED */
    HAL_LedInit();
    LOG(LEVEL_INFO,"HAL_LedInit\n");
    
    /* Initialize the SDRAM */
    //HAL_SdramInit();
    LOG(LEVEL_INFO,"HAL_SdramInit\n");
    
    /* Initialize the Touch screen */
    //HAL_TouchInit(); 
    LOG(LEVEL_INFO,"HAL_TouchInit\n");    
    
    /* Initialize the Lcd */
    //HAL_LcdInit();
    LOG(LEVEL_INFO,"HAL_LcdInit\n");

  
}

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None 
  * @retval None
  */
bool_t HAL_PreInitProcess(void)
{
    bool_t bPreInitDone = FALSE;
	switch(g_enPreInitStage)
    {
        case EN_PRE_INIT_STAGE_1:
            break;
        case EN_PRE_INIT_STAGE_2:
            break;
        case EN_PRE_INIT_STAGE_3:
            break;
        case EN_PRE_INIT_STAGE_4:
            break;
        case EN_PRE_INIT_STAGE_DONE:
            bPreInitDone = TRUE;
            break;
    }
    if(g_enPreInitStage < EN_PRE_INIT_STAGE_DONE)
    {
        g_enPreInitStage++;
    }
    return bPreInitDone;
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

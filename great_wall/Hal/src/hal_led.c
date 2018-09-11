/**
  ******************************************************************************
  * @file    hal_led.c 
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
#include "hal_led.h"


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
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
void HAL_LedInit(void)
{
    BSP_LED_Init(LED1);
}

/**
  * @brief  
  * @param  None 
  * @retval None
  */
void HAL_LedOn(void)
{
    BSP_LED_On(LED1); 
}

/**
  * @brief  
  * @param  None 
  * @retval None
  */
void HAL_LedOff(void)
{
    BSP_LED_Off(LED1); 
}

void HAL_LedTrogle(void)
{
    BSP_LED_Toggle(LED1);
}

#if DEMO_TEST_LED
void HAL_DEMO_LED(void)
{
    HAL_LedTrogle();
}
#endif
/**
  * @}
  */

/**
  * @}
  */

/****************************************************************END OF FILE****/

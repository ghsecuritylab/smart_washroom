/**
  ******************************************************************************
  * @file    bsp_led.h
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-08-27
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DEV_LED_H
#define __BSP_DEV_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "STM32_BD_F429_V01.h"

/** @addtogroup BSP
  * @{
  */

typedef enum
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
}Led_TypeDef;


/** @defgroup STM324x9I_EVAL_LOW_LEVEL_LED STM324x9I EVAL LOW LEVEL LED
  * @{
  */
#define LEDn                             2

#define LED1_PIN                         BOARD_LED1_PIN
#define LED1_GPIO_PORT                   BOARD_LED1_PORT
#define LED1_GPIO_CLK_ENABLE()           BOARD_LED1_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()          BOARD_LED1_CLK_DISABLE()


#define LED2_PIN                         BOARD_LED1_PIN
#define LED2_GPIO_PORT                   BOARD_LED1_PORT
#define LED2_GPIO_CLK_ENABLE()           BOARD_LED1_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()          BOARD_LED1_CLK_DISABLE()

#define LED3_PIN                         BOARD_LED3_PIN
#define LED3_GPIO_PORT                   BOARD_LED3_PORT
#define LED3_GPIO_CLK_ENABLE()           BOARD_LED3_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()          BOARD_LED3_CLK_DISABLE()

#define LED4_PIN                         BOARD_LED4_PIN
#define LED4_GPIO_PORT                   BOARD_LED4_PORT
#define LED4_GPIO_CLK_ENABLE()           BOARD_LED4_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()          BOARD_LED4_CLK_DISABLE()


#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LED1_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) LED2_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 2) LED3_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 3) LED4_GPIO_CLK_ENABLE(); \
                                            }while(0)

#define LEDx_GPIO_CLK_DISABLE(__INDEX__)  do{if((__INDEX__) == 0) LED1_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 1) LED1_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 2) LED1_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 3) LED2_GPIO_CLK_DISABLE(); \
                                             }while(0)


/** @defgroup STM324x9I_EVAL_LOW_LEVEL_Exported_Functions STM324x9I EVAL LOW LEVEL Exported Functions
  * @{
  */
void BSP_LED_Init(Led_TypeDef Led);
void BSP_LED_On(Led_TypeDef Led);
void BSP_LED_Off(Led_TypeDef Led);
void BSP_LED_Toggle(Led_TypeDef Led);

#if DEMO_TEST_LED
void HAL_DEMO_LED(void);
#endif

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

#endif //ifndef __BSP_LED_H

/****************************************************************END OF FILE****/

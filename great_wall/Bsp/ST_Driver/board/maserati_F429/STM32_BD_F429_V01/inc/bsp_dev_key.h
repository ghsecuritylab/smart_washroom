/**
  ******************************************************************************
  * @file    bsp_dev_key.h
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
#ifndef __BSP_DEV_KEY_H
#define __BSP_DEV_KEY_H

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
  BUTTON_WAKEUP = 0,
  BUTTON_TAMPER = 1,
  BUTTON_KEY = 2
}Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
}ButtonMode_TypeDef;


#define BUTTONn                             1 

/**
  * @brief Wakeup push-button
  */
#define WAKEUP_BUTTON_PIN                   GPIO_PIN_0
#define WAKEUP_BUTTON_GPIO_PORT             GPIOA
#define WAKEUP_BUTTON_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()  
#define WAKEUP_BUTTON_GPIO_CLK_DISABLE()    __GPIOA_CLK_DISABLE()
#define WAKEUP_BUTTON_EXTI_IRQn             EXTI0_IRQn 

/**
  * @brief Tamper push-button
  */
#define TAMPER_BUTTON_PIN                    GPIO_PIN_13
#define TAMPER_BUTTON_GPIO_PORT              GPIOC
#define TAMPER_BUTTON_GPIO_CLK_ENABLE()      __GPIOC_CLK_ENABLE()  
#define TAMPER_BUTTON_GPIO_CLK_DISABLE()     __GPIOC_CLK_DISABLE()
#define TAMPER_BUTTON_EXTI_IRQn              EXTI15_10_IRQn

/**
  * @brief Key push-button
  */
#define KEY_BUTTON_PIN                       BOARD_KEY1_PIN
#define KEY_BUTTON_GPIO_PORT                 BOARD_KEY1_PORT
#define KEY_BUTTON_GPIO_CLK_ENABLE()         BOARD_KEY1_CLK_ENABLE()  
#define KEY_BUTTON_GPIO_CLK_DISABLE()        BOARD_KEY1_CLK_DISABLE()
#define KEY_BUTTON_EXTI_IRQn                 BOARD_KEY1_EXTI_IRQn

#define BUTTONx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) WAKEUP_BUTTON_GPIO_CLK_ENABLE(); else \
                                               if((__INDEX__) == 1) TAMPER_BUTTON_GPIO_CLK_ENABLE(); else \
                                               if ((__INDEX__) == 2) KEY_BUTTON_GPIO_CLK_ENABLE(); \
                                               }while(0)
#define BUTTONx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) WAKEUP_BUTTON_GPIO_CLK_DISABLE(); else \
                                               if((__INDEX__) == 1) TAMPER_BUTTON_GPIO_CLK_DISABLE(); else \
                                               if ((__INDEX__) == 2) KEY_BUTTON_GPIO_CLK_DISABLE(); \
                                               }while(0)




void BSP_PB_Init(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t BSP_PB_GetState(Button_TypeDef Button);



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

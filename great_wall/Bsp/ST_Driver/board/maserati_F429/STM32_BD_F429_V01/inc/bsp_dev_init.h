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
#ifndef __BSP_INIT_H
#define __BSP_INIT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
   


/* Exported constant IO ------------------------------------------------------*/
#define IO_I2C_ADDRESS                   0x84 
#define TS_I2C_ADDRESS                   0x82
#define TS3510_I2C_ADDRESS               0x80
#define EXC7200_I2C_ADDRESS              0x08                                                         
#define CAMERA_I2C_ADDRESS               0x60
#define AUDIO_I2C_ADDRESS                0x34
#define EEPROM_I2C_ADDRESS_A01           0xA0
#define EEPROM_I2C_ADDRESS_A02           0xA6  
/* I2C clock speed configuration (in Hz) 
   WARNING: 
   Make sure that this define is not already declared in other files (ie. 
   stm324x9I_eval.h file). It can be used in parallel by other modules. */
#ifndef BSP_I2C_SPEED
 #define BSP_I2C_SPEED                        100000
#endif /* BSP_I2C_SPEED */

/* User can use this section to tailor I2Cx/I2Cx instance used and associated 
   resources */
/* Definition for I2Cx clock resources */
#define EVAL_I2Cx                             I2C1
#define EVAL_I2Cx_CLK_ENABLE()                __I2C1_CLK_ENABLE()
#define EVAL_DMAx_CLK_ENABLE()                __DMA1_CLK_ENABLE()
#define EVAL_I2Cx_SCL_SDA_GPIO_CLK_ENABLE()   __GPIOB_CLK_ENABLE()

#define EVAL_I2Cx_FORCE_RESET()               __I2C1_FORCE_RESET()
#define EVAL_I2Cx_RELEASE_RESET()             __I2C1_RELEASE_RESET()
   
/* Definition for I2Cx Pins */
#define EVAL_I2Cx_SCL_PIN                     GPIO_PIN_6
#define EVAL_I2Cx_SCL_SDA_GPIO_PORT           GPIOB
#define EVAL_I2Cx_SCL_SDA_AF                  GPIO_AF4_I2C1
#define EVAL_I2Cx_SDA_PIN                     GPIO_PIN_9

/* I2C interrupt requests */
#define EVAL_I2Cx_EV_IRQn                     I2C1_EV_IRQn
#define EVAL_I2Cx_ER_IRQn                     I2C1_ER_IRQn


  

/** @defgroup STM324x9I_EVAL_LOW_LEVEL_Exported_Functions STM324x9I EVAL LOW LEVEL Exported Functions
  * @{
  */
uint32_t BSP_GetVersion(void);  
void BSP_BoardInit(void);


/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM324X9I_EVAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

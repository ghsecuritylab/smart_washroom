/**
  ******************************************************************************
  * @file    STM32_BD_F429_V01.h
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-08-26
  * @brief
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_BD_F429_V01_H
#define __STM32_BD_F429_V01_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common_all_macro_def.h"
#include "stm32f4xx_hal.h"

#ifdef G_CONFIG_BOARD_TYPE_MACAN_V01_ST

/**
 * @brief STM324x9I EVAL BSP Driver version number V0.0.1
   */
#define __STM32_BD_F429_V01_VERSION_MAIN   (0x00) /*!< [31:24] main version */
#define __STM32_BD_F429_V01_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version */
#define __STM32_BD_F429_V01_VERSION_SUB2   (0x01) /*!< [15:8]  sub2 version */
#define __STM32_BD_F429_V01_VERSION_RC     (0x00) /*!< [7:0]  release candidate */
#define __BOARD_TYPE_VERSION                 ((__STM32_BD_F429_V01_VERSION_MAIN << 24)\
                                             |(__STM32_BD_F429_V01_VERSION_SUB1 << 16)\
                                             |(__STM32_BD_F429_V01_VERSION_SUB2 << 8 )\
                                             |(__STM32_BD_F429_V01_VERSION_RC))

/**
 * GPIO CONFIG
 */
#define BOARD_LED1_PORT             GPIOG
#define BOARD_LED1_PIN              GPIO_PIN_7
#define BOARD_LED1_CLK_ENABLE()     __GPIOG_CLK_ENABLE()
#define BOARD_LED1_CLK_DISABLE()    __GPIOG_CLK_DISABLE()

#define BOARD_LED2_PORT             GPIOD
#define BOARD_LED2_PIN              GPIO_PIN_12
#define BOARD_LED2_CLK_ENABLE()     __GPIOD_CLK_ENABLE()
#define BOARD_LED2_CLK_DISABLE()    __GPIOD_CLK_DISABLE()

#define BOARD_LED3_PORT             GPIOG
#define BOARD_LED3_PIN              GPIO_PIN_7
#define BOARD_LED3_CLK_ENABLE()     __GPIOD_CLK_ENABLE()
#define BOARD_LED3_CLK_DISABLE()    __GPIOD_CLK_DISABLE()

#define BOARD_LED4_PORT             GPIOG
#define BOARD_LED4_PIN              GPIO_PIN_7
#define BOARD_LED4_CLK_ENABLE()     __GPIOD_CLK_ENABLE()
#define BOARD_LED4_CLK_DISABLE()    __GPIOD_CLK_DISABLE()

#define BOARD_KEY1_PORT             GPIOI
#define BOARD_KEY1_PIN              GPIO_PIN_8
#define BOARD_KEY1_CLK_ENABLE()     __GPIOI_CLK_ENABLE()
#define BOARD_KEY1_CLK_DISABLE()    __GPIOI_CLK_DISABLE()
#define BOARD_KEY1_EXTI_IRQn        EXTI15_10_IRQn

//Serial Config --->bsp_uart.h

//Software I2c
/**
 * PC13/I2C1_SCL
 * PB2/I2C1_SDA
 */
#define BOARD_SW_I2C1_SCL_PORT             GPIOC
#define BOARD_SW_I2C1_SCL_PIN              GPIO_PIN_13
#define BOARD_SW_I2C1_SCL_CLK_ENABLE()     __GPIOC_CLK_ENABLE()
#define BOARD_SW_I2C1_SCL_CLK_DISABLE()    __GPIOC_CLK_DISABLE()

#define BOARD_SW_I2C1_SDA_PORT             GPIOB
#define BOARD_SW_I2C1_SDA_PIN              GPIO_PIN_2
#define BOARD_SW_I2C1_SDA_CLK_ENABLE()     __GPIOB_CLK_ENABLE()
#define BOARD_SW_I2C1_SDA_CLK_DISABLE()    __GPIOB_CLK_DISABLE()

#define BOARD_SW_I2C2_SCL_PORT             GPIOC
#define BOARD_SW_I2C2_SCL_PIN              GPIO_PIN_13
#define BOARD_SW_I2C2_SCL_CLK_ENABLE()     __GPIOC_CLK_ENABLE()
#define BOARD_SW_I2C2_SCL_CLK_DISABLE()    __GPIOC_CLK_DISABLE()

#define BOARD_SW_I2C2_SDA_PORT             GPIOB
#define BOARD_SW_I2C2_SDA_PIN              GPIO_PIN_2
#define BOARD_SW_I2C2_SDA_CLK_ENABLE()     __GPIOB_CLK_ENABLE()
#define BOARD_SW_I2C2_SDA_CLK_DISABLE()    __GPIOB_CLK_DISABLE()

#define BOARD_SW_I2C3_SCL_PORT             GPIOC
#define BOARD_SW_I2C3_SCL_PIN              GPIO_PIN_13
#define BOARD_SW_I2C3_SCL_CLK_ENABLE()     __GPIOC_CLK_ENABLE()
#define BOARD_SW_I2C3_SCL_CLK_DISABLE()    __GPIOC_CLK_DISABLE()

#define BOARD_SW_I2C3_SDA_PORT             GPIOB
#define BOARD_SW_I2C3_SDA_PIN              GPIO_PIN_2
#define BOARD_SW_I2C3_SDA_CLK_ENABLE()     __GPIOB_CLK_ENABLE()
#define BOARD_SW_I2C3_SDA_CLK_DISABLE()    __GPIOB_CLK_DISABLE()

#define BOARD_SW_I2C4_SCL_PORT             GPIOC
#define BOARD_SW_I2C4_SCL_PIN              GPIO_PIN_13
#define BOARD_SW_I2C4_SCL_CLK_ENABLE()     __GPIOC_CLK_ENABLE()
#define BOARD_SW_I2C4_SCL_CLK_DISABLE()    __GPIOC_CLK_DISABLE()

#define BOARD_SW_I2C4_SDA_PORT             GPIOB
#define BOARD_SW_I2C4_SDA_PIN              GPIO_PIN_2
#define BOARD_SW_I2C4_SDA_CLK_ENABLE()     __GPIOB_CLK_ENABLE()
#define BOARD_SW_I2C4_SDA_CLK_DISABLE()    __GPIOB_CLK_DISABLE()

/**
 * defined mcu
 */
#define MCU_UART1_DEFINED                   1
#define MCU_UART2_DEFINED                   0
#define MCU_UART3_DEFINED                   0

#define MCU_UART1_BAUD                      9600    //no use
#define MCU_UART2_BAUD                      9600    //no use
#define MCU_UART3_BAUD                      9600    //no use

#endif



#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Grugbai *****END OF FILE****/

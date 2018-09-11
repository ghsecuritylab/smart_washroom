/**
  ******************************************************************************
  * @file    bsp_dev_i2c_gpio.h
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
#ifndef __BSP_DEV_I2C_GPIO_H
#define __BSP_DEV_I2C_GPIO_H

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
  SW_I2C1 = 0,
  SW_I2C2 = 1,
  SW_I2C3 = 2,
  SW_I2C4 = 3
}SW_I2C_Typedef;

#define SW_I2C_NUM                             4
/**
 * PC13/I2C1_SCL
 * PB2/I2C1_SDA
 */
//I2C1
#define SW_I2C1_SCL_PIN                         BOARD_SW_I2C1_SCL_PIN
#define SW_I2C1_SCL_GPIO_PORT                   BOARD_SW_I2C1_SCL_PORT
#define SW_I2C1_SCL_GPIO_CLK_ENABLE()           BOARD_SW_I2C1_SCL_CLK_ENABLE()  
#define SW_I2C1_SCL_GPIO_CLK_DISABLE()          BOARD_SW_I2C1_SCL_CLK_DISABLE() 

#define SW_I2C1_SDA_PIN                         BOARD_SW_I2C1_SDA_PIN
#define SW_I2C1_SDA_GPIO_PORT                   BOARD_SW_I2C1_SDA_PORT
#define SW_I2C1_SDA_GPIO_CLK_ENABLE()           BOARD_SW_I2C1_SDA_CLK_ENABLE()  
#define SW_I2C1_SDA_GPIO_CLK_DISABLE()          BOARD_SW_I2C1_SDA_CLK_DISABLE() 

//I2C2
#define SW_I2C2_SCL_PIN                         BOARD_SW_I2C1_SCL_PIN
#define SW_I2C2_SCL_GPIO_PORT                   BOARD_SW_I2C1_SCL_PORT
#define SW_I2C2_SCL_GPIO_CLK_ENABLE()           BOARD_SW_I2C1_SCL_CLK_ENABLE()  
#define SW_I2C2_SCL_GPIO_CLK_DISABLE()          BOARD_SW_I2C1_SCL_CLK_DISABLE() 

#define SW_I2C2_SDA_PIN                         BOARD_SW_I2C1_SDA_PIN
#define SW_I2C2_SDA_GPIO_PORT                   BOARD_SW_I2C1_SDA_PORT
#define SW_I2C2_SDA_GPIO_CLK_ENABLE()           BOARD_SW_I2C1_SDA_CLK_ENABLE()  
#define SW_I2C2_SDA_GPIO_CLK_DISABLE()          BOARD_SW_I2C1_SDA_CLK_DISABLE() 

//I2C3
#define SW_I2C3_SCL_PIN                         BOARD_SW_I2C1_SCL_PIN
#define SW_I2C3_SCL_GPIO_PORT                   BOARD_SW_I2C1_SCL_PORT
#define SW_I2C3_SCL_GPIO_CLK_ENABLE()           BOARD_SW_I2C1_SCL_CLK_ENABLE()  
#define SW_I2C3_SCL_GPIO_CLK_DISABLE()          BOARD_SW_I2C1_SCL_CLK_DISABLE() 

#define SW_I2C3_SDA_PIN                         BOARD_SW_I2C1_SDA_PIN
#define SW_I2C3_SDA_GPIO_PORT                   BOARD_SW_I2C1_SDA_PORT
#define SW_I2C3_SDA_GPIO_CLK_ENABLE()           BOARD_SW_I2C1_SDA_CLK_ENABLE()  
#define SW_I2C3_SDA_GPIO_CLK_DISABLE()          BOARD_SW_I2C1_SDA_CLK_DISABLE() 

//I2C4
#define SW_I2C4_SCL_PIN                         BOARD_SW_I2C1_SCL_PIN
#define SW_I2C4_SCL_GPIO_PORT                   BOARD_SW_I2C1_SCL_PORT
#define SW_I2C4_SCL_GPIO_CLK_ENABLE()           BOARD_SW_I2C1_SCL_CLK_ENABLE()  
#define SW_I2C4_SCL_GPIO_CLK_DISABLE()          BOARD_SW_I2C1_SCL_CLK_DISABLE() 

#define SW_I2C4_SDA_PIN                         BOARD_SW_I2C1_SDA_PIN
#define SW_I2C4_SDA_GPIO_PORT                   BOARD_SW_I2C1_SDA_PORT
#define SW_I2C4_SDA_GPIO_CLK_ENABLE()           BOARD_SW_I2C1_SDA_CLK_ENABLE()  
#define SW_I2C4_SDA_GPIO_CLK_DISABLE()          BOARD_SW_I2C1_SDA_CLK_DISABLE() 

#define SW_I2Cx_SCL_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) BOARD_SW_I2C1_SCL_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) BOARD_SW_I2C2_SCL_CLK_ENABLE(); else \
                                            if((__INDEX__) == 2) BOARD_SW_I2C3_SCL_CLK_ENABLE(); else \
                                            if((__INDEX__) == 3) BOARD_SW_I2C4_SCL_CLK_ENABLE(); \
                                            }while(0)

#define SW_I2Cx_SCL_CLK_DISABLE(__INDEX__)  do{if((__INDEX__) == 0) SW_I2C1_SCL_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 1) SW_I2C2_SCL_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 2) SW_I2C3_SCL_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 3) SW_I2C4_SCL_GPIO_CLK_DISABLE(); \
                                             }while(0)

#define SW_I2Cx_SDA_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) SW_I2C1_SDA_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) SW_I2C2_SDA_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 2) SW_I2C3_SDA_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 3) SW_I2C4_SDA_GPIO_CLK_ENABLE(); \
                                            }while(0)

#define SW_I2Cx_SDA_CLK_DISABLE(__INDEX__)  do{if((__INDEX__) == 0) SW_I2C1_SDA_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 1) SW_I2C2_SDA_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 2) SW_I2C3_SDA_GPIO_CLK_DISABLE(); else \
                                             if((__INDEX__) == 3) SW_I2C4_SDA_GPIO_CLK_DISABLE(); \
                                             }while(0)



void BSP_I2CGpioInit(SW_I2C_Typedef i2cType);
void BSP_SetSclHigh(SW_I2C_Typedef i2cType);
void BSP_SetSclLow(SW_I2C_Typedef i2cType);
void BSP_SetSdaHigh(SW_I2C_Typedef i2cType);
void BSP_SetSdaLow(SW_I2C_Typedef i2cType);
uint8_t BSP_SetSclRead(SW_I2C_Typedef i2cType);
uint8_t BSP_SetSdaRead(SW_I2C_Typedef i2cType);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif //ifndef __BSP_LED_H

/****************************************************************END OF FILE****/

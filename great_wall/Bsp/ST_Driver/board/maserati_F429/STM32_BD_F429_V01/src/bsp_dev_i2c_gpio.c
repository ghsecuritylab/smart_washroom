/**
  ******************************************************************************
  * @file    bsp_dev_i2c_gpio.c
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-08-27
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *     V0.01   Init    baixiaodong
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_dev_i2c_gpio.h"


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
GPIO_TypeDef* GPIO_PORT_SCL[SW_I2C_NUM] = {
                                        SW_I2C1_SCL_GPIO_PORT,
                                        SW_I2C2_SCL_GPIO_PORT,
                                        SW_I2C3_SCL_GPIO_PORT,
                                        SW_I2C4_SCL_GPIO_PORT,
                                    };

const uint16_t GPIO_PIN_SCL[SW_I2C_NUM] = {
                                        SW_I2C1_SCL_PIN,
                                        SW_I2C2_SCL_PIN,
                                        SW_I2C3_SCL_PIN,
                                        SW_I2C4_SCL_PIN,
                                    };

GPIO_TypeDef* GPIO_PORT_SDA[SW_I2C_NUM] = {
                                        SW_I2C1_SDA_GPIO_PORT,
                                        SW_I2C2_SDA_GPIO_PORT,
                                        SW_I2C3_SDA_GPIO_PORT,
                                        SW_I2C4_SDA_GPIO_PORT,
                                    };

const uint16_t GPIO_PIN_SDA[SW_I2C_NUM] = {
                                        SW_I2C1_SDA_PIN,
                                        SW_I2C2_SDA_PIN,
                                        SW_I2C3_SDA_PIN,
                                        SW_I2C4_SDA_PIN,
                                    };
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
void BSP_I2CGpioInit(SW_I2C_Typedef i2cType)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    /* Enable the GPIO_LED clock */
    SW_I2Cx_SCL_CLK_ENABLE(i2cType);
    SW_I2Cx_SDA_CLK_ENABLE(i2cType);

    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin = GPIO_PIN_SCL[i2cType];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIO_PORT_SCL[i2cType], &GPIO_InitStruct);

    /* Configure the GPIO_LED pin */
    GPIO_InitStruct.Pin = GPIO_PIN_SDA[i2cType];
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

    HAL_GPIO_Init(GPIO_PORT_SDA[i2cType], &GPIO_InitStruct);

}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
void BSP_SetSclHigh(SW_I2C_Typedef i2cType)
{
    HAL_GPIO_WritePin(GPIO_PORT_SCL[i2cType],GPIO_PIN_SCL[i2cType],GPIO_PIN_SET);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
void BSP_SetSclLow(SW_I2C_Typedef i2cType)
{
    HAL_GPIO_WritePin(GPIO_PORT_SCL[i2cType],GPIO_PIN_SCL[i2cType],GPIO_PIN_RESET);
}


/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
void BSP_SetSdaHigh(SW_I2C_Typedef i2cType)
{
    HAL_GPIO_WritePin(GPIO_PORT_SDA[i2cType],GPIO_PIN_SDA[i2cType],GPIO_PIN_SET);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
void BSP_SetSdaLow(SW_I2C_Typedef i2cType)
{
    HAL_GPIO_WritePin(GPIO_PORT_SDA[i2cType],GPIO_PIN_SDA[i2cType],GPIO_PIN_RESET);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
uint8_t BSP_SetSclRead(SW_I2C_Typedef i2cType)
{
    return HAL_GPIO_ReadPin(GPIO_PORT_SCL[i2cType],GPIO_PIN_SCL[i2cType]);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
uint8_t BSP_SetSdaRead(SW_I2C_Typedef i2cType)
{
    return HAL_GPIO_ReadPin(GPIO_PORT_SDA[i2cType],GPIO_PIN_SDA[i2cType]);
}

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

/****************************************************************END OF FILE****/

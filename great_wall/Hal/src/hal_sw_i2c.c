/**
  ******************************************************************************
  * @file    bsp_sw_i2c.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-08-27
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "hal_sw_i2c.h"


/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void HAL_I2C_SCL_H(SW_I2C_Typedef i2cType);
static void HAL_I2C_SCL_L(SW_I2C_Typedef i2cType);
static void HAL_I2C_SDA_H(SW_I2C_Typedef i2cType);
static void HAL_I2C_SDA_L(SW_I2C_Typedef i2cType);
static uint8_t HAL_I2C_SCL_READ(SW_I2C_Typedef i2cType);
static uint8_t HAL_I2C_SDA_READ(SW_I2C_Typedef i2cType);

/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO
*/

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
void HAL_InitSWI2C(SW_I2C_Typedef i2cType)
{
    BSP_I2CGpioInit(i2cType);

    /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	HAL_SWI2C_Stop(i2cType);

}
/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < 30; i++);
}

/**
  * @brief      i2c start 
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
void HAL_SWI2C_Start(SW_I2C_Typedef i2cType)
{
	/** 
     * 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号
     */
	HAL_I2C_SDA_H(i2cType);
	HAL_I2C_SCL_H(i2cType);
	i2c_Delay();
	HAL_I2C_SDA_L(i2cType);
	i2c_Delay();
	
	HAL_I2C_SCL_L(i2cType);
	i2c_Delay();
}

/**
  * @brief      i2c stop 
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
void HAL_SWI2C_Stop(SW_I2C_Typedef i2cType)
{
	/** 
     * 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 
     */
	HAL_I2C_SDA_L(i2cType);
	HAL_I2C_SCL_H(i2cType);
	i2c_Delay();
	HAL_I2C_SDA_H(i2cType);
	i2c_Delay();
}

/**
  * @brief      CPU向I2C总线设备发送8bit数据
  * @param  
  * @InPara     SW_I2C_Typedef i2cType,
  *             ucByte ： 等待发送的字节
  * @OutPara    NONE
  * @retval     None
  */
void HAL_SWI2C_SendByte(SW_I2C_Typedef i2cType,uint8_t _ucByte)
{
	uint8_t i;

	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			HAL_I2C_SDA_H(i2cType);
		}
		else
		{
			HAL_I2C_SDA_L(i2cType);
		}
		i2c_Delay();
		HAL_I2C_SCL_H(i2cType);
		i2c_Delay();
		HAL_I2C_SCL_L(i2cType);
		if (i == 7)
		{
			 HAL_I2C_SDA_H(i2cType); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参:  无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t HAL_SWI2C_ReadByte(SW_I2C_Typedef i2cType)
{
	uint8_t i;
	uint8_t value;

	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		HAL_I2C_SCL_H(i2cType);
		i2c_Delay();
		if (HAL_I2C_SDA_READ(i2cType))
		{
			value++;
		}
		HAL_I2C_SCL_L(i2cType);
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参:  无
*	返 回 值: 返回0 RESPONSE_OK表示正确应答，1 RESPONSE_ERR表示无器件响应
*********************************************************************************************************
*/
uint8_t HAL_SWI2C_WaitAck(SW_I2C_Typedef i2cType)
{
	uint8_t ucErrTime=0;
    
	HAL_I2C_SDA_H(i2cType);	/* CPU释放SDA总线 */
	i2c_Delay();
	HAL_I2C_SCL_H(i2cType);	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	while (HAL_I2C_SDA_READ(i2cType))	/* CPU读取SDA口线状态 */
	{
        ucErrTime++;
		if(ucErrTime>250)
		{
			HAL_SWI2C_Stop(i2cType);
			return RESPONSE_ERR;
		}        
	}

	HAL_I2C_SCL_L(i2cType);
	i2c_Delay();
	return RESPONSE_OK;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_SWI2C_Ack(SW_I2C_Typedef i2cType)
{
	HAL_I2C_SDA_L(i2cType);	/* CPU驱动SDA = 0 */
	i2c_Delay();
	HAL_I2C_SCL_H(i2cType);	/* CPU产生1个时钟 */
	i2c_Delay();
	HAL_I2C_SCL_L(i2cType);
	i2c_Delay();
	HAL_I2C_SDA_H(i2cType);	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void HAL_SWI2C_NAck(SW_I2C_Typedef i2cType)
{
	HAL_I2C_SDA_H(i2cType);	/* CPU驱动SDA = 1 */
	i2c_Delay();
	HAL_I2C_SCL_H(i2cType);	/* CPU产生1个时钟 */
	i2c_Delay();
	HAL_I2C_SCL_L(i2cType);
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参:  _Address：设备的I2C总线地址
*	返 回 值: 返回值 0 RESPONSE_OK 表示正确， 返回1 RESPONSE_ERR 表示未探测到
*********************************************************************************************************
*/
uint8_t HAL_SWI2C_CheckDevice(SW_I2C_Typedef i2cType,uint8_t _Address)
{
	uint8_t ucAck;

	if (HAL_I2C_SDA_READ(i2cType) && HAL_I2C_SCL_READ(i2cType))
	{
		HAL_SWI2C_Start(i2cType);		/* 发送启动信号 */

		/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
		HAL_SWI2C_SendByte(i2cType,_Address | I2C_WR);
		ucAck = HAL_SWI2C_WaitAck(i2cType);	/* 检测设备的ACK应答 */

		HAL_SWI2C_Stop(i2cType);			/* 发送停止信号 */

		return ucAck;
	}
	return RESPONSE_ERR;	/* I2C总线异常 */
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
static void HAL_I2C_SCL_H(SW_I2C_Typedef i2cType)
{
    BSP_SetSclHigh(i2cType);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
static void HAL_I2C_SCL_L(SW_I2C_Typedef i2cType)
{
    BSP_SetSclLow(i2cType);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
static void HAL_I2C_SDA_H(SW_I2C_Typedef i2cType)
{
    BSP_SetSdaHigh(i2cType);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
static void HAL_I2C_SDA_L(SW_I2C_Typedef i2cType)
{
    BSP_SetSdaLow(i2cType);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
static uint8_t HAL_I2C_SCL_READ(SW_I2C_Typedef i2cType)
{
    return BSP_SetSclRead(i2cType);
}

/**
  * @brief      Init software I2C gpio.
  * @param  
  * @InPara     SW_I2C_Typedef i2cType
  * @OutPara    NONE
  * @retval     None
  */
static uint8_t HAL_I2C_SDA_READ(SW_I2C_Typedef i2cType)
{
    return BSP_SetSdaRead(i2cType);
}


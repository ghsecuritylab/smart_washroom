/**
  ******************************************************************************
  * @file    hal_sw_i2c.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-08-26
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

#ifndef _HAL_SW_I2C_H
#define _HAL_SW_I2C_H

#include "common_def.h"
#include "bsp_dev_i2c_gpio.h"

#define RESPONSE_OK     0  //IIC response
#define RESPONSE_ERR    1


#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

void HAL_InitSWI2C(SW_I2C_Typedef i2cType);
void HAL_SWI2C_Start(SW_I2C_Typedef i2cType);
void HAL_SWI2C_Stop(SW_I2C_Typedef i2cType);
void HAL_SWI2C_SendByte(SW_I2C_Typedef i2cType,uint8_t _ucByte);
uint8_t HAL_SWI2C_ReadByte(SW_I2C_Typedef i2cType);
uint8_t HAL_SWI2C_WaitAck(SW_I2C_Typedef i2cType);
void HAL_SWI2C_Ack(SW_I2C_Typedef i2cType);
void HAL_SWI2C_NAck(SW_I2C_Typedef i2cType);
uint8_t HAL_SWI2C_CheckDevice(SW_I2C_Typedef i2cType,uint8_t _Address);

#endif

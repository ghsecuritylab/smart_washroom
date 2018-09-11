/**
  ******************************************************************************
  * @file    main.c 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef __BSP_TOUCH_H__
#define __BSP_TOUCH_H__

#include "common_def.h"

	
/*---------------------- GT9XX芯片相关定义 ------------------------*/
					
#define TOUCH_MAX   5	//最大触摸点数

typedef struct 
{
	uint8_t  flag;	//触摸标志位，为1时表示有触摸操作
	uint8_t  num;		//触摸点数
	uint16_t x[TOUCH_MAX];	//x坐标
	uint16_t y[TOUCH_MAX];	//y坐标
}TouchStructure;

extern TouchStructure g_tTouchInfo;	//结构体声明

#define GT9XX_IIC_RADDR 0xBB	
#define GT9XX_IIC_WADDR 0xBA

#define GT9XX_READ_ADDR 0x814E	//信息寄存器
#define GT9XX_ID_ADDR   0x8140		//触摸面板ID寄存器

void BSP_TouchInit(void);
uint8_t BSP_TouchScan(void);

#endif


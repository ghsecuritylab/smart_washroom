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

	
/*---------------------- GT9XXоƬ��ض��� ------------------------*/
					
#define TOUCH_MAX   5	//���������

typedef struct 
{
	uint8_t  flag;	//������־λ��Ϊ1ʱ��ʾ�д�������
	uint8_t  num;		//��������
	uint16_t x[TOUCH_MAX];	//x����
	uint16_t y[TOUCH_MAX];	//y����
}TouchStructure;

extern TouchStructure g_tTouchInfo;	//�ṹ������

#define GT9XX_IIC_RADDR 0xBB	
#define GT9XX_IIC_WADDR 0xBA

#define GT9XX_READ_ADDR 0x814E	//��Ϣ�Ĵ���
#define GT9XX_ID_ADDR   0x8140		//�������ID�Ĵ���

void BSP_TouchInit(void);
uint8_t BSP_TouchScan(void);

#endif


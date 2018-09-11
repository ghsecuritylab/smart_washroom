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
	Ӧ��˵����
	�ڷ���I2C�豸ǰ�����ȵ��� i2c_CheckDevice() ���I2C�豸�Ƿ��������ú���������GPIO
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

    /* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
	HAL_SWI2C_Stop(i2cType);

}
/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
		CPU��Ƶ168MHzʱ�����ڲ�Flash����, MDK���̲��Ż�����̨ʽʾ�����۲Ⲩ�Ρ�
		ѭ������Ϊ5ʱ��SCLƵ�� = 1.78MHz (����ʱ: 92ms, ��д������������ʾ����̽ͷ���ϾͶ�дʧ�ܡ�ʱ��ӽ��ٽ�)
		ѭ������Ϊ10ʱ��SCLƵ�� = 1.1MHz (����ʱ: 138ms, ���ٶ�: 118724B/s)
		ѭ������Ϊ30ʱ��SCLƵ�� = 440KHz�� SCL�ߵ�ƽʱ��1.0us��SCL�͵�ƽʱ��1.2us

		��������ѡ��2.2Kŷʱ��SCL������ʱ��Լ0.5us�����ѡ4.7Kŷ����������Լ1us

		ʵ��Ӧ��ѡ��400KHz���ҵ����ʼ���
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
     * ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź�
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
     * ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� 
     */
	HAL_I2C_SDA_L(i2cType);
	HAL_I2C_SCL_H(i2cType);
	i2c_Delay();
	HAL_I2C_SDA_H(i2cType);
	i2c_Delay();
}

/**
  * @brief      CPU��I2C�����豸����8bit����
  * @param  
  * @InPara     SW_I2C_Typedef i2cType,
  *             ucByte �� �ȴ����͵��ֽ�
  * @OutPara    NONE
  * @retval     None
  */
void HAL_SWI2C_SendByte(SW_I2C_Typedef i2cType,uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
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
			 HAL_I2C_SDA_H(i2cType); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    ��:  ��
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t HAL_SWI2C_ReadByte(SW_I2C_Typedef i2cType)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
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
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    ��:  ��
*	�� �� ֵ: ����0 RESPONSE_OK��ʾ��ȷӦ��1 RESPONSE_ERR��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t HAL_SWI2C_WaitAck(SW_I2C_Typedef i2cType)
{
	uint8_t ucErrTime=0;
    
	HAL_I2C_SDA_H(i2cType);	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	HAL_I2C_SCL_H(i2cType);	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	while (HAL_I2C_SDA_READ(i2cType))	/* CPU��ȡSDA����״̬ */
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
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_SWI2C_Ack(SW_I2C_Typedef i2cType)
{
	HAL_I2C_SDA_L(i2cType);	/* CPU����SDA = 0 */
	i2c_Delay();
	HAL_I2C_SCL_H(i2cType);	/* CPU����1��ʱ�� */
	i2c_Delay();
	HAL_I2C_SCL_L(i2cType);
	i2c_Delay();
	HAL_I2C_SDA_H(i2cType);	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HAL_SWI2C_NAck(SW_I2C_Typedef i2cType)
{
	HAL_I2C_SDA_H(i2cType);	/* CPU����SDA = 1 */
	i2c_Delay();
	HAL_I2C_SCL_H(i2cType);	/* CPU����1��ʱ�� */
	i2c_Delay();
	HAL_I2C_SCL_L(i2cType);
	i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_CheckDevice
*	����˵��: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
*	��    ��:  _Address���豸��I2C���ߵ�ַ
*	�� �� ֵ: ����ֵ 0 RESPONSE_OK ��ʾ��ȷ�� ����1 RESPONSE_ERR ��ʾδ̽�⵽
*********************************************************************************************************
*/
uint8_t HAL_SWI2C_CheckDevice(SW_I2C_Typedef i2cType,uint8_t _Address)
{
	uint8_t ucAck;

	if (HAL_I2C_SDA_READ(i2cType) && HAL_I2C_SCL_READ(i2cType))
	{
		HAL_SWI2C_Start(i2cType);		/* ���������ź� */

		/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
		HAL_SWI2C_SendByte(i2cType,_Address | I2C_WR);
		ucAck = HAL_SWI2C_WaitAck(i2cType);	/* ����豸��ACKӦ�� */

		HAL_SWI2C_Stop(i2cType);			/* ����ֹͣ�ź� */

		return ucAck;
	}
	return RESPONSE_ERR;	/* I2C�����쳣 */
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


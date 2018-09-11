/**
  ******************************************************************************
  * @file    bsp_touch.c 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "bsp_touch.h"
#include "hal_sw_i2c.h"
#include "serial_debug_log.h"

TouchStructure g_tTouchInfo; //������Ϣ�ṹ��

/*---------------------- GT9XX��غ��� ------------------------*/

// ������GT9XX д����
//	������addr - Ҫ�����ļĴ���
//			
static uint8_t GT9XX_WriteHandle (uint16_t addr)
{
	uint8_t status;

	HAL_SWI2C_Start(SW_I2C1);
    
    HAL_SWI2C_SendByte(SW_I2C1,GT9XX_IIC_WADDR);  //д����ָ��
    if(HAL_SWI2C_WaitAck(SW_I2C1) != RESPONSE_OK)
    {
        LOG(LEVEL_ERROR,"I2C no ack!\n");
        goto cmd_fail;
    }
    
    HAL_SWI2C_SendByte(SW_I2C1,(uint8_t)(addr >> 8));//д��16��ַ
    if(HAL_SWI2C_WaitAck(SW_I2C1) != RESPONSE_OK)
    {
        LOG(LEVEL_ERROR,"I2C no ack!\n");
        goto cmd_fail;
    }
    
    HAL_SWI2C_SendByte((uint8_t)(addr)) ;
    if(i2c_WaitAck() != RESPONSE_OK)
    {
        LOG(LEVEL_ERROR,"I2C no ack!\n");
        goto cmd_fail;
    }
	status = SUCCESS;
	return status;
    
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	
	return ERROR;	
}

// ������GT9XX д����
//	������addr - Ҫд���ݵĵ�ַ
//			value - д�������
//
static  uint8_t GT9XX_WriteData (uint16_t addr,uint8_t value)
{
	uint8_t status;
	
	i2c_Start(); //����IICͨѶ

	if( GT9XX_WriteHandle(addr) == SUCCESS)	//д��Ҫ�����ļĴ���
	{
        i2c_SendByte(value) ;//д����
        
        if(i2c_WaitAck() != RESPONSE_OK)
        {
            LOG(LEVEL_ERROR,"I2C no ack!\n");
            goto cmd_fail;
        }
	}
	
	i2c_Stop(); //ֹͣͨѶ
	
	status = SUCCESS;
	return status;
    
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	
	return ERROR;    
}

// ������GT9XX ������
//	������addr - Ҫ�����ݵĵ�ַ
//			num - �������ֽ���
//			*value - ���ڻ�ȡ�洢���ݵ��׵�ַ
//
static  uint8_t GT9XX_ReadData (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status;
	uint8_t i;

	status = ERROR;
	i2c_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS) //д��Ҫ�����ļĴ���
	{
		i2c_Stop(); //ֹͣͨѶ
        
        i2c_Start(); //��������IICͨѶ

		i2c_SendByte(GT9XX_IIC_RADDR);
        if(i2c_WaitAck() != RESPONSE_OK)
        {
            LOG(LEVEL_ERROR,"I2C no ack!\n");
            goto cmd_fail;
        }

        for(i = 0 ; i < cnt; i++)
        {
            if (i == (cnt - 1))
            {
                value[i] = i2c_ReadByte();//�������һ������ʱ���� ��Ӧ���ź�
                i2c_NAck(); //��Ӧ���ź�
            }
            else
            {
                value[i] = i2c_ReadByte();
                i2c_Ack();
            }
		}
	}else
    {
         goto cmd_fail;
    }
	status = SUCCESS;
	i2c_Stop();
	return (status);
    
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	
	return ERROR; 	
}

/**
  * @brief  
  * @param  None 
  * @retval None
  */
void BSP_TouchInit(void)
{
	uint8_t touchIC_ID[4];
    BSP_InitI2C(); //��ʼ������ͨ�ŵ�IIC����
    GT9XX_ReadData (GT9XX_ID_ADDR,4,touchIC_ID);	//��ID��Ϣ
	
	if( touchIC_ID[0] == '9' )	//�жϵ�һ���ַ��Ƿ�Ϊ 9
	{
		LOG(LEVEL_DEBUG,"Touch ID: %s \r\n",touchIC_ID);	//��ӡ����оƬ��ID
	}
	else
	{
		LOG(LEVEL_DEBUG,"Touch Error\r\n");	//����δ��⵽������
	}
}

// ����������ɨ��
//	˵�����ڳ����������Եĵ��øú��������Լ�ⴥ������
//
uint8_t	BSP_TouchScan(void)
{
 	uint8_t  touchData[2 + 8 * TOUCH_MAX ]; //���ڴ洢��������

	GT9XX_ReadData (GT9XX_READ_ADDR,2 + 8 * TOUCH_MAX ,touchData);	//������
	GT9XX_WriteData (GT9XX_READ_ADDR,0);	//	�������оƬ�ļĴ�����־λ
	g_tTouchInfo.num = touchData[0] & 0x0f;	//ȡ��ǰ�Ĵ�������
	
	if ( (g_tTouchInfo.num >= 1) && (g_tTouchInfo.num <=5) ) //���������� 1-5 ֮��ʱ
	{
		// ȡ��Ӧ�Ĵ�������
		switch(g_tTouchInfo.num)
		{
			case 5:
			{
				g_tTouchInfo.y[4] = (touchData[5+32]<<8) | touchData[4+32];
				g_tTouchInfo.x[4] = (touchData[3+32]<<8) | touchData[2+32];	
     		}
			case 4:
			{
				g_tTouchInfo.y[3] = (touchData[5+24]<<8) | touchData[4+24];
				g_tTouchInfo.x[3] = (touchData[3+24]<<8) | touchData[2+24];		
			}
			case 3:
			{
				g_tTouchInfo.y[2] = (touchData[5+16]<<8) | touchData[4+16];
				g_tTouchInfo.x[2] = (touchData[3+16]<<8) | touchData[2+16];				
			}
			case 2:
			{
				g_tTouchInfo.y[1] = (touchData[5+8]<<8) | touchData[4+8];
				g_tTouchInfo.x[1] = (touchData[3+8]<<8) | touchData[2+8];			
				
			}
			case 1:
			{
				g_tTouchInfo.y[0] = ((touchData[5]<<8) | touchData[4])*0.81;
				g_tTouchInfo.x[0] = ((touchData[3]<<8) | touchData[2])*0.78;					
			}
			default:break;
		}	
		g_tTouchInfo.flag = 1;		
		return	SUCCESS ;	
	}

	else                       
	{
		g_tTouchInfo.flag = 0;
		return	ERROR ;		
	}
	
}




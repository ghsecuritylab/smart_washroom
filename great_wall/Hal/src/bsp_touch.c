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

TouchStructure g_tTouchInfo; //触摸信息结构体

/*---------------------- GT9XX相关函数 ------------------------*/

// 函数：GT9XX 写操作
//	参数：addr - 要操作的寄存器
//			
static uint8_t GT9XX_WriteHandle (uint16_t addr)
{
	uint8_t status;

	HAL_SWI2C_Start(SW_I2C1);
    
    HAL_SWI2C_SendByte(SW_I2C1,GT9XX_IIC_WADDR);  //写数据指令
    if(HAL_SWI2C_WaitAck(SW_I2C1) != RESPONSE_OK)
    {
        LOG(LEVEL_ERROR,"I2C no ack!\n");
        goto cmd_fail;
    }
    
    HAL_SWI2C_SendByte(SW_I2C1,(uint8_t)(addr >> 8));//写入16地址
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
    
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	
	return ERROR;	
}

// 函数：GT9XX 写数据
//	参数：addr - 要写数据的地址
//			value - 写入的数据
//
static  uint8_t GT9XX_WriteData (uint16_t addr,uint8_t value)
{
	uint8_t status;
	
	i2c_Start(); //启动IIC通讯

	if( GT9XX_WriteHandle(addr) == SUCCESS)	//写入要操作的寄存器
	{
        i2c_SendByte(value) ;//写数据
        
        if(i2c_WaitAck() != RESPONSE_OK)
        {
            LOG(LEVEL_ERROR,"I2C no ack!\n");
            goto cmd_fail;
        }
	}
	
	i2c_Stop(); //停止通讯
	
	status = SUCCESS;
	return status;
    
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
	i2c_Stop();
	
	return ERROR;    
}

// 函数：GT9XX 读数据
//	参数：addr - 要读数据的地址
//			num - 读出的字节数
//			*value - 用于获取存储数据的首地址
//
static  uint8_t GT9XX_ReadData (uint16_t addr, uint8_t cnt, uint8_t *value)
{
	uint8_t status;
	uint8_t i;

	status = ERROR;
	i2c_Start();

	if( GT9XX_WriteHandle(addr) == SUCCESS) //写入要操作的寄存器
	{
		i2c_Stop(); //停止通讯
        
        i2c_Start(); //重新启动IIC通讯

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
                value[i] = i2c_ReadByte();//读到最后一个数据时发送 非应答信号
                i2c_NAck(); //非应答信号
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
    
cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
	/* 发送I2C总线停止信号 */
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
    BSP_InitI2C(); //初始化用于通信的IIC引脚
    GT9XX_ReadData (GT9XX_ID_ADDR,4,touchIC_ID);	//读ID信息
	
	if( touchIC_ID[0] == '9' )	//判断第一个字符是否为 9
	{
		LOG(LEVEL_DEBUG,"Touch ID: %s \r\n",touchIC_ID);	//打印触摸芯片的ID
	}
	else
	{
		LOG(LEVEL_DEBUG,"Touch Error\r\n");	//错误，未检测到触摸屏
	}
}

// 函数：触摸扫描
//	说明：在程序里周期性的调用该函数，用以检测触摸操作
//
uint8_t	BSP_TouchScan(void)
{
 	uint8_t  touchData[2 + 8 * TOUCH_MAX ]; //用于存储触摸数据

	GT9XX_ReadData (GT9XX_READ_ADDR,2 + 8 * TOUCH_MAX ,touchData);	//读数据
	GT9XX_WriteData (GT9XX_READ_ADDR,0);	//	清除触摸芯片的寄存器标志位
	g_tTouchInfo.num = touchData[0] & 0x0f;	//取当前的触摸点数
	
	if ( (g_tTouchInfo.num >= 1) && (g_tTouchInfo.num <=5) ) //当触摸数在 1-5 之间时
	{
		// 取相应的触摸坐标
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




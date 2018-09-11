/**
  ******************************************************************************
  * @file    bsp_dev_lcd_config.c 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_dev_lcd_config.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*********************************************************************************************************
*	                                 下面的函数被emWin所调用
*********************************************************************************************************
*/

Lcd_InitParaTypeDef t_LcdPara;  //全局LCD参数变量，会被enWin调用
uint8_t g_LcdType = LCD_RGB_AT070TN92;

/*
*********************************************************************************************************
*	函 数 名: Bsp_lcdGetType
*	功能说明: 获取使用的LCD模组型号
*	形    参：lcdType:读取到的lcd ID号
*	返 回 值: 无
*********************************************************************************************************
*/
uint8_t Bsp_lcdGetType(void)
{
    
    return 0;
}

/*
*********************************************************************************************************
*	函 数 名: Bsp_LcdSetPara
*	功能说明: 根据lcd类型，初始化Lcd结构体t_LcdPara，供lcd init使用
*	形    参：lcdType:读取到的lcd ID号
*	返 回 值: 无
*********************************************************************************************************
*/
void Bsp_LcdSetPara(uint8_t lcdType)
{
   switch(lcdType)
   {
       //群创7“屏
       case LCD_RGB_AT070TN92:
            {   
                t_LcdPara.ClockPllsain = 25*3*2;
                t_LcdPara.ClockPllsair = 3;
                t_LcdPara.ClockPllsaidiver =  PLLSAIDIVR_2;
                
                t_LcdPara.Width = 800;
                t_LcdPara.Height = 480;
                
                t_LcdPara.HSW = 1;
                t_LcdPara.VSW = 1;
                t_LcdPara.HBP = 46;
                t_LcdPara.VBP = 23;
                t_LcdPara.HFP = 210;
                t_LcdPara.VFP = 22;
            }
           break;
       case LCD_RGB_XXXX:
            {   
                t_LcdPara.ClockPllsain = 400;
                t_LcdPara.ClockPllsair = 4;
                t_LcdPara.ClockPllsaidiver =  PLLSAIDIVR_4;
                
                t_LcdPara.Width = 800;
                t_LcdPara.Height = 480;
                
                t_LcdPara.HSW = 1;
                t_LcdPara.VSW = 1;
                t_LcdPara.HBP = 46;
                t_LcdPara.VBP = 23;
                t_LcdPara.HFP = 210;
                t_LcdPara.VFP = 22;
            }
           break;
       case LCD_NULL:
           //no support lcd ,print some log here
           break;
   }       
    
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

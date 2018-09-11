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
*	                                 ����ĺ�����emWin������
*********************************************************************************************************
*/

Lcd_InitParaTypeDef t_LcdPara;  //ȫ��LCD�����������ᱻenWin����
uint8_t g_LcdType = LCD_RGB_AT070TN92;

/*
*********************************************************************************************************
*	�� �� ��: Bsp_lcdGetType
*	����˵��: ��ȡʹ�õ�LCDģ���ͺ�
*	��    �Σ�lcdType:��ȡ����lcd ID��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t Bsp_lcdGetType(void)
{
    
    return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: Bsp_LcdSetPara
*	����˵��: ����lcd���ͣ���ʼ��Lcd�ṹ��t_LcdPara����lcd initʹ��
*	��    �Σ�lcdType:��ȡ����lcd ID��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Bsp_LcdSetPara(uint8_t lcdType)
{
   switch(lcdType)
   {
       //Ⱥ��7����
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

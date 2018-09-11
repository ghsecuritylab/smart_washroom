/**
  ******************************************************************************
  * @file    bsp_lcd_config.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-08-08
  * @brief   该文件作为多lcd显示配置文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_DEV_LCD_CONFIG_H
#define __BSP_DEV_LCD_CONFIG_H
#include "common_def.h"


/* 1、LCD面板类型 */
enum
{
    LCD_RGB_AT070TN92 = 0,
    LCD_RGB_XXXX,
    LCD_NULL = 0xff,
};

typedef struct {
    __IO uint16_t Width;
    __IO uint16_t Height;
    __IO uint16_t HSW ;
    __IO uint16_t VSW;
    __IO uint16_t HBP ;
    __IO uint16_t VBP ;
    __IO uint16_t HFP ;
    __IO uint16_t VFP ;
    __IO uint32_t ClockPllsain;
    __IO uint32_t ClockPllsair;
    __IO uint32_t ClockPllsaidiver ;
    uint8_t Dir;    //方向
    uint8_t activelayer;    
    uint8_t pixsize;    //每个像素所占字节数
}Lcd_InitParaTypeDef;



#define PLLSAIDIVR_2                ((uint32_t)0x00000000U)
#define PLLSAIDIVR_4                ((uint32_t)0x00010000U)
#define PLLSAIDIVR_8                ((uint32_t)0x00020000U)
#define PLLSAIDIVR_16               ((uint32_t)0x00030000U)

uint8_t Bsp_lcdGetType(void);
void Bsp_LcdSetPara(uint8_t lcdType);
uint32_t BSP_LCD_GetYSize(void);
uint32_t BSP_LCD_GetXSize(void);

/* 外部声明全局变量 */
extern uint8_t g_LcdType;
extern Lcd_InitParaTypeDef t_LcdPara;
/**
  * @}
  */
  

#endif 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

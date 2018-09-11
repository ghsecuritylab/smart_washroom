/**
  ******************************************************************************
  * @file    AT070TN92_800x480.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT070TN92_800x480_H
#define __AT070TN92_800x480_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/  

/** @addtogroup BSP
  * @{
  */ 

/** @addtogroup Components
  * @{
  */ 
  
/** @addtogroup AT070TN92_800x480
  * @{
  */

/** @defgroup AT070TN92_800x480_Exported_Types
  * @{
  */
   
/**
  * @}
  */ 

/** @defgroup AT070TN92_800x480_Exported_Constants
  * @{
  */
  
/** 
  * @brief  AT070TN92_800x480 Size  
  */ 
#define LCD_Width     ((uint16_t)800)   //LCD的像素长度
#define LCD_Height    ((uint16_t)480)   //LCD的像素宽度
#define LCD_Pixels    (LCD_Width*LCD_Height) 		    //分辨率  
/** 
  * @brief  AT070TN92_800x480 Timing  
  */     

#define HBP  (uint16_t)46     //HS Blanking     thb
#define VBP  (uint16_t)23     //VS Blanking     tvb    
#define HSW  (uint16_t)1      //HS pulse width   thpw
#define VSW  (uint16_t)1    //VS pulse width    tvpw
#define HFP  (uint16_t)210    //HS Front Porch      thfp
#define VFP  (uint16_t)22     //VS Front Porch      tvfp
/** 
  * @brief  AT070TN92_800x480 frequency divider  
  */    
#define  AT070TN92_800x480_FREQUENCY_DIVIDER    2            /* LCD Frequency divider      */
#define DCLK_FRQ    33.3           
/**
  * @}
  */
  
/** @defgroup AT070TN92_800x480_Exported_Functions
  * @{
  */    

/**
  * @}
  */    
#ifdef __cplusplus
}
#endif

#endif /* __AT070TN92_800x480_H */
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

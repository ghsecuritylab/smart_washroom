/**
  ******************************************************************************
  * @file    hal_board_init.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26 
  * @brief   Header for main.c file
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HAL_SDRAM_H
#define __HAL_SDRAM_H

#include "common_def.h"
#include "bsp_dev_sdram.h"

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/

/* LCD�Դ�, ͼ��1, ����4M�ֽ� */
#define SDRAM_LCD_BUF1 		SDRAM_DEVICE_ADDR

/* LCD�Դ�, ͼ��2, ����4M�ֽ� */
#define SDRAM_LCD_BUF2	   (SDRAM_LCD_BUF1 + 4 * 1024 * 1024)

/* ��SDRAM��������Ĳ��Դ�������� */
#define SDRAM_APP_SIZE	   (12 * 1024 * 1024)

/*
**********************************************************************************************************
					emWin��̬�ڴ�ʹ�ã������Դ�ʹ�õ�8MB����8MB����̬�ڴ�ʹ��
**********************************************************************************************************
*/
/* emWin��̬�ڴ��׵�ַ */
#define SDRAM_APP_BUF     (SDRAM_LCD_BUF1 + 8 * 1024 * 1024)  
#define GUI_USE_SDRAM_NUM  8 * 1024 * 1024      //8M      
          
/* EVAL includes component */

/* FatFs includes component */

/* Exported types ------------------------------------------------------------*/

/* Exported variables --------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void HAL_SdramInit(void);
uint32_t HAL_SdramGetBaseAddr(void);     
void HAL_SdramReadData(void);
void HAL_SdramReadDataDMA(void);
void HAL_SdramWriteData(void);
void HAL_SdramWriteData_DMA(void);     
     
#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

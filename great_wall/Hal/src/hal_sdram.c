/**
  ******************************************************************************
  * @file    hal_sdram.c 
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
#include "hal_sdram.h"

#include "bsp_dev_sdram.h"



/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes SRAM.
  * @param  None 
  * @retval None
  */
void HAL_SdramInit(void)
{
    /* Configure HardWareInit */
    BSP_SDRAM_Init();
  
}

/**
  * @brief  Initializes SRAM.
  * @param  None 
  * @retval None
  */
uint32_t HAL_SdramGetBaseAddr(void)
{
    /* return sdram start addr  */
    return SDRAM_DEVICE_ADDR;
  
}

/**
  * @brief  Initializes SRAM.
  * @param  None 
  * @retval None
  */
void HAL_SdramReadData(void)
{
    /* Configure HardWareInit */
    //BSP_SDRAM_ReadData();
  
}

/**
  * @brief  Initializes SRAM.
  * @param  None 
  * @retval None
  */
void HAL_SdramReadDataDMA(void)
{
    /* Configure HardWareInit */
    //BSP_SDRAM_ReadData_DMA();
  
}

/**
  * @brief  Initializes SRAM.
  * @param  None 
  * @retval None
  */
void HAL_SdramWriteData(void)
{
    /* Configure HardWareInit */
    //BSP_SDRAM_WriteData();
  
}

/**
  * @brief  Initializes SRAM.
  * @param  None 
  * @retval None
  */
void HAL_SdramWriteData_DMA(void)
{
    /* Configure HardWareInit */
    //BSP_SDRAM_WriteData_DMA();
  
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

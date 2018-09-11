/**
  ******************************************************************************
  * @file    biz.c 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2017-07-26
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* File Info : -----------------------------------------------------------------
                                   User NOTES 
 
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "bsp_dev_lcd_ewin.h"
#include "bsp_dev_lcd_config.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
/* 这两个变量会被EWIN调用 */
LTDC_HandleTypeDef  LtdcHandler;
DMA2D_HandleTypeDef Dma2dHandler;
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void Bsp_LcdGpioInit(void);
static void BSP_LtdcInit(void);
static uint8_t BSP_LtdcClkSet(uint32_t pllsain,uint32_t pllsair,uint32_t pllsaidivr);

/* Private functions ---------------------------------------------------------*/



//////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
*********************************************************************************************************
*	                                 下面的函数被emWin所调用
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*	函 数 名: LCD_ConfigLTDC
*	功能说明: 配置LTDC
*	形    参: 无
*	返 回 值: 无
*   笔    记:
*       LCD_TFT 同步时序配置（整理自官方做的一个截图，言简意赅）：
*       ----------------------------------------------------------------------------
*    
*                                                 Total Width
*                             <--------------------------------------------------->
*                       Hsync width HBP             Active Width                HFP
*                             <---><--><--------------------------------------><-->
*                         ____    ____|_______________________________________|____ 
*                             |___|   |                                       |    |
*                                     |                                       |    |
*                         __|         |                                       |    |
*            /|\    /|\  |            |                                       |    |
*             | VSYNC|   |            |                                       |    |
*             |Width\|/  |__          |                                       |    |
*             |     /|\     |         |                                       |    |
*             |  VBP |      |         |                                       |    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |         | / / / / / / / / / / / / / / / / / / / |    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Total    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*    Heigh    |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Active|      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |Heigh |      |         |/ / / / / / Active Display Area / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |      |      |         |/ / / / / / / / / / / / / / / / / / / /|    |
*             |     \|/_____|_________|_______________________________________|    |
*             |     /|\     |                                                      |
*             |  VFP |      |                                                      |
*            \|/    \|/_____|______________________________________________________|
*            
*     
*     每个LCD设备都有自己的同步时序值：
*     Horizontal Synchronization (Hsync) 
*     Horizontal Back Porch (HBP)       
*     Active Width                      
*     Horizontal Front Porch (HFP)     
*   
*     Vertical Synchronization (Vsync)  
*     Vertical Back Porch (VBP)         
*     Active Heigh                       
*     Vertical Front Porch (VFP)         
*     
*     LCD_TFT 窗口水平和垂直的起始以及结束位置 :
*     ----------------------------------------------------------------
*   
*     HorizontalStart = (Offset_X + Hsync + HBP);
*     HorizontalStop  = (Offset_X + Hsync + HBP + Window_Width - 1); 
*     VarticalStart   = (Offset_Y + Vsync + VBP);
*     VerticalStop    = (Offset_Y + Vsync + VBP + Window_Heigh - 1);
*
*********************************************************************************************************
*/
/**
  * @brief  Initializes the LCD.
  * @retval LCD state
  */
void BSP_LCD_LL_Init(void)
{
    /* config lcd gpio */
    Bsp_LcdGpioInit();
    
    /* ltdc init */
    BSP_LtdcInit();
    
    t_LcdPara.Dir =1;
}

/**
  * @brief  Initializes the LTDC MSP.
  */
static void Bsp_LcdGpioInit(void)
{
    GPIO_InitTypeDef GPIO_Init_Structure;

    /* Enable peripherals and GPIO Clocks */  
    /* Enable the LTDC Clock */
    __HAL_RCC_LTDC_CLK_ENABLE();

    /* Enable GPIO Clock */
    __HAL_RCC_DMA2D_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();
    __HAL_RCC_GPIOJ_CLK_ENABLE();
    __HAL_RCC_GPIOK_CLK_ENABLE();  

    /* Configure peripheral GPIO */

    /* LTDC pins configuraiton: PI12 -- 15 */  
    GPIO_Init_Structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FAST;
    GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;  
    HAL_GPIO_Init(GPIOI, &GPIO_Init_Structure);

    /* LTDC pins configuraiton: PJ0 -- 15 */  
    GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                  GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15; 
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FAST;
    GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;  
    HAL_GPIO_Init(GPIOJ, &GPIO_Init_Structure);  

    /* LTDC pins configuraiton: PK0 -- 7 */  
    GPIO_Init_Structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7; 
    GPIO_Init_Structure.Mode      = GPIO_MODE_AF_PP;
    GPIO_Init_Structure.Pull      = GPIO_NOPULL;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FAST;
    GPIO_Init_Structure.Alternate = GPIO_AF14_LTDC;  
    HAL_GPIO_Init(GPIOK, &GPIO_Init_Structure);


    /* backlight pin config */
    GPIO_Init_Structure.Pin       = GPIO_PIN_13; 
    GPIO_Init_Structure.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_Init_Structure.Pull      = GPIO_PULLUP;
    GPIO_Init_Structure.Speed     = GPIO_SPEED_FAST;
    HAL_GPIO_Init(GPIOD, &GPIO_Init_Structure);

    /* Set LTDC Interrupt to the lowest priority */
    HAL_NVIC_SetPriority(LTDC_IRQn, 0xE, 0);

    /* Enable LTDC Interrupt */
    HAL_NVIC_EnableIRQ(LTDC_IRQn);
}

/*
*********************************************************************************************************
*	函 数 名: bsp_Init
*	功能说明: 初始化所有的硬件设备。该函数配置CPU寄存器和外设的寄存器并初始化一些全局变量。只需要调用一次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void BSP_LtdcInit(void)
{
    /* LTDC Configuration ----------------------------------------------------*/
    LtdcHandler.Instance = LTDC;
    
    /* Polarity */
    LtdcHandler.Init.HSPolarity = LTDC_HSPOLARITY_AL;
    LtdcHandler.Init.VSPolarity = LTDC_VSPOLARITY_AL;
    LtdcHandler.Init.DEPolarity = LTDC_DEPOLARITY_AL;
    LtdcHandler.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
      
    /* Configure R,G,B component values for LCD background color */
    LtdcHandler.Init.Backcolor.Red= 0;
    LtdcHandler.Init.Backcolor.Blue= 0;
    LtdcHandler.Init.Backcolor.Green= 0;
    
    Bsp_LcdSetPara(g_LcdType);
    
    /* Configure horizontal synchronization width */
    LtdcHandler.Init.HorizontalSync = (t_LcdPara.HSW - 1);
    /* Configure vertical synchronization height */
    LtdcHandler.Init.VerticalSync = (t_LcdPara.VSW - 1) ;
    /* Configure accumulated horizontal back porch */
    LtdcHandler.Init.AccumulatedHBP = (t_LcdPara.HSW + t_LcdPara.HBP - 1);
    /* Configure accumulated vertical back porch */
    LtdcHandler.Init.AccumulatedVBP = (t_LcdPara.VSW + t_LcdPara.VBP - 1);
    /* Configure accumulated active width */
    LtdcHandler.Init.AccumulatedActiveW = (t_LcdPara.HSW + t_LcdPara.HBP + t_LcdPara.Width - 1);
    /* Configure accumulated active height */
    LtdcHandler.Init.AccumulatedActiveH = (t_LcdPara.VSW + t_LcdPara.VBP + t_LcdPara.Height - 1);
    /* Configure total width */
    LtdcHandler.Init.TotalWidth = (t_LcdPara.HSW + t_LcdPara.HBP + t_LcdPara.Width + t_LcdPara.HFP - 1);
    /* Configure total height */
    LtdcHandler.Init.TotalHeigh = (t_LcdPara.VSW + t_LcdPara.VBP + t_LcdPara.Height + t_LcdPara.VFP - 1);
    
    /* init lcd clock */
    BSP_LtdcClkSet(t_LcdPara.ClockPllsain,t_LcdPara.ClockPllsair,t_LcdPara.ClockPllsaidiver);

    HAL_LTDC_Init(&LtdcHandler); 
    
    HAL_LTDC_ProgramLineEvent(&LtdcHandler, 0);
    
    /* Configure the DMA2D default mode */ 
    Dma2dHandler.Init.Mode         = DMA2D_R2M;
    Dma2dHandler.Init.ColorMode    = DMA2D_ARGB8888;
    Dma2dHandler.Init.OutputOffset = 0x0;     

    Dma2dHandler.Instance          = DMA2D; 

    if(HAL_DMA2D_Init(&Dma2dHandler) != HAL_OK)
    {
        //LOG(LEVEL_ERROR,"BSP_LtdcInit fail,goto while1!\n");
        while (1);      
        //do something
    }
}

/**
  * @brief  Initializes the LCD.
  * @retval LCD state
    //LTDC时钟(Fdclk)设置函数
    //Fvco=Fin*pllsain; 
    //Fdclk=Fvco/pllsair/2*2^pllsaidivr=Fin*pllsain/pllsair/2*2^pllsaidivr;

    //Fvco:VCO频率
    //Fin:输入时钟频率一般为1Mhz(来自系统时钟PLLM分频后的时钟,见时钟树图)
    //pllsain:SAI时钟倍频系数N,取值范围:50~432.  
    //pllsair:SAI时钟的分频系数R,取值范围:2~7
    //pllsaidivr:LCD时钟分频系数,取值范围:RCC_PLLSAIDIVR_2/4/8/16,对应分频2~16 
    //假设:外部晶振为25M,pllm=25的时候,Fin=1Mhz.
    //例如:要得到20M的LTDC时钟,则可以设置:pllsain=400,pllsair=5,pllsaidivr=RCC_PLLSAIDIVR_4
    //Fdclk=1*400/5/4=400/20=20Mhz  400/3/4
    //返回值:0,成功;1,失败。
  */
static uint8_t BSP_LtdcClkSet(uint32_t pllsain,uint32_t pllsair,uint32_t pllsaidivr)
{
	RCC_PeriphCLKInitTypeDef PeriphClkIniture;
	
	//LTDC输出像素时钟，需要根据自己所使用的LCD数据手册来配置！
    PeriphClkIniture.PeriphClockSelection=RCC_PERIPHCLK_LTDC;	//LTDC时钟 	
	PeriphClkIniture.PLLSAI.PLLSAIN=pllsain;    
	PeriphClkIniture.PLLSAI.PLLSAIR=pllsair;  
	PeriphClkIniture.PLLSAIDivR=pllsaidivr;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkIniture) == HAL_OK )    //配置像素时钟
    {
        return 0;   //成功
    }
    else return 1;  //失败    
}

/**
  * @brief  Gets the LCD Y size.  
  * @retval The used LCD Y size
  */
void BSP_LcdOpenBl(void)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
}

/**
  * @brief  Gets the LCD Y size.  
  * @retval The used LCD Y size
  */
void BSP_LcdCloseBl(void)
{
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
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

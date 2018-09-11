/**
  ******************************************************************************
  * @file    lcdconf.c
  * @author  MCD Application Team
  * @version V1.4.6
  * @date    04-November-2016 
  * @brief   This file implements the configuration for the GUI library
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "LCDConf.h"
#include "GUI_Private.h"
#include "hal_sdram.h"
#include "bsp_dev_lcd_ewin.h"
#include "stm32f4xx_hal.h"
#include "bsp_dev_lcd_config.h"

/*
**********************************************************************************************************
									用户可以配置的选项
**********************************************************************************************************
*/
/* 0. 在官方代码的基础上再做优化，官方的部分函数效率低，耗内存, 0表示优化 */
#define emWin_Optimize   0

/* 
  1. 显示屏的物理分辨率，驱动已经做了显示屏自适应，支持4.3寸，5寸和7寸屏
     这里填写自适应显示屏中的最大分辨率。
*/
#define XSIZE_PHYS       t_LcdPara.Width
#define YSIZE_PHYS       t_LcdPara.Height

#define XSIZE_PHYS_1       800



/* 2. 多缓冲 / 虚拟屏，多缓冲和虚拟屏不可同时使用，emWin不支持 */
#define NUM_BUFFERS      3 /* 定义多缓冲个数，仅可以设置1,2和3，也就是最大支持三缓冲 */
#define NUM_VSCREENS     1 /* 定义虚拟屏个数 */

/* 3. 没有图层激活时，背景色设置, 暂时未用到 */
#define BK_COLOR         GUI_DARKBLUE

/* 
   4. 重定义图层数，对于STM32F429/439，用户可以选择一个图层或者两个图层，不支持三图层 
      (1). 设置GUI_NUM_LAYERS = 1时，即仅使用图层1时，默认触摸值是发送给图层1的。
	  (2). 设置GUI_NUM_LAYERS = 2时，即图层1和图层2都已经使能，此时图层2是顶层，
	       用户需要根据自己的使用情况设置如下两个地方。
		   a. 在bsp_touch.c文件中的函数TOUCH_InitHard里面设置参数State.Layer = 1，1就表示
		      给图层2发送触摸值。
		   b. 调用GUI_Init函数后，调用函数GUI_SelectLayer(1), 设置当前操作的是图层2。
*/
#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS    1

/* 
   5. 设置图层1和图层2对应的显存地址
      (1) EXT_SDRAM_ADDR 是SDRAM的首地址。
      (2) LCD_LAYER0_FRAME_BUFFER 是图层1的显存地址。
	  (3) LCD_LAYER1_FRAME_BUFFER 是图层2的显存地址。
	  (4) 每个图层的显存大小比较考究，这里进行下简单的说明。
	      如果用户选择的颜色模式 = 32位色ARGB8888，显存的大小：
	      XSIZE_PHYS * YSIZE_PHYS * 4 * NUM_VSCREENS * NUM_BUFFERS
		  
	      颜色模式 = 24位色RGB888，显存的大小：
	      XSIZE_PHYS * YSIZE_PHYS * 3 * NUM_VSCREENS * NUM_BUFFERS
		  
	      颜色模式 = 16位色RGB566，ARGB1555, ARGB4444，AL88，那么显存的大小就是：
	      XSIZE_PHYS * YSIZE_PHYS * 2 * NUM_VSCREENS * NUM_BUFFERS

	      颜色模式 = 8位色L8，AL44，那么显存的大小就是：
	      XSIZE_PHYS * YSIZE_PHYS * 1 * NUM_VSCREENS * NUM_BUFFERS	
      
      这里为了方便起见，将开发板配套的16MB的SDRAM前8MB分配给LCD显存使用，后8MB用于emWin动态内存。
	  对于24位色，16位色，8位色，用户可以对其使能三缓冲，并且使能双图层。但是32位色也使能三缓冲和双
	  图层的话会超出8MB，所以用户根据自己的情况做显存和emWin动态内存的分配调整。
	    举一个例子，对于800*480分辨率的显示屏，使能32位色，三缓冲，那么最终一个图层需要的大小就是
      800 * 480 * 4 * 3  = 4.394MB的空间，如果是双图层，已经超出8MB的分配范围。

      (5)为了方便起见，图层2的宏定义LCD_LAYER1_FRAME_BUFFER中的参数4是按照32位色设置的，如果用户的图层1
         使用的是8位色，这里填数字1,如果是16位色，这里填2，如果是24位色，这里填3。
*/
#define LCD_LAYER0_FRAME_BUFFER  SDRAM_LCD_BUF1
#define LCD_LAYER1_FRAME_BUFFER  (LCD_LAYER0_FRAME_BUFFER + XSIZE_PHYS * YSIZE_PHYS * 4 * NUM_VSCREENS * NUM_BUFFERS)



/* 
   6. STM32F429/439支持的颜色模式，所有模式都支持，用户可任意配置。
      特别注意如下两个问题：
	  (1) 如果用户选择了ARGB8888或者RGB888模式，LCD闪烁比较厉害的话，
	      请降低LTDC的时钟大小，在文件bsp_tft_429.c的函数LCD_ConfigLTDC里面设置。
	      a. 一般800*480分辨率的显示屏，ARGB8888或者RGB888模式LTDC时钟选择10-20MHz即可。
	      b. 480*272分辨率的可以高些，取20MHz左右即可。
	  (2) 16位色或者8位色模式，LTDC的时钟频率一般可以比24位色或者32位色的高一倍。
*/
#define _CM_ARGB8888      1
#define _CM_RGB888        2
#define _CM_RGB565        3
#define _CM_ARGB1555      4
#define _CM_ARGB4444      5
#define _CM_L8            6
#define _CM_AL44          7
#define _CM_AL88          8

/* 7. 配置图层1的颜色模式和分辨率大小 */
#define COLOR_MODE_0      _CM_ARGB8888
#define XSIZE_0           XSIZE_PHYS
#define YSIZE_0           YSIZE_PHYS

/* 8. 配置图层2的的颜色模式和分辨率大小 */
#define COLOR_MODE_1      _CM_RGB565
#define XSIZE_1           XSIZE_PHYS
#define YSIZE_1           YSIZE_PHYS

/* 9. 单图层情况下，根据用户选择的颜色模式可自动选择图层1的emWin的驱动和颜色模式 */
#if   (COLOR_MODE_0 == _CM_ARGB8888)
  #define COLOR_CONVERSION_0 GUICC_M8888I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_32
#elif (COLOR_MODE_0 == _CM_RGB888)
  #define COLOR_CONVERSION_0 GUICC_M888
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_24
#elif (COLOR_MODE_0 == _CM_RGB565)
  #define COLOR_CONVERSION_0 GUICC_M565
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
#elif (COLOR_MODE_0 == _CM_ARGB1555)
  #define COLOR_CONVERSION_0 GUICC_M1555I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
#elif (COLOR_MODE_0 == _CM_ARGB4444)
  #define COLOR_CONVERSION_0 GUICC_M4444I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
#elif (COLOR_MODE_0 == _CM_L8)
  #define COLOR_CONVERSION_0 GUICC_8666
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
#elif (COLOR_MODE_0 == _CM_AL44)
  #define COLOR_CONVERSION_0 GUICC_1616I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_8
#elif (COLOR_MODE_0 == _CM_AL88)
  #define COLOR_CONVERSION_0 GUICC_88666I
  #define DISPLAY_DRIVER_0   GUIDRV_LIN_16
#else
  #error Illegal color mode 0!
#endif

/* 10. 双图层情况下，根据用户选择的颜色模式可自动选择图层2的emWin的驱动和颜色模式 */
#if (GUI_NUM_LAYERS > 1)

    #if   (COLOR_MODE_1 == _CM_ARGB8888)
      #define COLOR_CONVERSION_1 GUICC_M8888I
      #define DISPLAY_DRIVER_1   GUIDRV_LIN_32
    #elif (COLOR_MODE_1 == _CM_RGB888)
      #define COLOR_CONVERSION_1 GUICC_M888
      #define DISPLAY_DRIVER_1   GUIDRV_LIN_24
    #elif (COLOR_MODE_1 == _CM_RGB565)
      #define COLOR_CONVERSION_1 GUICC_M565
      #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
    #elif (COLOR_MODE_1 == _CM_ARGB1555)
      #define COLOR_CONVERSION_1 GUICC_M1555I
      #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
    #elif (COLOR_MODE_1 == _CM_ARGB4444)
      #define COLOR_CONVERSION_1 GUICC_M4444I
      #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
    #elif (COLOR_MODE_1 == _CM_L8)
      #define COLOR_CONVERSION_1 GUICC_8666
      #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
    #elif (COLOR_MODE_1 == _CM_AL44)
      #define COLOR_CONVERSION_1 GUICC_1616I
      #define DISPLAY_DRIVER_1   GUIDRV_LIN_8
    #elif (COLOR_MODE_1 == _CM_AL88)
      #define COLOR_CONVERSION_1 GUICC_88666I
      #define DISPLAY_DRIVER_1   GUIDRV_LIN_16
    #else
      #error Illegal color mode 1!
    #endif

#endif

/*11. 配置选项检测，防止配置错误或者某些选项没有配置 */
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif


/*
**********************************************************************************************************
									文件内使用的全局变量
**********************************************************************************************************
*/
/** @defgroup LCD CONFIGURATION_Private_Variables
* @{
*/
#define DEFINEDMA2D_COLORCONVERSION(PFIX, PIXELFORMAT)                                                             \
static void Color2IndexBulk_##PFIX##DMA2D(LCD_COLOR * pColor, void * pIndex, U32 NumItems, U8 SizeOfIndex) { \
  DMA2D_Color2IndexBulk(pColor, pIndex, NumItems, SizeOfIndex, PIXELFORMAT);                                         \
}                                                                                                                   \
static void Index2ColorBulk_##PFIX##DMA2D(void * pIndex, LCD_COLOR * pColor, U32 NumItems, U8 SizeOfIndex) { \
  DMA2D_Index2ColorBulk(pIndex, pColor, NumItems, SizeOfIndex, PIXELFORMAT);  \
}  
  
static U32 aBufferDMA2D [XSIZE_PHYS_1 * sizeof(U32)];
static U32 aBuffer_FG   [XSIZE_PHYS_1 * sizeof(U32)];
static U32 aBuffer_BG   [XSIZE_PHYS_1 * sizeof(U32)];  

static LCD_LayerPropTypedef          layer_prop[GUI_NUM_LAYERS];

static const LCD_API_COLOR_CONV * apColorConvAPI[] = 
{
  COLOR_CONVERSION_0,
#if GUI_NUM_LAYERS > 1
  COLOR_CONVERSION_1,
#endif
};


/** @defgroup LCD CONFIGURATION_Private_FunctionPrototypes
* @{
*/ 
static uint32_t GetBufferSize(unsigned int LayerIndex);
static uint32_t LCD_LL_GetPixelformat(unsigned int LayerIndex);
static void DMA2D_CopyBuffer(unsigned int LayerIndex, void * pSrc, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLineSrc, uint32_t OffLineDst);
static void DMA2D_FillBuffer(unsigned int LayerIndex, void * pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);
static void LCD_LL_Init(void); 
static void LCD_LL_LayerInit(unsigned int LayerIndex); 

static void CUSTOM_CopyBuffer(unsigned int LayerIndex, int IndexSrc, int IndexDst) ;
static void CUSTOM_CopyRect(unsigned int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize);
static void CUSTOM_FillRect(unsigned int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) ;

static void DMA2D_Index2ColorBulk(void * pIndex, LCD_COLOR * pColor, uint32_t NumItems, U8 SizeOfIndex, uint32_t PixelFormat);
static void DMA2D_Color2IndexBulk(LCD_COLOR * pColor, void * pIndex, uint32_t NumItems, U8 SizeOfIndex, uint32_t PixelFormat);

static void LCD_DrawBitmap8bpp(unsigned int LayerIndex, int32_t x, int32_t y, U8 const * p, int32_t xSize, int32_t ySize, int32_t BytesPerLine);
static void LCD_DrawBitmap16bpp(unsigned int LayerIndex, int32_t x, int32_t y, U16 const * p, int32_t xSize, int32_t ySize, int32_t BytesPerLine);
static void LCD_DrawBitmap32bpp(unsigned int LayerIndex, int32_t x, int32_t y, U8 const * p, int32_t xSize, int32_t ySize, int32_t BytesPerLine);
static void DMA2D_AlphaBlendingBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems);
static void DMA2D_AlphaBlending(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U32 NumItems);
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal, const GUI_BITMAP GUI_UNI_PTR * pBitmap, int LayerIndex);
static LCD_COLOR DMA2D_MixColors(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens);
static void LCD_MixColorsBulk(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens);

DEFINEDMA2D_COLORCONVERSION(M8888I, LTDC_PIXEL_FORMAT_ARGB8888)
DEFINEDMA2D_COLORCONVERSION(M888,   LTDC_PIXEL_FORMAT_ARGB8888)
//DEFINEDMA2D_COLORCONVERSION(M565,   LTDC_PIXEL_FORMAT_RGB565)
DEFINEDMA2D_COLORCONVERSION(M1555I, LTDC_PIXEL_FORMAT_ARGB1555)
DEFINEDMA2D_COLORCONVERSION(M4444I, LTDC_PIXEL_FORMAT_ARGB4444)



/**
  * @brief DMA2D MSP Initialization 
  *        This function configures the hardware resources used in this demonstration: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  * @param Dma2dHandler: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *Dma2dHandler)
{  
  /* Enable peripheral */
  __HAL_RCC_DMA2D_CLK_ENABLE();   
}

/**
  * @brief LTDC MSP De-Initialization 
  *        This function frees the hardware resources used in this demonstration:
  *          - Disable the Peripheral's clock
  * @param LtdcHandler: LTDC handle pointer
  * @retval None
  */
void HAL_LTDC_MspDeInit(LTDC_HandleTypeDef *LtdcHandler)
{
  /* Reset peripherals */
  /* Enable LTDC reset state */
  __HAL_RCC_LTDC_FORCE_RESET();
  
  /* Release LTDC from reset state */ 
  __HAL_RCC_LTDC_RELEASE_RESET();
}

/**
  * @brief DMA2D MSP De-Initialization 
  *        This function frees the hardware resources used in this demonstration:
  *          - Disable the Peripheral's clock
  * @param Dma2dHandler: DMA2D handle pointer
  * @retval None
  */
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *Dma2dHandler)
{
  /* Enable DMA2D reset state */
  __HAL_RCC_DMA2D_FORCE_RESET();
  
  /* Release DMA2D from reset state */ 
  __HAL_RCC_DMA2D_RELEASE_RESET();
}

#if defined (STM32F429xx)
/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  HAL_LTDC_IRQHandler(&LtdcHandler);
}
#endif

/**
  * @brief  Line Event callback.
  * @param  LtdcHandler: pointer to a LTDC_HandleTypeDef structure that contains
  *                the configuration information for the specified LTDC.
  * @retval None
  */
void HAL_LTDC_LineEvenCallback(LTDC_HandleTypeDef *LtdcHandler)
{
  U32 Addr;
  U32 layer;

  for (layer = 0; layer < GUI_NUM_LAYERS; layer++) 
  {
    if (layer_prop[layer].pending_buffer >= 0) 
    {
      /* Calculate address of buffer to be used  as visible frame buffer */
      Addr = layer_prop[layer].address + \
             layer_prop[layer].xSize * layer_prop[layer].ySize * layer_prop[layer].pending_buffer * layer_prop[layer].BytesPerPixel;
      
      __HAL_LTDC_LAYER(LtdcHandler, layer)->CFBAR = Addr;
     
      __HAL_LTDC_RELOAD_CONFIG(LtdcHandler);
      
      /* Notify STemWin that buffer is used */
      GUI_MULTIBUF_ConfirmEx(layer, layer_prop[layer].pending_buffer);

      /* Clear pending buffer flag of layer */
      layer_prop[layer].pending_buffer = -1;
    }
  }
  
  HAL_LTDC_ProgramLineEvent(LtdcHandler, 0);
}

/*
*********************************************************************************************************
*	函 数 名: LCD_LL_GetPixelformat
*	功能说明: 获取图层1或者图层2使用的颜色格式
*	形    参: LayerIndex  图层
*	返 回 值: 颜色格式
*********************************************************************************************************
*/
static uint32_t LCD_LL_GetPixelformat(unsigned int LayerIndex)
{
    const LCD_API_COLOR_CONV * pColorConvAPI;
    if (LayerIndex>=GUI_NUM_LAYERS) return 0;
    pColorConvAPI=layer_prop[LayerIndex].pColorConvAPI;
    if(pColorConvAPI==GUICC_M8888I) return LTDC_PIXEL_FORMAT_ARGB8888;
    else if (pColorConvAPI==GUICC_M888) return LTDC_PIXEL_FORMAT_RGB888;
    else if (pColorConvAPI==GUICC_M565) return LTDC_PIXEL_FORMAT_RGB565;
    else if (pColorConvAPI==GUICC_M1555I) return LTDC_PIXEL_FORMAT_ARGB1555;
    else if (pColorConvAPI==GUICC_M4444I) return LTDC_PIXEL_FORMAT_ARGB4444;
    return 1;
}
/*
*********************************************************************************************************
*	函 数 名: DMA2D_CopyBuffer
*	功能说明: 通过DMA2D从前景层复制指定区域的颜色数据到目标区域
*	形    参: LayerIndex    图层
*             pSrc          颜色数据源地址
*             pDst          颜色数据目的地址
*             xSize         要复制区域的X轴大小，即每行像素数
*             ySize         要复制区域的Y轴大小，即行数
*             OffLineSrc    前景层图像的行偏移
*             OffLineDst    输出的行偏移
*	返 回 值: 无
*********************************************************************************************************
*/
static void DMA2D_CopyBuffer(unsigned int LayerIndex,void * pSrc,void * pDst,uint32_t xSize,uint32_t ySize,uint32_t OffLineSrc,uint32_t OffLineDst)
{
    uint32_t PixelFormat;
    U32 timeout=0;
    PixelFormat=LCD_LL_GetPixelformat(LayerIndex);  //获取像素格式
    DMA2D->CR=0x00000000UL|(1<<9);                  //存储器到存储器
    DMA2D->FGMAR=(uint32_t)pSrc;                    //设置前景层存储器地址                   
    DMA2D->OMAR=(uint32_t)pDst;                     //设置输出存储器地址    
    DMA2D->FGOR=OffLineSrc;                         //设置前景层偏移
    DMA2D->OOR=OffLineDst;                          //设置输出偏移
    DMA2D->FGPFCCR=PixelFormat;                     //设置颜色模式
    DMA2D->NLR=(uint32_t)(xSize<<16)|(U16)ySize;    //设置每行像素数和行数
    DMA2D->CR|=DMA2D_CR_START;                      //启动DMA2D传输
    while(DMA2D->CR&DMA2D_CR_START)                 //等待传输完成 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	                //超时退出
    }
}

/*
*********************************************************************************************************
*	函 数 名: DMA2D_FillBuffer
*	功能说明: 通过DMA2D对于指定区域进行颜色填充
*	形    参: LayerIndex    图层
*             pDst          颜色数据目的地址
*             xSize         要复制区域的X轴大小，即每行像素数
*             ySize         要复制区域的Y轴大小，即行数
*             OffLine       前景层图像的行偏移
*             ColorIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void DMA2D_FillBuffer(unsigned int LayerIndex,void * pDst,uint32_t xSize,uint32_t ySize,uint32_t OffLine,uint32_t ColorIndex) 
{
    uint32_t PixelFormat;
    U32 timeout=0;
    PixelFormat=LCD_LL_GetPixelformat(LayerIndex);  //获取像素格式
    DMA2D->CR=0x00030000UL|(1<<9);                  //寄存器到存储器 
    DMA2D->OCOLR=ColorIndex;                        //设置要填充的颜色                   
    DMA2D->OMAR=(uint32_t)pDst;                     //设置输出寄存器地址
    DMA2D->OOR=OffLine;                             //设置输出偏移
    DMA2D->OPFCCR=PixelFormat;                      //设置颜色格式
    DMA2D->NLR=(uint32_t)(xSize << 16)|(U16)ySize;  //设置每行像素数和行数
    DMA2D->CR|=DMA2D_CR_START;                      //启动DMA2D传输
    while(DMA2D->CR&DMA2D_CR_START)                 //等待传输完成 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	                //超时退出
    }
}
/*
*********************************************************************************************************
*	函 数 名: _GetBufferSize
*	功能说明: 获取指定层显存大小
*	形    参: LayerIndex    图层
*	返 回 值: 显存大小
*********************************************************************************************************
*/
static uint32_t GetBufferSize(unsigned int LayerIndex) 
{
    uint32_t BufferSize;
    BufferSize = layer_prop[LayerIndex].xSize*layer_prop[LayerIndex].ySize*layer_prop[LayerIndex].BytesPerPixel;
    return BufferSize;
}

/*
*********************************************************************************************************
*	函 数 名: CUSTOM_CopyBuffer
*	功能说明: 此函数用于多缓冲，将一个缓冲中的所有数据复制到另一个缓冲。
*	形    参: LayerIndex    图层
*             IndexSrc      源缓冲序号
*             IndexDst      目标缓冲序号
*	返 回 值: 无
*********************************************************************************************************
*/
static void CUSTOM_CopyBuffer(unsigned int LayerIndex, int IndexSrc, int IndexDst) 
{
	U32 BufferSize, AddrSrc, AddrDst;

	BufferSize = GetBufferSize(LayerIndex);
    AddrSrc    = layer_prop[LayerIndex].address + BufferSize * IndexSrc;
    AddrDst    = layer_prop[LayerIndex].address + BufferSize * IndexDst;

    DMA2D_CopyBuffer(LayerIndex, (void *)AddrSrc, (void *)AddrDst, layer_prop[LayerIndex].xSize, layer_prop[LayerIndex].ySize, 0, 0);
	layer_prop[LayerIndex].buffer_index = IndexDst; // After this function has been called all drawing operations are routed to Buffer[IndexDst]!
}

/*
*********************************************************************************************************
*	函 数 名: CUSTOM_CopyRect
*	功能说明: 此函数用于多缓冲，将一个缓冲中指定区域数据复制到另一个缓冲。
*	形    参: LayerIndex    图层
*             x0            源缓冲x轴位置
*             y0            源缓冲y轴位置
*             x1            目标冲x轴位置
*             y1            目标冲y轴位置
*             xSize         要复制的x轴大小
*             ySize         要复制的y轴大小
*	返 回 值: 无
*********************************************************************************************************
*/
static void CUSTOM_CopyRect(unsigned int LayerIndex, int x0, int y0, int x1, int y1, int xSize, int ySize) 
{
	U32 AddrSrc, AddrDst; 

    AddrSrc = layer_prop[LayerIndex].address + (y0 * layer_prop[LayerIndex].xSize + x0) * layer_prop[LayerIndex].BytesPerPixel;
    AddrDst = layer_prop[LayerIndex].address + (y1 * layer_prop[LayerIndex].xSize + x1) * layer_prop[LayerIndex].BytesPerPixel;
	DMA2D_CopyBuffer(LayerIndex, (void *)AddrSrc, (void *)AddrDst, xSize, ySize, layer_prop[LayerIndex].xSize - xSize, layer_prop[LayerIndex].xSize - xSize);
}

/*
*********************************************************************************************************
*	函 数 名: CUSTOM_FillRect
*	功能说明: 对指定的区域进行颜色填充
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void CUSTOM_FillRect(unsigned int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex) 
{
  U32 BufferSize, AddrDst;
  int xSize, ySize;

  if (GUI_GetDrawMode() == GUI_DM_XOR) {
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, NULL);
    LCD_FillRect(x0, y0, x1, y1);
    LCD_SetDevFunc(LayerIndex, LCD_DEVFUNC_FILLRECT, (void(*)(void))CUSTOM_FillRect);
  } else {
    xSize = x1 - x0 + 1;
    ySize = y1 - y0 + 1;
    BufferSize = GetBufferSize(LayerIndex);
    AddrDst = layer_prop[LayerIndex].address + BufferSize * layer_prop[LayerIndex].buffer_index + (y0 * layer_prop[LayerIndex].xSize + x0) * layer_prop[LayerIndex].BytesPerPixel;
    DMA2D_FillBuffer(LayerIndex, (void *)AddrDst, xSize, ySize, layer_prop[LayerIndex].xSize - xSize, PixelIndex);
  }
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
//DMA2D的CLUT加载
//pColor:CLUT存储器地址
//NumItems:CLUT大小
static void DMA2D_LoadLUT(LCD_COLOR * pColor,uint32_t NumItems) 
{
    DMA2D->FGCMAR=(uint32_t)pColor; //设置前景层CLUT存储器地址
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_RGB888|((NumItems-1)&0xFF)<<8;//设置颜色模式和CLUT大小
    DMA2D->FGPFCCR|=(1<<5);         //启动加载
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
//交换指定地址颜色的RGB顺序并且翻转alpha值并将新得到的颜色值复制到目标地址中
//pColorSrc:源地址
//pColorDst:目的地址
//NumItems:要处理的数量
static void InvertAlpha_SwapRB(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
    uint32_t Color;
    do 
    {
        Color=*pColorSrc++;
        *pColorDst++=((Color&0x000000FF)<<16)           //交换红色(R)和蓝色(B)的位置
                    |(Color&0x0000FF00)                 //绿色(G)的位置不变
                    |((Color&0x00FF0000)>>16)           //交换红色(R)和蓝色(B)的位置
                    |((Color&0xFF000000)^0xFF000000);   //翻转alpha值
    } while (--NumItems);
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
//反转alpha，因为DMA2D的颜色格式和STemWin的颜色格式不同，因此需要翻转alpha值使其适配STemWin
//pColorSrc:源地址
//pColorDst:目的地址
//NumItems:要处理的数量
static void InvertAlpha(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
    uint32_t Color;
    do 
    {
        Color = *pColorSrc++;
        *pColorDst++=Color^0xFF000000;//翻转alpha
    } while (--NumItems);
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
//alpha混色，使用DMA2D
//pColorFG:前景层存储器地址 
//pColorBG:背景层存储器地址
//pColorDst:输出存储器地址
//NumItems:行数
static void DMA2D_AlphaBlendingBulk(LCD_COLOR *pColorFG,LCD_COLOR *pColorBG,LCD_COLOR *pColorDst,U32 NumItems) 
{
    U32 timeout=0;
    DMA2D->CR=0x00020000UL|(1<<9);              //存储器到存储器并执行混合
    DMA2D->FGMAR=(uint32_t)pColorFG;            //设置前景层存储器地址
    DMA2D->BGMAR=(uint32_t)pColorBG;            //设置背景层存储器地址
    DMA2D->OMAR=(uint32_t)pColorDst;            //设置输出存储器地址
    DMA2D->FGOR=0;                              //设置前景层偏移
    DMA2D->BGOR=0;                              //设置背景层偏移
    DMA2D->OOR=0;                               //设置输出偏移
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;  //设置前景层颜色格式
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;  //设置背景层颜色格式
    DMA2D->OPFCCR =LTDC_PIXEL_FORMAT_ARGB8888;  //设置输出颜色格式
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //设置行数 
    DMA2D->CR|=DMA2D_CR_START;                  //启动DMA2D传输
    while(DMA2D->CR&DMA2D_CR_START)             //等待传输完成 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //超时退出
    }

}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
//混合两种颜色,混合前景色和背景色
//Color:前景色
//BkColor:背景色
//Intens:混合强度
//返回值:混合后的颜色
static LCD_COLOR DMA2D_MixColors(LCD_COLOR Color,LCD_COLOR BkColor,U8 Intens) 
{
    uint32_t ColorFG,ColorBG,ColorDst;
    U32 timeout=0;
    if((BkColor&0xFF000000)==0xFF000000)return Color;//背景色透明，不需要混色
    ColorFG=Color^0xFF000000;
    ColorBG=BkColor^0xFF000000;
  
    DMA2D->CR=0x00020000UL|(1<<9);              //存储器到存储器并执行混合  
    DMA2D->FGMAR=(uint32_t)&ColorFG;            //设置前景层存储器地址
    DMA2D->BGMAR=(uint32_t)&ColorBG;            //设置背景层存储器地址
    DMA2D->OMAR=(uint32_t)&ColorDst;            //设置输出存储器地址
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888|(1UL<<16)|((uint32_t)Intens<<24);         //设置前景层颜色格式
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888|(0UL<<16)|((uint32_t)(255-Intens)<<24);   //设置背景层颜色格式
    DMA2D->OPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;   //设置背景层颜色格式
    DMA2D->NLR=(uint32_t)(1<<16)|1;             //设置每行像素数和行数 
    DMA2D->CR|=DMA2D_CR_START;                  //启动DMA2D传输
    while(DMA2D->CR&DMA2D_CR_START)             //等待传输完成 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //超时退出
    }
    return (ColorDst^0xFF000000);               //返回混合后的颜色
}

//使用DMA2D进行颜色转换
//pSrc:源颜色(前景色)
//pDst:转换后的颜色(输出颜色)
//PixelFormatSrc:前景层颜色格式
//PixelFormatDst:输出颜色格式
//NumItems:每行像素数
/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void DMA2D_ConvertColor(void * pSrc,void * pDst,uint32_t PixelFormatSrc,uint32_t PixelFormatDst,uint32_t NumItems)
{
    U32 timeout=0;
    DMA2D->CR=0x00010000UL|(1<<9);              //存储器到存储器并执行FPC
    DMA2D->FGMAR=(uint32_t)pSrc;                //前景层存储器地址
    DMA2D->OMAR=(uint32_t)pDst;                 //输出存储器地址
    DMA2D->FGOR=0;                              //背景层偏移
    DMA2D->OOR=0;                               //输出偏移
    DMA2D->FGPFCCR=PixelFormatSrc;              //前景层颜色格式
    DMA2D->OPFCCR=PixelFormatDst;               //输出颜色格式
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //设置每行像素数和行数
    DMA2D->CR|=DMA2D_CR_START;                  //启动DMA2D传输
    while(DMA2D->CR&DMA2D_CR_START)             //等待传输完成 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //超时退出
    }
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static LCD_PIXELINDEX * _LCD_GetpPalConvTable(const LCD_LOGPALETTE GUI_UNI_PTR * pLogPal,const GUI_BITMAP GUI_UNI_PTR * pBitmap,int LayerIndex)
{
    void (* pFunc)(void);
    int32_t DoDefault = 0;

    if (pBitmap->BitsPerPixel == 8) 
    {
        pFunc=LCD_GetDevFunc(LayerIndex, LCD_DEVFUNC_DRAWBMP_8BPP);
        if(pFunc) 
        {
            if(pBitmap->pPal) 
            {
                if(pBitmap->pPal->HasTrans) DoDefault = 1;
            }else DoDefault = 1;  
        }else DoDefault = 1;  
    } 
    else DoDefault = 1;
    if (DoDefault) return LCD_GetpPalConvTable(pLogPal);
    InvertAlpha_SwapRB((U32 *)pLogPal->pPalEntries, aBufferDMA2D, pLogPal->NumEntries);
    DMA2D_LoadLUT(aBufferDMA2D, pLogPal->NumEntries);
    return aBufferDMA2D;
}

//使用DMA2D进行颜色混合
//pColorFG:前景色存储器地址
//pColorBG:背景色存储器地址
//pColorDst:输出存储器地址
//Intens:混合强度
//NumItems:每行像素数
/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void DMA2D_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, uint32_t NumItems)
{
    U32 timeout=0;
    DMA2D->CR=0x00020000UL|(1<<9);              //存储器到存储器并执行混合
    DMA2D->FGMAR=(uint32_t)pColorFG;            //设置前景色存储器地址
    DMA2D->BGMAR=(uint32_t)pColorBG;            //设置背景色存储器地址
    DMA2D->OMAR=(uint32_t)pColorDst;            //设置输出存储器地址
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888   //设置前景色颜色格式
                    |(1UL<<16)
                    |((uint32_t)Intens<<24);
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888   //设置背景色颜色格式
                    |(0UL<<16)
                    |((uint32_t)(255-Intens)<<24);    
    DMA2D->OPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;   //设置输出颜色格式
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //设置每行像素数和行数    
    DMA2D->CR|=DMA2D_CR_START;                  //启动DMA2D传输
    while(DMA2D->CR&DMA2D_CR_START)             //等待传输完成 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //超时退出
    }
}



//使用DMA2D进行alpha混色
//pColorFG:前景色
//pColorBG:背景色
//pColorDst:输出颜色
//NumItems:每行像素数
/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void DMA2D_AlphaBlending(LCD_COLOR *pColorFG,LCD_COLOR *pColorBG,LCD_COLOR *pColorDst,U32 NumItems)
{
    InvertAlpha(pColorFG,aBuffer_FG,NumItems);
    InvertAlpha(pColorBG,aBuffer_BG,NumItems);
    DMA2D_AlphaBlendingBulk(aBuffer_FG,aBuffer_BG,aBufferDMA2D,NumItems);
    InvertAlpha(aBufferDMA2D,pColorDst,NumItems);
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色索引转换为颜色值
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void DMA2D_Index2ColorBulk(void *pIndex,LCD_COLOR *pColor,uint32_t NumItems, U8 SizeOfIndex,uint32_t PixelFormat)
{
    DMA2D_ConvertColor(pIndex,aBufferDMA2D,PixelFormat,LTDC_PIXEL_FORMAT_ARGB8888,NumItems);
    InvertAlpha_SwapRB(aBufferDMA2D,pColor,NumItems);
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 颜色值转换为颜色索引
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void DMA2D_Color2IndexBulk(LCD_COLOR *pColor,void *pIndex,uint32_t NumItems,U8 SizeOfIndex,uint32_t PixelFormat) 
{
    InvertAlpha_SwapRB(pColor,aBufferDMA2D,NumItems);
    DMA2D_ConvertColor(aBufferDMA2D,pIndex,LTDC_PIXEL_FORMAT_ARGB8888,PixelFormat,NumItems);
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 对指定的区域进行颜色填充
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void LCD_MixColorsBulk(U32 *pFG,U32 *pBG,U32 *pDst,unsigned OffFG,unsigned OffBG,unsigned OffDest,unsigned xSize,unsigned ySize,U8 Intens)
{
    int32_t y;

    GUI_USE_PARA(OffFG);
    GUI_USE_PARA(OffDest);
    for(y=0;y<ySize;y++) 
    {
        InvertAlpha(pFG,aBuffer_FG,xSize);
        InvertAlpha(pBG,aBuffer_BG,xSize);
        DMA2D_MixColorsBulk(aBuffer_FG,aBuffer_BG,aBufferDMA2D,Intens,xSize);
        InvertAlpha(aBufferDMA2D,pDst,xSize);
        pFG+=xSize+OffFG;
        pBG+=xSize+OffBG;
        pDst+=xSize+OffDest;
    }
}

/*
*********************************************************************************************************
*	函 数 名: _DMA_DrawBitmapL8
*	功能说明: 对指定的区域进行颜色填充
*	形    参: LayerIndex    图层
*             x0            起始x轴位置
*             y0            起始y轴位置
*             x1            结束x轴位置
*             y1            结束y轴位置
*             PixelIndex    要填充的颜色值
*	返 回 值: 无
*********************************************************************************************************
*/
static void DMA2D_DrawBitmapL8(void * pSrc, void * pDst,  uint32_t OffSrc, uint32_t OffDst, uint32_t PixelFormatDst, uint32_t xSize, uint32_t ySize)
{
    U32 timeout=0;
    DMA2D->CR=0x00010000UL|(1<<9);    
    DMA2D->FGMAR=(uint32_t)pSrc;                    
    DMA2D->OMAR=(uint32_t)pDst;                       
    DMA2D->FGOR=OffSrc;                      
    DMA2D->OOR=OffDst;                          
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_L8;             
    DMA2D->OPFCCR=PixelFormatDst;           
    DMA2D->NLR=(uint32_t)(xSize<<16)|ySize;      
    DMA2D->CR|=DMA2D_CR_START;                  //启动DMA2D传输
    while(DMA2D->CR&DMA2D_CR_START)             //等待传输完成 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //超时退出
    }
}

/*
*********************************************************************************************************
*	函 数 名: _LCD_DrawBitmap8bpp
*	功能说明: 8bpp位图绘制
*	形    参: --
*	返 回 值: 无
*********************************************************************************************************
*/
static void LCD_DrawBitmap8bpp(unsigned int LayerIndex, int32_t x, int32_t y, U8 const * p, int32_t xSize, int32_t ySize, int32_t BytesPerLine)
{ 
    uint32_t BufferSize, AddrDst;
    int32_t OffLineSrc, OffLineDst;
    uint32_t PixelFormat;

    BufferSize=GetBufferSize(LayerIndex);
    AddrDst=layer_prop[LayerIndex].address+BufferSize*layer_prop[LayerIndex].buffer_index+(y*layer_prop[LayerIndex].xSize + x)*layer_prop[LayerIndex].BytesPerPixel;
    OffLineSrc=BytesPerLine-xSize;
    OffLineDst=layer_prop[LayerIndex].xSize-xSize;
    PixelFormat=LCD_LL_GetPixelformat(LayerIndex);
    DMA2D_DrawBitmapL8((void *)p,(void *)AddrDst,OffLineSrc,OffLineDst,PixelFormat,xSize,ySize);
}

/*
*********************************************************************************************************
*	函 数 名: _LCD_DrawBitmap16bpp
*	功能说明: 16bpp位图绘制
*	形    参: --
*	返 回 值: 无
*********************************************************************************************************
*/
static void LCD_DrawBitmap16bpp(unsigned int LayerIndex,int32_t x,int32_t y,U16 const * p,int32_t xSize,int32_t ySize,int32_t BytesPerLine)
{
    uint32_t BufferSize, AddrDst;
    int32_t OffLineSrc, OffLineDst;

    BufferSize=GetBufferSize(LayerIndex);
    AddrDst=layer_prop[LayerIndex].address+BufferSize*layer_prop[LayerIndex].buffer_index+(y*layer_prop[LayerIndex].xSize+x)*layer_prop[LayerIndex].BytesPerPixel;
    OffLineSrc=(BytesPerLine/2)-xSize;
    OffLineDst=layer_prop[LayerIndex].xSize-xSize;
    DMA2D_CopyBuffer(LayerIndex,(void *)p,(void *)AddrDst,xSize,ySize,OffLineSrc,OffLineDst);
}

/*
*********************************************************************************************************
*	函 数 名: _LCD_DrawBitmap32bpp
*	功能说明: 32bpp位图绘制
*	形    参: --
*	返 回 值: 无
*********************************************************************************************************
*/
static void LCD_DrawBitmap32bpp(unsigned int LayerIndex, int32_t x, int32_t y, U8 const * p, int32_t xSize, int32_t ySize, int32_t BytesPerLine)
{
  U32 BufferSize, AddrDst;
  int OffLineSrc, OffLineDst;

  BufferSize = GetBufferSize(LayerIndex);
  AddrDst = layer_prop[LayerIndex].address + BufferSize * layer_prop[LayerIndex].buffer_index + (y * layer_prop[LayerIndex].xSize + x) * layer_prop[LayerIndex].BytesPerPixel;
  OffLineSrc = (BytesPerLine / 4) - xSize;
  OffLineDst = layer_prop[LayerIndex].xSize - xSize;
  DMA2D_CopyBuffer(LayerIndex, (void *)p, (void *)AddrDst, xSize, ySize, OffLineSrc, OffLineDst);
}


/*
*********************************************************************************************************
*	函 数 名: _LCD_DisplayOn
*	功能说明: 打开LCD
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void _LCD_DisplayOn(void) 
{
    BSP_LcdOpenBl();
    __HAL_LTDC_ENABLE(&LtdcHandler);
}

/*
*********************************************************************************************************
*	函 数 名: _LCD_DisplayOff
*	功能说明: 关闭LCD
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void _LCD_DisplayOff(void) 
{
     BSP_LcdCloseBl();
    __HAL_LTDC_DISABLE(&LtdcHandler);
}

/*
*********************************************************************************************************
*	函 数 名: DMA2D_IRQHandler
*	功能说明: DMA2D传输完成中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void DMA2D_IRQHandler(void) 
{
	DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
}

/*
*********************************************************************************************************
*	函 数 名: DMA2D_IRQHandler
*	功能说明: DMA2D传输完成中断服务程序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void LCD_LL_Init(void) 
{
//    //LTDC中断，抢占优先级1，子优先级1
//    HAL_NVIC_SetPriority(LTDC_IRQn,1,1);    
//    HAL_NVIC_EnableIRQ(LTDC_IRQn);
//    HAL_LTDC_ProgramLineEvent(&LtdcHandler,0);//开启LTDC的行中断
//  
//    //DMA2D默认设置
//    Dma2dHandler.Instance=DMA2D; 
//    Dma2dHandler.Init.Mode=DMA2D_R2M;          //内存到存储器模式
//    Dma2dHandler.Init.ColorMode=DMA2D_RGB565;  //RGB565模式
//    Dma2dHandler.Init.OutputOffset=0x0;        //输出偏移为0    
//    HAL_DMA2D_Init(&Dma2dHandler);
}

/**
  * @brief  Initialize the LCD Controller.
  * @param  LayerIndex : layer Index.
  * @retval None
  */
static void LCD_LL_LayerInit(unsigned int LayerIndex) 
{
    LTDC_LayerCfgTypeDef             layer_cfg;
    uint32_t i;
    static uint32_t LUT[256];
    if (LayerIndex < GUI_NUM_LAYERS) 
    {
        /* Layer configuration */
        layer_cfg.WindowX0 = 0;
        layer_cfg.WindowX1 = XSIZE_PHYS;
        layer_cfg.WindowY0 = 0;
        layer_cfg.WindowY1 = YSIZE_PHYS; 
        layer_cfg.PixelFormat = LCD_LL_GetPixelformat(LayerIndex);
        layer_cfg.FBStartAdress = layer_prop[LayerIndex].address;
        layer_cfg.Alpha = 255;
        layer_cfg.Alpha0 = 0;
        layer_cfg.Backcolor.Blue = 0;
        layer_cfg.Backcolor.Green = 0;
        layer_cfg.Backcolor.Red = 0;
        layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
        layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
        layer_cfg.ImageWidth = XSIZE_PHYS;
        layer_cfg.ImageHeight = YSIZE_PHYS;
        HAL_LTDC_ConfigLayer(&LtdcHandler, &layer_cfg, LayerIndex);  

        /* Enable LUT on demand */
        if (LCD_GetBitsPerPixelEx(LayerIndex) <= 8) 
        {
          /* Enable usage of LUT for all modes with <= 8bpp*/
          HAL_LTDC_EnableCLUT(&LtdcHandler, LayerIndex);
        } 
        else 
        {
            if (layer_prop[LayerIndex].pColorConvAPI==GUICC_88666I) 
            {
                HAL_LTDC_EnableCLUT(&LtdcHandler,LayerIndex);
                for (i=0;i<256;i++) 
                {
                    LUT[i]=LCD_API_ColorConv_8666.pfIndex2Color(i);
                }
                HAL_LTDC_ConfigCLUT(&LtdcHandler,LUT,256,LayerIndex);
            }
        }
    } 
}

/*
*********************************************************************************************************
*	函 数 名: LCD_X_Config
*	功能说明: LCD配置
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void LCD_X_Config(void) 
{
	int i;
    
    LCD_LL_Init();
    
	//
	// At first initialize use of multiple buffers on demand
	//
	#if (NUM_BUFFERS > 1)
		for (i = 0; i < GUI_NUM_LAYERS; i++) 
		{
			GUI_MULTIBUF_ConfigEx(i, NUM_BUFFERS);
		}
	#endif
		
	//
	// Set display driver and color conversion for 1st layer
	//
	GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_0, COLOR_CONVERSION_0, 0, 0);
		
    /* Set size of 1st layer */
    if (t_LcdPara.Dir == 0) {
        LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
        LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
        GUI_SetOrientation(GUI_SWAP_XY|GUI_MIRROR_Y); 
    } else {
        LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
        LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
    }
	#if (GUI_NUM_LAYERS > 1)
		//
		// Set display driver and color conversion for 2nd layer
		//
		GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER_1, COLOR_CONVERSION_1, 0, 1);
		
		//
		// Set size of 2nd layer
		//
        if (t_LcdPara.Dir == 0) {
            LCD_SetSizeEx (1, YSIZE_PHYS, XSIZE_PHYS);
            LCD_SetVSizeEx(1, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
        } else {
            LCD_SetSizeEx (1, XSIZE_PHYS, YSIZE_PHYS);
            LCD_SetVSizeEx(1, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
        }
	#endif

    /*Initialize GUI Layer structure */
    layer_prop[0].address = LCD_LAYER0_FRAME_BUFFER;
    
#if (GUI_NUM_LAYERS > 1)    
    layer_prop[1].address = LCD_LAYER1_FRAME_BUFFER; 
#endif
	//
	// Setting up VRam address and custom functions for CopyBuffer-, CopyRect- and FillRect operations
	//
	for (i = 0; i < GUI_NUM_LAYERS; i++) 
	{
        layer_prop[i].pColorConvAPI = (LCD_API_COLOR_CONV *)apColorConvAPI[i];
         
        layer_prop[i].pending_buffer = -1;

        /* Set VRAM address */
        LCD_SetVRAMAddrEx(i, (void *)(layer_prop[i].address));

        /* Remember color depth for further operations */
        layer_prop[i].BytesPerPixel = LCD_GetBitsPerPixelEx(i) >> 3;
		
		//
		// Set custom functions for several operations
		//
		LCD_SetDevFunc(i, LCD_DEVFUNC_COPYBUFFER, (void(*)(void))CUSTOM_CopyBuffer);
		LCD_SetDevFunc(i, LCD_DEVFUNC_COPYRECT,   (void(*)(void))CUSTOM_CopyRect);
		
		//
		// Filling via DMA2D does only work with 16bpp or more
		//
		if (LCD_LL_GetPixelformat(i) <= LTDC_PIXEL_FORMAT_ARGB4444) 
		{
			LCD_SetDevFunc(i, LCD_DEVFUNC_FILLRECT, (void(*)(void))CUSTOM_FillRect);
			LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_8BPP, (void(*)(void))LCD_DrawBitmap8bpp); 
		}
		
		//
		// Set up drawing routine for 16bpp bitmap using DMA2D
		//
		if (LCD_LL_GetPixelformat(i) == LTDC_PIXEL_FORMAT_RGB565) 
		{
			LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_16BPP, (void(*)(void))LCD_DrawBitmap16bpp);     // Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565
		}

		//
		// Set up drawing routine for 32bpp bitmap using DMA2D
		//
		if (LCD_LL_GetPixelformat(i) == LTDC_PIXEL_FORMAT_ARGB8888) 
		{
			LCD_SetDevFunc(i, LCD_DEVFUNC_DRAWBMP_32BPP, (void(*)(void))LCD_DrawBitmap32bpp);     // Set up drawing routine for 16bpp bitmap using DMA2D. Makes only sense with RGB565
		}
        
        GUICC_M1555I_SetCustColorConv(Color2IndexBulk_M1555IDMA2D,Index2ColorBulk_M1555IDMA2D); 
        //GUICC_M565_SetCustColorConv(Color2IndexBulk_M565DMA2D,Index2ColorBulk_M565DMA2D);  
        GUICC_M4444I_SetCustColorConv(Color2IndexBulk_M4444IDMA2D,Index2ColorBulk_M4444IDMA2D); 
        GUICC_M888_SetCustColorConv(Color2IndexBulk_M888DMA2D,Index2ColorBulk_M888DMA2D);   
        GUICC_M8888I_SetCustColorConv(Color2IndexBulk_M8888IDMA2D,Index2ColorBulk_M8888IDMA2D);
      
        GUI_SetFuncAlphaBlending(DMA2D_AlphaBlending);                                            
        GUI_SetFuncGetpPalConvTable(_LCD_GetpPalConvTable);
        GUI_SetFuncMixColors(DMA2D_MixColors);
        GUI_SetFuncMixColorsBulk(LCD_MixColorsBulk);

	}
}

/*
*********************************************************************************************************
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*
* Return Value:
*   < -1 - Error
*     -1 - Command not handled
*      0 - Ok
*********************************************************************************************************
*/
int LCD_X_DisplayDriver(unsigned int LayerIndex, unsigned Cmd, void * pData) 
{
    int r = 0;
    U32 addr;
    int xPos, yPos;
    U32 Color;

	switch (Cmd) 
	{
		case LCD_X_INITCONTROLLER: 
			{
				//
				// Called during the initialization process in order to set up the display controller and put it into operation.
				//
				LCD_LL_LayerInit(LayerIndex);
				break;
			}
			
		case LCD_X_SETORG: 
			{
                addr = layer_prop[LayerIndex].address + ((LCD_X_SETORG_INFO *)pData)->yPos * layer_prop[LayerIndex].xSize * layer_prop[LayerIndex].BytesPerPixel;
                HAL_LTDC_SetAddress(&LtdcHandler, addr, LayerIndex);
				break;
			}
			
		case LCD_X_SHOWBUFFER: 
			{
                layer_prop[LayerIndex].pending_buffer = ((LCD_X_SHOWBUFFER_INFO *)pData)->Index;
				break;
			}
			
		case LCD_X_SETLUTENTRY: 
			{
                HAL_LTDC_ConfigCLUT(&LtdcHandler, (uint32_t *)&(((LCD_X_SETLUTENTRY_INFO *)pData)->Color), 1, LayerIndex);
				break;
			}
		case LCD_X_ON: 
			{
				//
				// Required if the display controller should support switching on and off
				//
				_LCD_DisplayOn();
				break;
			}
			
		case LCD_X_OFF:
			{
				//
				// Required if the display controller should support switching on and off
				//
                _LCD_DisplayOff();
				
				break;
			}
			
		case LCD_X_SETVIS:
			{
                if(((LCD_X_SETVIS_INFO *)pData)->OnOff  == ENABLE )
                {
                  __HAL_LTDC_LAYER_ENABLE(&LtdcHandler, LayerIndex); 
                }
                else
                {
                  __HAL_LTDC_LAYER_DISABLE(&LtdcHandler, LayerIndex); 
                }
                __HAL_LTDC_RELOAD_CONFIG(&LtdcHandler); 
				break;
			}
			
		case LCD_X_SETPOS: 
			{
                HAL_LTDC_SetWindowPosition(&LtdcHandler, 
                                           ((LCD_X_SETPOS_INFO *)pData)->xPos, 
                                           ((LCD_X_SETPOS_INFO *)pData)->yPos, 
                                           LayerIndex);
				break;
			}
			
		case LCD_X_SETSIZE: 
			{
                GUI_GetLayerPosEx(LayerIndex, &xPos, &yPos);
                layer_prop[LayerIndex].xSize = ((LCD_X_SETSIZE_INFO *)pData)->xSize;
                layer_prop[LayerIndex].ySize = ((LCD_X_SETSIZE_INFO *)pData)->ySize;
                HAL_LTDC_SetWindowPosition(&LtdcHandler, xPos, yPos, LayerIndex);
				break;
			}
			
		case LCD_X_SETALPHA: 
			{
                HAL_LTDC_SetAlpha(&LtdcHandler, ((LCD_X_SETALPHA_INFO *)pData)->Alpha, LayerIndex);
				break;
			}
			
		case LCD_X_SETCHROMAMODE: 
			{
                if(((LCD_X_SETCHROMAMODE_INFO *)pData)->ChromaMode != 0)
                {
                  HAL_LTDC_EnableColorKeying(&LtdcHandler, LayerIndex);
                }
                else
                {
                  HAL_LTDC_DisableColorKeying(&LtdcHandler, LayerIndex);      
                }
				break;
			}
			
		case LCD_X_SETCHROMA: 
			{
                Color = ((((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0xFF0000) >> 16) |\
                         (((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0x00FF00) |\
                        ((((LCD_X_SETCHROMA_INFO *)pData)->ChromaMin & 0x0000FF) << 16);
                
                HAL_LTDC_ConfigColorKeying(&LtdcHandler, Color, LayerIndex);
				break;
			}
		
		default:
			r = -1;
	}
	
	return r;
}




/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


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
									�û��������õ�ѡ��
**********************************************************************************************************
*/
/* 0. �ڹٷ�����Ļ����������Ż����ٷ��Ĳ��ֺ���Ч�ʵͣ����ڴ�, 0��ʾ�Ż� */
#define emWin_Optimize   0

/* 
  1. ��ʾ��������ֱ��ʣ������Ѿ�������ʾ������Ӧ��֧��4.3�磬5���7����
     ������д����Ӧ��ʾ���е����ֱ��ʡ�
*/
#define XSIZE_PHYS       t_LcdPara.Width
#define YSIZE_PHYS       t_LcdPara.Height

#define XSIZE_PHYS_1       800



/* 2. �໺�� / ���������໺�������������ͬʱʹ�ã�emWin��֧�� */
#define NUM_BUFFERS      3 /* ����໺�����������������1,2��3��Ҳ�������֧�������� */
#define NUM_VSCREENS     1 /* �������������� */

/* 3. û��ͼ�㼤��ʱ������ɫ����, ��ʱδ�õ� */
#define BK_COLOR         GUI_DARKBLUE

/* 
   4. �ض���ͼ����������STM32F429/439���û�����ѡ��һ��ͼ���������ͼ�㣬��֧����ͼ�� 
      (1). ����GUI_NUM_LAYERS = 1ʱ������ʹ��ͼ��1ʱ��Ĭ�ϴ���ֵ�Ƿ��͸�ͼ��1�ġ�
	  (2). ����GUI_NUM_LAYERS = 2ʱ����ͼ��1��ͼ��2���Ѿ�ʹ�ܣ���ʱͼ��2�Ƕ��㣬
	       �û���Ҫ�����Լ���ʹ������������������ط���
		   a. ��bsp_touch.c�ļ��еĺ���TOUCH_InitHard�������ò���State.Layer = 1��1�ͱ�ʾ
		      ��ͼ��2���ʹ���ֵ��
		   b. ����GUI_Init�����󣬵��ú���GUI_SelectLayer(1), ���õ�ǰ��������ͼ��2��
*/
#undef  GUI_NUM_LAYERS
#define GUI_NUM_LAYERS    1

/* 
   5. ����ͼ��1��ͼ��2��Ӧ���Դ��ַ
      (1) EXT_SDRAM_ADDR ��SDRAM���׵�ַ��
      (2) LCD_LAYER0_FRAME_BUFFER ��ͼ��1���Դ��ַ��
	  (3) LCD_LAYER1_FRAME_BUFFER ��ͼ��2���Դ��ַ��
	  (4) ÿ��ͼ����Դ��С�ȽϿ�������������¼򵥵�˵����
	      ����û�ѡ�����ɫģʽ = 32λɫARGB8888���Դ�Ĵ�С��
	      XSIZE_PHYS * YSIZE_PHYS * 4 * NUM_VSCREENS * NUM_BUFFERS
		  
	      ��ɫģʽ = 24λɫRGB888���Դ�Ĵ�С��
	      XSIZE_PHYS * YSIZE_PHYS * 3 * NUM_VSCREENS * NUM_BUFFERS
		  
	      ��ɫģʽ = 16λɫRGB566��ARGB1555, ARGB4444��AL88����ô�Դ�Ĵ�С���ǣ�
	      XSIZE_PHYS * YSIZE_PHYS * 2 * NUM_VSCREENS * NUM_BUFFERS

	      ��ɫģʽ = 8λɫL8��AL44����ô�Դ�Ĵ�С���ǣ�
	      XSIZE_PHYS * YSIZE_PHYS * 1 * NUM_VSCREENS * NUM_BUFFERS	
      
      ����Ϊ�˷�������������������׵�16MB��SDRAMǰ8MB�����LCD�Դ�ʹ�ã���8MB����emWin��̬�ڴ档
	  ����24λɫ��16λɫ��8λɫ���û����Զ���ʹ�������壬����ʹ��˫ͼ�㡣����32λɫҲʹ���������˫
	  ͼ��Ļ��ᳬ��8MB�������û������Լ���������Դ��emWin��̬�ڴ�ķ��������
	    ��һ�����ӣ�����800*480�ֱ��ʵ���ʾ����ʹ��32λɫ�������壬��ô����һ��ͼ����Ҫ�Ĵ�С����
      800 * 480 * 4 * 3  = 4.394MB�Ŀռ䣬�����˫ͼ�㣬�Ѿ�����8MB�ķ��䷶Χ��

      (5)Ϊ�˷��������ͼ��2�ĺ궨��LCD_LAYER1_FRAME_BUFFER�еĲ���4�ǰ���32λɫ���õģ�����û���ͼ��1
         ʹ�õ���8λɫ������������1,�����16λɫ��������2�������24λɫ��������3��
*/
#define LCD_LAYER0_FRAME_BUFFER  SDRAM_LCD_BUF1
#define LCD_LAYER1_FRAME_BUFFER  (LCD_LAYER0_FRAME_BUFFER + XSIZE_PHYS * YSIZE_PHYS * 4 * NUM_VSCREENS * NUM_BUFFERS)



/* 
   6. STM32F429/439֧�ֵ���ɫģʽ������ģʽ��֧�֣��û����������á�
      �ر�ע�������������⣺
	  (1) ����û�ѡ����ARGB8888����RGB888ģʽ��LCD��˸�Ƚ������Ļ���
	      �뽵��LTDC��ʱ�Ӵ�С�����ļ�bsp_tft_429.c�ĺ���LCD_ConfigLTDC�������á�
	      a. һ��800*480�ֱ��ʵ���ʾ����ARGB8888����RGB888ģʽLTDCʱ��ѡ��10-20MHz���ɡ�
	      b. 480*272�ֱ��ʵĿ��Ը�Щ��ȡ20MHz���Ҽ��ɡ�
	  (2) 16λɫ����8λɫģʽ��LTDC��ʱ��Ƶ��һ����Ա�24λɫ����32λɫ�ĸ�һ����
*/
#define _CM_ARGB8888      1
#define _CM_RGB888        2
#define _CM_RGB565        3
#define _CM_ARGB1555      4
#define _CM_ARGB4444      5
#define _CM_L8            6
#define _CM_AL44          7
#define _CM_AL88          8

/* 7. ����ͼ��1����ɫģʽ�ͷֱ��ʴ�С */
#define COLOR_MODE_0      _CM_ARGB8888
#define XSIZE_0           XSIZE_PHYS
#define YSIZE_0           YSIZE_PHYS

/* 8. ����ͼ��2�ĵ���ɫģʽ�ͷֱ��ʴ�С */
#define COLOR_MODE_1      _CM_RGB565
#define XSIZE_1           XSIZE_PHYS
#define YSIZE_1           YSIZE_PHYS

/* 9. ��ͼ������£������û�ѡ�����ɫģʽ���Զ�ѡ��ͼ��1��emWin����������ɫģʽ */
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

/* 10. ˫ͼ������£������û�ѡ�����ɫģʽ���Զ�ѡ��ͼ��2��emWin����������ɫģʽ */
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

/*11. ����ѡ���⣬��ֹ���ô������ĳЩѡ��û������ */
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
									�ļ���ʹ�õ�ȫ�ֱ���
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
*	�� �� ��: LCD_LL_GetPixelformat
*	����˵��: ��ȡͼ��1����ͼ��2ʹ�õ���ɫ��ʽ
*	��    ��: LayerIndex  ͼ��
*	�� �� ֵ: ��ɫ��ʽ
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
*	�� �� ��: DMA2D_CopyBuffer
*	����˵��: ͨ��DMA2D��ǰ���㸴��ָ���������ɫ���ݵ�Ŀ������
*	��    ��: LayerIndex    ͼ��
*             pSrc          ��ɫ����Դ��ַ
*             pDst          ��ɫ����Ŀ�ĵ�ַ
*             xSize         Ҫ���������X���С����ÿ��������
*             ySize         Ҫ���������Y���С��������
*             OffLineSrc    ǰ����ͼ�����ƫ��
*             OffLineDst    �������ƫ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DMA2D_CopyBuffer(unsigned int LayerIndex,void * pSrc,void * pDst,uint32_t xSize,uint32_t ySize,uint32_t OffLineSrc,uint32_t OffLineDst)
{
    uint32_t PixelFormat;
    U32 timeout=0;
    PixelFormat=LCD_LL_GetPixelformat(LayerIndex);  //��ȡ���ظ�ʽ
    DMA2D->CR=0x00000000UL|(1<<9);                  //�洢�����洢��
    DMA2D->FGMAR=(uint32_t)pSrc;                    //����ǰ����洢����ַ                   
    DMA2D->OMAR=(uint32_t)pDst;                     //��������洢����ַ    
    DMA2D->FGOR=OffLineSrc;                         //����ǰ����ƫ��
    DMA2D->OOR=OffLineDst;                          //�������ƫ��
    DMA2D->FGPFCCR=PixelFormat;                     //������ɫģʽ
    DMA2D->NLR=(uint32_t)(xSize<<16)|(U16)ySize;    //����ÿ��������������
    DMA2D->CR|=DMA2D_CR_START;                      //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)                 //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	                //��ʱ�˳�
    }
}

/*
*********************************************************************************************************
*	�� �� ��: DMA2D_FillBuffer
*	����˵��: ͨ��DMA2D����ָ�����������ɫ���
*	��    ��: LayerIndex    ͼ��
*             pDst          ��ɫ����Ŀ�ĵ�ַ
*             xSize         Ҫ���������X���С����ÿ��������
*             ySize         Ҫ���������Y���С��������
*             OffLine       ǰ����ͼ�����ƫ��
*             ColorIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DMA2D_FillBuffer(unsigned int LayerIndex,void * pDst,uint32_t xSize,uint32_t ySize,uint32_t OffLine,uint32_t ColorIndex) 
{
    uint32_t PixelFormat;
    U32 timeout=0;
    PixelFormat=LCD_LL_GetPixelformat(LayerIndex);  //��ȡ���ظ�ʽ
    DMA2D->CR=0x00030000UL|(1<<9);                  //�Ĵ������洢�� 
    DMA2D->OCOLR=ColorIndex;                        //����Ҫ������ɫ                   
    DMA2D->OMAR=(uint32_t)pDst;                     //��������Ĵ�����ַ
    DMA2D->OOR=OffLine;                             //�������ƫ��
    DMA2D->OPFCCR=PixelFormat;                      //������ɫ��ʽ
    DMA2D->NLR=(uint32_t)(xSize << 16)|(U16)ySize;  //����ÿ��������������
    DMA2D->CR|=DMA2D_CR_START;                      //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)                 //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	                //��ʱ�˳�
    }
}
/*
*********************************************************************************************************
*	�� �� ��: _GetBufferSize
*	����˵��: ��ȡָ�����Դ��С
*	��    ��: LayerIndex    ͼ��
*	�� �� ֵ: �Դ��С
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
*	�� �� ��: CUSTOM_CopyBuffer
*	����˵��: �˺������ڶ໺�壬��һ�������е��������ݸ��Ƶ���һ�����塣
*	��    ��: LayerIndex    ͼ��
*             IndexSrc      Դ�������
*             IndexDst      Ŀ�껺�����
*	�� �� ֵ: ��
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
*	�� �� ��: CUSTOM_CopyRect
*	����˵��: �˺������ڶ໺�壬��һ��������ָ���������ݸ��Ƶ���һ�����塣
*	��    ��: LayerIndex    ͼ��
*             x0            Դ����x��λ��
*             y0            Դ����y��λ��
*             x1            Ŀ���x��λ��
*             y1            Ŀ���y��λ��
*             xSize         Ҫ���Ƶ�x���С
*             ySize         Ҫ���Ƶ�y���С
*	�� �� ֵ: ��
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
*	�� �� ��: CUSTOM_FillRect
*	����˵��: ��ָ�������������ɫ���
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
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
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//DMA2D��CLUT����
//pColor:CLUT�洢����ַ
//NumItems:CLUT��С
static void DMA2D_LoadLUT(LCD_COLOR * pColor,uint32_t NumItems) 
{
    DMA2D->FGCMAR=(uint32_t)pColor; //����ǰ����CLUT�洢����ַ
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_RGB888|((NumItems-1)&0xFF)<<8;//������ɫģʽ��CLUT��С
    DMA2D->FGPFCCR|=(1<<5);         //��������
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//����ָ����ַ��ɫ��RGB˳���ҷ�תalphaֵ�����µõ�����ɫֵ���Ƶ�Ŀ���ַ��
//pColorSrc:Դ��ַ
//pColorDst:Ŀ�ĵ�ַ
//NumItems:Ҫ���������
static void InvertAlpha_SwapRB(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
    uint32_t Color;
    do 
    {
        Color=*pColorSrc++;
        *pColorDst++=((Color&0x000000FF)<<16)           //������ɫ(R)����ɫ(B)��λ��
                    |(Color&0x0000FF00)                 //��ɫ(G)��λ�ò���
                    |((Color&0x00FF0000)>>16)           //������ɫ(R)����ɫ(B)��λ��
                    |((Color&0xFF000000)^0xFF000000);   //��תalphaֵ
    } while (--NumItems);
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//��תalpha����ΪDMA2D����ɫ��ʽ��STemWin����ɫ��ʽ��ͬ�������Ҫ��תalphaֵʹ������STemWin
//pColorSrc:Դ��ַ
//pColorDst:Ŀ�ĵ�ַ
//NumItems:Ҫ���������
static void InvertAlpha(LCD_COLOR * pColorSrc, LCD_COLOR * pColorDst, uint32_t NumItems) 
{
    uint32_t Color;
    do 
    {
        Color = *pColorSrc++;
        *pColorDst++=Color^0xFF000000;//��תalpha
    } while (--NumItems);
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//alpha��ɫ��ʹ��DMA2D
//pColorFG:ǰ����洢����ַ 
//pColorBG:������洢����ַ
//pColorDst:����洢����ַ
//NumItems:����
static void DMA2D_AlphaBlendingBulk(LCD_COLOR *pColorFG,LCD_COLOR *pColorBG,LCD_COLOR *pColorDst,U32 NumItems) 
{
    U32 timeout=0;
    DMA2D->CR=0x00020000UL|(1<<9);              //�洢�����洢����ִ�л��
    DMA2D->FGMAR=(uint32_t)pColorFG;            //����ǰ����洢����ַ
    DMA2D->BGMAR=(uint32_t)pColorBG;            //���ñ�����洢����ַ
    DMA2D->OMAR=(uint32_t)pColorDst;            //��������洢����ַ
    DMA2D->FGOR=0;                              //����ǰ����ƫ��
    DMA2D->BGOR=0;                              //���ñ�����ƫ��
    DMA2D->OOR=0;                               //�������ƫ��
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;  //����ǰ������ɫ��ʽ
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;  //���ñ�������ɫ��ʽ
    DMA2D->OPFCCR =LTDC_PIXEL_FORMAT_ARGB8888;  //���������ɫ��ʽ
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //�������� 
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }

}

/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
//���������ɫ,���ǰ��ɫ�ͱ���ɫ
//Color:ǰ��ɫ
//BkColor:����ɫ
//Intens:���ǿ��
//����ֵ:��Ϻ����ɫ
static LCD_COLOR DMA2D_MixColors(LCD_COLOR Color,LCD_COLOR BkColor,U8 Intens) 
{
    uint32_t ColorFG,ColorBG,ColorDst;
    U32 timeout=0;
    if((BkColor&0xFF000000)==0xFF000000)return Color;//����ɫ͸��������Ҫ��ɫ
    ColorFG=Color^0xFF000000;
    ColorBG=BkColor^0xFF000000;
  
    DMA2D->CR=0x00020000UL|(1<<9);              //�洢�����洢����ִ�л��  
    DMA2D->FGMAR=(uint32_t)&ColorFG;            //����ǰ����洢����ַ
    DMA2D->BGMAR=(uint32_t)&ColorBG;            //���ñ�����洢����ַ
    DMA2D->OMAR=(uint32_t)&ColorDst;            //��������洢����ַ
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888|(1UL<<16)|((uint32_t)Intens<<24);         //����ǰ������ɫ��ʽ
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888|(0UL<<16)|((uint32_t)(255-Intens)<<24);   //���ñ�������ɫ��ʽ
    DMA2D->OPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;   //���ñ�������ɫ��ʽ
    DMA2D->NLR=(uint32_t)(1<<16)|1;             //����ÿ�������������� 
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
    return (ColorDst^0xFF000000);               //���ػ�Ϻ����ɫ
}

//ʹ��DMA2D������ɫת��
//pSrc:Դ��ɫ(ǰ��ɫ)
//pDst:ת�������ɫ(�����ɫ)
//PixelFormatSrc:ǰ������ɫ��ʽ
//PixelFormatDst:�����ɫ��ʽ
//NumItems:ÿ��������
/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DMA2D_ConvertColor(void * pSrc,void * pDst,uint32_t PixelFormatSrc,uint32_t PixelFormatDst,uint32_t NumItems)
{
    U32 timeout=0;
    DMA2D->CR=0x00010000UL|(1<<9);              //�洢�����洢����ִ��FPC
    DMA2D->FGMAR=(uint32_t)pSrc;                //ǰ����洢����ַ
    DMA2D->OMAR=(uint32_t)pDst;                 //����洢����ַ
    DMA2D->FGOR=0;                              //������ƫ��
    DMA2D->OOR=0;                               //���ƫ��
    DMA2D->FGPFCCR=PixelFormatSrc;              //ǰ������ɫ��ʽ
    DMA2D->OPFCCR=PixelFormatDst;               //�����ɫ��ʽ
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //����ÿ��������������
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
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

//ʹ��DMA2D������ɫ���
//pColorFG:ǰ��ɫ�洢����ַ
//pColorBG:����ɫ�洢����ַ
//pColorDst:����洢����ַ
//Intens:���ǿ��
//NumItems:ÿ��������
/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DMA2D_MixColorsBulk(LCD_COLOR * pColorFG, LCD_COLOR * pColorBG, LCD_COLOR * pColorDst, U8 Intens, uint32_t NumItems)
{
    U32 timeout=0;
    DMA2D->CR=0x00020000UL|(1<<9);              //�洢�����洢����ִ�л��
    DMA2D->FGMAR=(uint32_t)pColorFG;            //����ǰ��ɫ�洢����ַ
    DMA2D->BGMAR=(uint32_t)pColorBG;            //���ñ���ɫ�洢����ַ
    DMA2D->OMAR=(uint32_t)pColorDst;            //��������洢����ַ
    DMA2D->FGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888   //����ǰ��ɫ��ɫ��ʽ
                    |(1UL<<16)
                    |((uint32_t)Intens<<24);
    DMA2D->BGPFCCR=LTDC_PIXEL_FORMAT_ARGB8888   //���ñ���ɫ��ɫ��ʽ
                    |(0UL<<16)
                    |((uint32_t)(255-Intens)<<24);    
    DMA2D->OPFCCR=LTDC_PIXEL_FORMAT_ARGB8888;   //���������ɫ��ʽ
    DMA2D->NLR=(uint32_t)(NumItems<<16)|1;      //����ÿ��������������    
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
}



//ʹ��DMA2D����alpha��ɫ
//pColorFG:ǰ��ɫ
//pColorBG:����ɫ
//pColorDst:�����ɫ
//NumItems:ÿ��������
/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
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
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫ����ת��Ϊ��ɫֵ
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DMA2D_Index2ColorBulk(void *pIndex,LCD_COLOR *pColor,uint32_t NumItems, U8 SizeOfIndex,uint32_t PixelFormat)
{
    DMA2D_ConvertColor(pIndex,aBufferDMA2D,PixelFormat,LTDC_PIXEL_FORMAT_ARGB8888,NumItems);
    InvertAlpha_SwapRB(aBufferDMA2D,pColor,NumItems);
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ɫֵת��Ϊ��ɫ����
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DMA2D_Color2IndexBulk(LCD_COLOR *pColor,void *pIndex,uint32_t NumItems,U8 SizeOfIndex,uint32_t PixelFormat) 
{
    InvertAlpha_SwapRB(pColor,aBufferDMA2D,NumItems);
    DMA2D_ConvertColor(aBufferDMA2D,pIndex,LTDC_PIXEL_FORMAT_ARGB8888,PixelFormat,NumItems);
}

/*
*********************************************************************************************************
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ָ�������������ɫ���
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
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
*	�� �� ��: _DMA_DrawBitmapL8
*	����˵��: ��ָ�������������ɫ���
*	��    ��: LayerIndex    ͼ��
*             x0            ��ʼx��λ��
*             y0            ��ʼy��λ��
*             x1            ����x��λ��
*             y1            ����y��λ��
*             PixelIndex    Ҫ������ɫֵ
*	�� �� ֵ: ��
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
    DMA2D->CR|=DMA2D_CR_START;                  //����DMA2D����
    while(DMA2D->CR&DMA2D_CR_START)             //�ȴ�������� 
    {
        timeout++;
		if(timeout>0X1FFFFF)break;	            //��ʱ�˳�
    }
}

/*
*********************************************************************************************************
*	�� �� ��: _LCD_DrawBitmap8bpp
*	����˵��: 8bppλͼ����
*	��    ��: --
*	�� �� ֵ: ��
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
*	�� �� ��: _LCD_DrawBitmap16bpp
*	����˵��: 16bppλͼ����
*	��    ��: --
*	�� �� ֵ: ��
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
*	�� �� ��: _LCD_DrawBitmap32bpp
*	����˵��: 32bppλͼ����
*	��    ��: --
*	�� �� ֵ: ��
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
*	�� �� ��: _LCD_DisplayOn
*	����˵��: ��LCD
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _LCD_DisplayOn(void) 
{
    BSP_LcdOpenBl();
    __HAL_LTDC_ENABLE(&LtdcHandler);
}

/*
*********************************************************************************************************
*	�� �� ��: _LCD_DisplayOff
*	����˵��: �ر�LCD
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void _LCD_DisplayOff(void) 
{
     BSP_LcdCloseBl();
    __HAL_LTDC_DISABLE(&LtdcHandler);
}

/*
*********************************************************************************************************
*	�� �� ��: DMA2D_IRQHandler
*	����˵��: DMA2D��������жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DMA2D_IRQHandler(void) 
{
	DMA2D->IFCR = (U32)DMA2D_IFSR_CTCIF;
}

/*
*********************************************************************************************************
*	�� �� ��: DMA2D_IRQHandler
*	����˵��: DMA2D��������жϷ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void LCD_LL_Init(void) 
{
//    //LTDC�жϣ���ռ���ȼ�1�������ȼ�1
//    HAL_NVIC_SetPriority(LTDC_IRQn,1,1);    
//    HAL_NVIC_EnableIRQ(LTDC_IRQn);
//    HAL_LTDC_ProgramLineEvent(&LtdcHandler,0);//����LTDC�����ж�
//  
//    //DMA2DĬ������
//    Dma2dHandler.Instance=DMA2D; 
//    Dma2dHandler.Init.Mode=DMA2D_R2M;          //�ڴ浽�洢��ģʽ
//    Dma2dHandler.Init.ColorMode=DMA2D_RGB565;  //RGB565ģʽ
//    Dma2dHandler.Init.OutputOffset=0x0;        //���ƫ��Ϊ0    
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
*	�� �� ��: LCD_X_Config
*	����˵��: LCD����
*	��    ��: ��
*	�� �� ֵ: ��
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


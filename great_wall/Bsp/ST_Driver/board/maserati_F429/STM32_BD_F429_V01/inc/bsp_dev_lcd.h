/**
  ******************************************************************************
  * @file    stm32f429i_discovery_lcd.h
  * @author  MCD Application Team
  * @version V2.1.4
  * @date    06-May-2016
  * @brief   This file contains all the functions prototypes for the 
  *          stm32f429i_discovery_lcd.c driver.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BSP_LCD_H
#define __BSP_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
/* Include SDRAM Driver */
#include "bsp_sdram.h"
#include "../../../Utilities/Fonts/fonts.h"
/* Include LCD component driver */
#include "../../../Components/at070tn92/at070tn92.h" 

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM32F429I_DISCOVERY
  * @{
  */ 
    
/** @addtogroup STM32F429I_DISCOVERY_LCD
  * @{
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Exported_Types STM32F429I DISCOVERY LCD Exported Types
  * @{
  */
typedef enum 
{
  LCD_OK = 0,
  LCD_ERROR = 1,
  LCD_TIMEOUT = 2
}LCD_StatusTypeDef;

typedef struct 
{ 
  uint32_t  TextColor; 
  uint32_t  BackColor;  
  sFONT     *pFont;
}LCD_DrawPropTypeDef;
   
typedef struct 
{
  int16_t X;
  int16_t Y;
} Point, * pPoint;	 
	 
/** 
  * @brief  Line mode structures definition  
  */ 
typedef enum
{
  CENTER_MODE             = 0x01,    /* center mode */
  RIGHT_MODE              = 0x02,    /* right mode  */     
  LEFT_MODE               = 0x03,    /* left mode   */                                                                               
}Text_AlignModeTypdef;
/**
  * @}
  */ 

/* ����LCD����ʹ��ENWIN */
#define LCD_DRIVER_TO_GUI        1

/** @defgroup STM32F429I_DISCOVERY_LCD_Exported_Constants STM32F429I DISCOVERY LCD Exported Constants
  * @{
  */ 
#define LCD_LayerCfgTypeDef    LTDC_LayerCfgTypeDef

/** 
  * @brief  LCD status structure definition  
  */     
#define MAX_LAYER_NUMBER       2
#define LCD_FRAME_BUFFER       ((uint32_t)0xD0000000)
#define BUFFER_OFFSET          ((uint32_t)0x50000) 
#define LCD_MemoryAdd_OFFSET    xx    


/** 
  * @brief  LCD color  
  */ 
#define LCD_COLOR_BLUE          0xFF0000FF
#define LCD_COLOR_GREEN         0xFF00FF00
#define LCD_COLOR_RED           0xFFFF0000
#define LCD_COLOR_CYAN          0xFF00FFFF
#define LCD_COLOR_MAGENTA       0xFFFF00FF
#define LCD_COLOR_YELLOW        0xFFFFFF00
#define LCD_COLOR_LIGHTBLUE     0xFF8080FF
#define LCD_COLOR_LIGHTGREEN    0xFF80FF80
#define LCD_COLOR_LIGHTRED      0xFFFF8080
#define LCD_COLOR_LIGHTCYAN     0xFF80FFFF
#define LCD_COLOR_LIGHTMAGENTA  0xFFFF80FF
#define LCD_COLOR_LIGHTYELLOW   0xFFFFFF80
#define LCD_COLOR_DARKBLUE      0xFF000080
#define LCD_COLOR_DARKGREEN     0xFF008000
#define LCD_COLOR_DARKRED       0xFF800000
#define LCD_COLOR_DARKCYAN      0xFF008080
#define LCD_COLOR_DARKMAGENTA   0xFF800080
#define LCD_COLOR_DARKYELLOW    0xFF808000
#define LCD_COLOR_WHITE         0xFFFFFFFF
#define LCD_COLOR_LIGHTGRAY     0xFFD3D3D3
#define LCD_COLOR_GRAY          0xFF808080
#define LCD_COLOR_DARKGRAY      0xFF404040
#define LCD_COLOR_BLACK         0xFF000000
#define LCD_COLOR_BROWN         0xFFA52A2A
#define LCD_COLOR_ORANGE        0xFFFFA500
#define LCD_COLOR_TRANSPARENT   0xFF000000

/** 
  * @brief LCD default font ����Ĭ������
  */ 
#define LCD_DEFAULT_FONT         Font24

/** 
  * @brief LCD default font ����Ĭ�ϱ���ɫ
  */ 
#define LCD_DEFAULT_BACK_COLOR   LCD_COLOR_WHITE

/** 
  * @brief LCD default font ����Ĭ������ɫ
  */ 
#define LCD_DEFAULT_TEXT_COLOR   LCD_COLOR_BLACK

/** 
  * @brief  LCD Reload Types
  */
#define LCD_RELOAD_IMMEDIATE               ((uint32_t)LTDC_SRCR_IMR)
#define LCD_RELOAD_VERTICAL_BLANKING       ((uint32_t)LTDC_SRCR_VBR)

/** 
  * @brief  LCD Layer  
  */ 
#define LCD_BACKGROUND_LAYER     0x0000
#define LCD_FOREGROUND_LAYER     0x0001

/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Exported_Macros STM32F429I DISCOVERY LCD Exported Macros
  * @{
  */ 
/** 
  * @brief LCD Pixel format 
  */  
#define LCD_PIXEL_FORMAT_ARGB8888         LTDC_PIXEL_FORMAT_ARGB8888
#define LCD_PIXEL_FORMAT_RGB888           LTDC_PIXEL_FORMAT_RGB888        
#define LCD_PIXEL_FORMAT_RGB565           LTDC_PIXEL_FORMAT_RGB565                
#define LCD_PIXEL_FORMAT_ARGB1555         LTDC_PIXEL_FORMAT_ARGB1555        
#define LCD_PIXEL_FORMAT_ARGB4444         LTDC_PIXEL_FORMAT_ARGB4444        
#define LCD_PIXEL_FORMAT_L8               LTDC_PIXEL_FORMAT_L8        
#define LCD_PIXEL_FORMAT_AL44             LTDC_PIXEL_FORMAT_AL44        
#define LCD_PIXEL_FORMAT_AL88             LTDC_PIXEL_FORMAT_AL88
/**
  * @}
  */ 

/** @defgroup STM32F429I_DISCOVERY_LCD_Exported_Functions STM32F429I DISCOVERY LCD Exported Functions
  * @{
  */ 
uint8_t  BSP_LCD_Init(void);
uint32_t BSP_LCD_GetXSize(void);
uint32_t BSP_LCD_GetYSize(void);

/* functions using the LTDC controller */
/**
  * @brief  Initializes the LCD layers.Ĭ��ͼ�㣬�����ϣ����壬������ɫ����
  * @param  LayerIndex: the layer foreground or background. 
  * @param  FB_Address: the layer frame buffer.
  */
void     BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FrameBuffer);

/**
  * @brief  Configures the Transparency.����ͼ��͸����
  * @param  LayerIndex: the Layer foreground or background.
  * @param  Transparency: the Transparency, 
  *    This parameter must range from 0x00 to 0xFF.
  */
void     BSP_LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency);

/**
  * @brief  Configures the transparency without reloading.
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF 
  * @retval None
  */
void     BSP_LCD_SetTransparency_NoReload(uint32_t LayerIndex, uint8_t Transparency);
/**
  * @brief  Sets a LCD layer frame buffer address.���ò��ַ
  * @param  LayerIndex: specifies the Layer foreground or background
  * @param  Address: new LCD frame buffer value      
  */
void     BSP_LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address);
/**
  * @brief  Sets an LCD layer frame buffer address without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value      
  * @retval None
  */
void     BSP_LCD_SetLayerAddress_NoReload(uint32_t LayerIndex, uint32_t Address);
/**
  * @brief  Configures and sets the color Keying.
  * @param  LayerIndex: the Layer foreground or background
  * @param  RGBValue: the Color reference
  */
void     BSP_LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue);
/**
  * @brief  Configures and sets the color keying without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
void     BSP_LCD_SetColorKeying_NoReload(uint32_t LayerIndex, uint32_t RGBValue);
/**
  * @brief  Disables the color Keying.
  * @param  LayerIndex: the Layer foreground or background
  */
void     BSP_LCD_ResetColorKeying(uint32_t LayerIndex);
/**
  * @brief  Disables the color keying without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void     BSP_LCD_ResetColorKeying_NoReload(uint32_t LayerIndex);
/**
  * @brief  Sets the Display window.
  * @param  LayerIndex: layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height  
  */
void     BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
/**
  * @brief  Sets display window without reloading.
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height  
  * @retval None
  */
void     BSP_LCD_SetLayerWindow_NoReload(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
/**
  * @brief  Selects the LCD Layer.
  * @param  LayerIndex: the Layer foreground or background.
  */
void     BSP_LCD_SelectLayer(uint32_t LayerIndex);
/**
  * @brief  Sets a LCD Layer visible.
  * @param  LayerIndex: the visible Layer.
  * @param  state: new state of the specified layer.
  *    This parameter can be: ENABLE or DISABLE.  
  */
void     BSP_LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState state);
/**
  * @brief  Sets an LCD Layer visible without reloading.
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE 
  * @retval None
  */
void     BSP_LCD_SetLayerVisible_NoReload(uint32_t LayerIndex, FunctionalState State);
/**
  * @brief  Disables the color keying without reloading.
  * @param  ReloadType: can be one of the following values
  *         - LCD_RELOAD_IMMEDIATE
  *         - LCD_RELOAD_VERTICAL_BLANKING
  * @retval None
  */
void     BSP_LCD_Relaod(uint32_t ReloadType);

  /**
  * @brief  Sets the Text color.����lcd������ɫ 
  * @param  Color: the Text color code ARGB(8-8-8-8)
  */
void     BSP_LCD_SetTextColor(uint32_t Color);
/** 
  * @brief  ���ñ�����ɫ  
  */
void     BSP_LCD_SetBackColor(uint32_t Color);
/** 
  * @brief  ��ȡ������ɫ
  */
uint32_t BSP_LCD_GetTextColor(void);
/** 
  * @brief  ��ȡ������ɫ  
  */
uint32_t BSP_LCD_GetBackColor(void);
/** 
  * @brief  ��������  
  */
void     BSP_LCD_SetFont(sFONT *pFonts);
/** 
  * @brief  ��ȡ���� 
  */
sFONT    *BSP_LCD_GetFont(void);

/**
  * @brief  Reads Pixel.��ȡĳλ��һ��������ɫֵ 
  * @param  Xpos: the X position
  * @param  Ypos: the Y position 
  * @retval RGB pixel color
  */
uint32_t BSP_LCD_ReadPixel(uint16_t Xpos, uint16_t Ypos);
/**
  * @brief  Writes Pixel.����һ�����ص�
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  RGB_Code: the pixel color in ARGB mode (8-8-8-8)  
  */
void     BSP_LCD_DrawPixel(uint16_t Xpos, uint16_t Ypos, uint32_t pixel);
/**
  * @brief  Clears the hole LCD.����
  * @param  Color: the color of the background
  */
void     BSP_LCD_Clear(uint32_t Color);
/**
  * @brief  Clears the selected line.���һ����ɫ
  * @param  Line: the line to be cleared
  */
void     BSP_LCD_ClearStringLine(uint32_t Line);
/**
  * @brief  Displays a maximum of 20 char on the LCD.
  * @param  Line: the Line where to display the character shape
  * @param  ptr: pointer to string to display on LCD
  */
void     BSP_LCD_DisplayStringAtLine(uint16_t Line, uint8_t *ptr);
/**
  * @brief  Displays a maximum of 60 char on the LCD.��ʾ�ַ���
  * @param  X: pointer to x position (in pixel)
  * @param  Y: pointer to y position (in pixel)    
  * @param  pText: pointer to string to display on LCD
  * @param  mode: The display mode
  *    This parameter can be one of the following values:�ֲ���ʽ
  *                @arg CENTER_MODE 
  *                @arg RIGHT_MODE
  *                @arg LEFT_MODE   
  */
void     BSP_LCD_DisplayStringAt(uint16_t X, uint16_t Y, uint8_t *pText, Text_AlignModeTypdef mode);
/**
  * @brief  Displays one character.��ʾһ���ַ�
  * @param  Xpos: start column address
  * @param  Ypos: the Line where to display the character shape
  * @param  Ascii: character ascii code, must be between 0x20 and 0x7E
  */
void     BSP_LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t Ascii);

/**
  * @brief  Displays an horizontal line.����һ����ֱ��
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Length: line length
  */
void     BSP_LCD_DrawHLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
/**
  * @brief  Displays a vertical line.����һ����ֱ��
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Length: line length
  */
void     BSP_LCD_DrawVLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length);
/**
  * @brief  Displays an uni-line (between two points).����һ���㵽��ֱ��
  * @param  X1: the point 1 X position
  * @param  Y1: the point 1 Y position
  * @param  X2: the point 2 X position
  * @param  Y2: the point 2 Y position
  */
void     BSP_LCD_DrawLine(uint16_t X1, uint16_t Y1, uint16_t X2, uint16_t Y2);
/**
  * @brief  Displays a rectangle.���ƾ���
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Height: display rectangle height
  * @param  Width: display rectangle width
  */
void     BSP_LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
/**
  * @brief  Displays a circle.����Բ��
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Radius: the circle radius
  */
void     BSP_LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
/**
  * @brief  Displays an poly-line (between many points).���ƶ����
  * @param  Points: pointer to the points array
  * @param  PointCount: Number of points
  */
void     BSP_LCD_DrawPolygon(pPoint Points, uint16_t PointCount);
/**
  * @brief  Displays an Ellipse.������Բ��
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  XRadius: the X radius of ellipse
  * @param  YRadius: the Y radius of ellipse
  */
void     BSP_LCD_DrawEllipse(int Xpos, int Ypos, int XRadius, int YRadius);
/**
  * @brief  Displays a bitmap picture loaded in the internal Flash (32 bpp).��ʾ32bppλͼ
  * @param  X: the bmp x position in the LCD
  * @param  Y: the bmp Y position in the LCD
  * @param  pBmp: Bmp picture address in the internal Flash
  */
void     BSP_LCD_DrawBitmap(uint32_t X, uint32_t Y, uint8_t *pBmp);

/**
  * @brief  Displays a full rectangle.����������
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Height: rectangle height
  * @param  Width: rectangle width
  */
void     BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
/**
  * @brief  Displays a full circle.�������Բ��
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  Radius: the circle radius
  */
void     BSP_LCD_FillCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius);
/**
  * @brief  Fill triangle.�������������
  * @param  X1: the point 1 x position
  * @param  Y1: the point 1 y position
  * @param  X2: the point 2 x position
  * @param  Y2: the point 2 y position
  * @param  X3: the point 3 x position
  * @param  Y3: the point 3 y position
  */
void     BSP_LCD_FillTriangle(uint16_t X1, uint16_t X2, uint16_t X3, uint16_t Y1, uint16_t Y2, uint16_t Y3);
/**
  * @brief  Displays a full poly-line (between many points).�����������
  * @param  Points: pointer to the points array
  * @param  PointCount: Number of points
  */
void     BSP_LCD_FillPolygon(pPoint Points, uint16_t PointCount);
/**
  * @brief  Draw a full ellipse.���������Բ
  * @param  Xpos: the X position
  * @param  Ypos: the Y position
  * @param  XRadius: X radius of ellipse
  * @param  YRadius: Y radius of ellipse. 
  */
void     BSP_LCD_FillEllipse(int Xpos, int Ypos, int XRadius, int YRadius);

/**
  * @brief  Disables the Display.�ر���
  */
void     BSP_LCD_DisplayOff(void);
/**
  * @brief  Disables the Display.������
  */
void     BSP_LCD_DisplayOn(void);


#ifdef LCD_DRIVER_TO_GUI
uint8_t BSP_LCD_LL_Init(void);

#endif

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
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32F429I_DISCOVERY_LCD_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

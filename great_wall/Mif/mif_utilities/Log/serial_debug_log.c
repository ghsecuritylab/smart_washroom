/**
  ******************************************************************************
  * @file    hal_serial_log.c
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-08-27
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdarg.h>
#include "serial_debug_log.h"
#include "bsp_mcu_uart.h" 


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/

/**
  * @brief  Redirect printf
  * @param  None
  * @retval None
  */
static void simple_uart1_put_porting(uint8_t cr)
{
    BSP_Uart1SendChar(cr);			
}

/**
  * @brief  Redirect printf
  * @param  None
  * @retval None
  */
static void simple_uart1_putstring(uint8_t *str)
{
    uint8_t i = 0;
    uint8_t ch = str[i++];
    while (ch != '\0')
    {
        simple_uart1_put_porting(ch);
        ch = str[i++];
    }
}

/**
  * @brief  Redirect printf
  * @param  None
  * @retval None
  */
void __log(uint8_t level, const char * func, uint32_t line, const char *format, ...)
{
    char str[128];
    va_list ap;
    if( level >= __LEVEL__ ) 
    {

//        va_list ap;
//        
//        snprintf(str, sizeof(str), "[%d]%s(%d): ",level,func,line);
//        simple_uart1_putstring((uint8_t *)str);

        switch(level)
        {
            case LEVEL_VERBOSE:
                break;        
            case LEVEL_INFO:
                {
                    //snprintf(str, sizeof(str), "[%s]: ","INFO");
                    //simple_uart1_putstring((uint8_t *)str);
                }
                break;
            case LEVEL_DEBUG:
                {
                    
                    snprintf(str, sizeof(str), "[%s]%s(%d): ","DEBUG",func,line);
                    simple_uart1_putstring((uint8_t *)str);    
                }                
                break;
            case LEVEL_WARNING:
                {
                    
                    snprintf(str, sizeof(str), "[%s]%s(%d): ","WARNING",func,line);
                    simple_uart1_putstring((uint8_t *)str);    
                }             
                break;
            case LEVEL_ERROR:
                {

                    snprintf(str, sizeof(str), "[%s]%s(%d): ","ERROR",func,line);
                    simple_uart1_putstring((uint8_t *)str);    
                }             
                break;                           
        }
        
        va_start(ap, format);
        (void)vsnprintf(str, sizeof(str), format, ap);
        va_end(ap);
        simple_uart1_putstring((uint8_t *)str);
        simple_uart1_put_porting('\r');
    }
} 
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

/**
  ******************************************************************************
  * @file    hal_uart.c 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-09-02
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "hal_uart.h"


/** @addtogroup CORE
  * @{
  */

/** @defgroup KERNEL_BSP
  * @brief Kernel BSP routines
  * @{
  */


/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
uint8_t g_bUartInit = 0;
Uart_Port_S uartPort[UART_1];
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
void HAL_UartInit(void)
{

}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
COMMON_StatusTypeDef HAL_UartOpen(Uart_Type_S uart)
{    
    return BSP_UartInit(uart.port,uart.baud);
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
COMMON_StatusTypeDef HAL_UartClose(Uart_Type_S port,uint8_t* readBuf,uint16_t readLen)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;
    return iRet;
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
COMMON_StatusTypeDef HAL_UartRead(Uart_Type_S port,uint8_t* readBuf,uint16_t readLen)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;
    return iRet;
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
COMMON_StatusTypeDef HAL_UartWrite(Uart_Type_S port,uint8_t* writeBuf,uint16_t writeLen)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;
    return iRet;
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
COMMON_StatusTypeDef HAL_UartSetBaud(Uart_Type_S port,uint32_t baud)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;
    BSP_UartSetBaud(port,uint32_t baud);
    return iRet;
}

/**
  * @brief  Clear fifo
  * @param  None 
  * @retval None
  */
COMMON_StatusTypeDef HAL_UartTcflush(Uart_Type_S port)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;
    return iRet;
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
COMMON_StatusTypeDef HAL_UartWrite1Byte(Uart_Type_S port,char ch)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;
    return iRet;
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
void HAL_UartDemoTest(void)
{
    
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */

static COMMON_StatusTypeDef TBuf_Init(Uart_Port_S *uartPort,UART_PORT_E portNum)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;

    if(uartPort == NULL)
    {   
        iRet = COMMON_OK;
        return  iRet;
    }
    
    uartPort->PortQueue[portNum].T_Front = 0;
    uartPort->PortQueue[portNum].T_Rear = 0;
    uartPort->PortQueue[portNum].T_Count = 0;
    uartPort->PortQueue[portNum].T_Disabled = 1;
    
    return  iRet;
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
static COMMON_StatusTypeDef RBuf_Init(Uart_Port_S *uartPort,UART_PORT_E portNum)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;

    if(uartPort == NULL)
    {   
        iRet = COMMON_OK;
        return  iRet;
    }
    
    uartPort->PortQueue[portNum].R_Front = 0;
    uartPort->PortQueue[portNum].R_Rear = 0;
    uartPort->PortQueue[portNum].R_Count = 0;
    uartPort->PortQueue[portNum].R_Overflow = 0;
    
    return  iRet;
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
void Uart_Rx_Callback(UART_PORT_E portNum)
{
    switch(portNum)
    {
        case UART_1:
        
            break;
        case UART_2:
            break;
        case UART_3:
            break;
        case UART_4:
            break;
        case UART_5:
            break;
        case UART_6:
            break;
        case UART_7:
            break;
        default:
            break;
    }

}
void HAL_UartIsrRegister(void)
{
    
}

/**
  * @brief  Initializes LEDs.
  * @param  None 
  * @retval None
  */
uint32_t HAL_TimerCreate(TimerCb pTimerCb,
                         uint32_t u32FirstTimeMs,
                         uint32_t u32PeriodTimeMs,
                         bool_t   bStartTimer,
                         char     *pName)
{

}
/**
  * @}
  */

/**
  * @}
  */

/****************************************************************END OF FILE****/

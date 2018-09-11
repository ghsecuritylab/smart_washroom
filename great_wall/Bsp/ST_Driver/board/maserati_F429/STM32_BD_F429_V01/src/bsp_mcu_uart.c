/**
  ******************************************************************************
  * @file    bsp_mcu_uart.c
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-09-01
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "bsp_mcu_uart.h"


/* Private defines -----------------------------------------------------------*/
/**
 * @brief Definition for COM port1, connected to USART1
 */

#define MCU_COM1                           USART1
#define MCU_COM1_CLK_ENABLE()             __USART1_CLK_ENABLE()
#define MCU_COM1_CLK_DISABLE()            __USART1_CLK_DISABLE()

#define MCU_COM1_TX_PIN                   GPIO_PIN_9
#define MCU_COM1_TX_GPIO_PORT             GPIOA
#define MCU_COM1_TX_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()
#define MCU_COM1_TX_GPIO_CLK_DISABLE()    __GPIOA_CLK_DISABLE()
#define MCU_COM1_TX_AF                    GPIO_AF7_USART1

#define MCU_COM1_RX_PIN                   GPIO_PIN_10
#define MCU_COM1_RX_GPIO_PORT             GPIOA
#define MCU_COM1_RX_GPIO_CLK_ENABLE()     __GPIOA_CLK_ENABLE()
#define MCU_COM1_RX_GPIO_CLK_DISABLE()    __GPIOA_CLK_DISABLE()
#define MCU_COM1_RX_AF                    GPIO_AF7_USART1

#define MCU_COM1_IRQn                     USART1_IRQn

#define MCU_COMx_CLK_ENABLE(__INDEX__)              do{if((__INDEX__) == 0) EVAL_COM1_CLK_ENABLE(); \
                                                       }while(0)
#define MCU_COMx_CLK_DISABLE(__INDEX__)             do{if((__INDEX__) == 0) EVAL_COM1_CLK_DISABLE(); \
                                                       }while(0)

#define MCU_COMx_TX_GPIO_CLK_ENABLE(__INDEX__)      do{if((__INDEX__) == 0) EVAL_COM1_TX_GPIO_CLK_ENABLE(); \
                                                     }while(0)
#define MCU_COMx_TX_GPIO_CLK_DISABLE(__INDEX__)     do{if((__INDEX__) == 0) EVAL_COM1_TX_GPIO_CLK_DISABLE(); \
                                                     }while(0)

#define MCU_COMx_RX_GPIO_CLK_ENABLE(__INDEX__)      do{if((__INDEX__) == 0) EVAL_COM1_RX_GPIO_CLK_ENABLE(); \
                                                       }while(0)
#define MCU_COMx_RX_GPIO_CLK_DISABLE(__INDEX__)     do{if((__INDEX__) == 0) EVAL_COM1_RX_GPIO_CLK_DISABLE(); \
                                                       }while(0)
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* UART handler declaration */
UART_HandleTypeDef Uart1Handle;
UART_HandleTypeDef Uart2Handle;
UART_HandleTypeDef Uart3Handle;

/* Private functions ---------------------------------------------------------*/
static COMMON_StatusTypeDef BSP_Uart1Init(uint32_t u32Baud);
static COMMON_StatusTypeDef BSP_Uart2Init(uint32_t u32Baud);
static COMMON_StatusTypeDef BSP_Uart3Init(uint32_t u32Baud);

/**
  * @brief  Configures COM port.
  * @param  COM: COM port to be configured.
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
COMMON_StatusTypeDef BSP_UartInit(En_MCU_UART_PORT enUartPort,uint32_t u32Baud)
{
    COMMON_StatusTypeDef iRet = COMMON_OK;
    if(enUartPort > EN_MCU_UART_MAX)
    {
        return COMMON_PARA_ERROR;
    }

    switch(enUartPort)
    {
        case EN_MCU_UART_1:
            iRet = BSP_Uart1Init(u32Baud);
            break;
        case EN_MCU_UART_2:
            iRet = BSP_Uart2Init(u32Baud);
            break;
        case EN_MCU_UART_3:
            iRet = BSP_Uart3Init(u32Baud);
            break;
        default:
            iRet = COMMON_PARA_ERROR;
            break;
    }

    return iRet;
}

/**
  * @brief  Configures COM port.
  * @param  COM: COM port to be configured.
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
static COMMON_StatusTypeDef BSP_Uart1Init(uint32_t u32Baud)
{
#if (defined(MCU_UART1_DEFINED))
    GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIO clock */
    MCU_COM1_TX_GPIO_CLK_ENABLE();

    /* Enable USART clock */
    MCU_COM1_CLK_ENABLE() ;

    /* Configure USART Tx as alternate function */
    /* TXD ---- PA9 */
    GPIO_InitStruct.Pin = MCU_COM1_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = MCU_COM1_TX_AF;
    HAL_GPIO_Init(MCU_COM1_TX_GPIO_PORT, &GPIO_InitStruct);

    /*RXD ---- PA10 */
    GPIO_InitStruct.Pin = MCU_COM1_RX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Alternate = MCU_COM1_RX_AF;
    HAL_GPIO_Init(MCU_COM1_TX_GPIO_PORT, &GPIO_InitStruct);

    /* USART configuration */
    Uart1Handle.Instance = MCU_COM1;

    Uart1Handle.Init.BaudRate = u32Baud;
    Uart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
    Uart1Handle.Init.StopBits = UART_STOPBITS_1;
    Uart1Handle.Init.Parity = UART_PARITY_NONE;
    Uart1Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    Uart1Handle.Init.Mode = UART_MODE_TX_RX;

    if(HAL_UART_Init(&Uart1Handle) != HAL_OK)
    {
        return COMMON_ERROR;
    }
    return COMMON_OK;
#else
    #error no defined MCU_UART1_DEFINED
#endif
}

/**
  * @brief  Configures COM port.
  * @param  COM: COM port to be configured.
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
static COMMON_StatusTypeDef BSP_Uart2Init(uint32_t u32Baud)
{
#if (defined(MCU_UART2_DEFINED))
#else
    return COMMON_ERROR;
#endif
}

/**
  * @brief  Configures COM port.
  * @param  COM: COM port to be configured.
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
static COMMON_StatusTypeDef BSP_Uart3Init(uint32_t u32Baud)
{
#if (defined(MCU_UART3_DEFINED))
#else
    return COMMON_ERROR;
#endif
}

/**
  * @brief  Send one char.
  * @param  COM: COM port to be configured.
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */
uint8_t BSP_Uart1SendChar(uint8_t u8Ch)
{
    HAL_UART_Transmit(&Uart1Handle, (uint8_t *)&u8Ch, 1, 0xFFFF);
    return u8Ch;
}

/**
  * @brief  Send one char.
  * @param  COM: COM port to be configured.
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */


/**
  * @brief  Send one char.
  * @param  COM: COM port to be configured.
  * @param  huart: Pointer to a UART_HandleTypeDef structure that contains the
  *                configuration information for the specified USART peripheral.
  */

/**
  * @brief  This function handles UART interrupt request.
  * @param  None
  * @retval None
  * @Note   This function is redefined in "main.h" and related to DMA stream
  *         used for USART data transmission
  */
void USARTx_IRQHandler(void)
{
    #if (defined(MCU_UART1_DEFINED))
        HAL_UART_IRQHandler(& Uart1Handle);
    #elif (defined(MCU_UART2_DEFINED))
        HAL_UART_IRQHandler(& Uart2Handle);
    #elif (defined(MCU_UART3_DEFINED))
        HAL_UART_IRQHandler(& Uart3Handle);
    #endif
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

/****************************************************************END OF FILE****/

/**
  ******************************************************************************
  * @file    main.c 
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
#include "main.h" 
#include "app_main.h"

/** @addtogroup CORE
  * @{
  */

/** @defgroup MAIN
  * @brief main file
  * @{
  */ 

/** @defgroup MAIN_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup MAIN_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup MAIN_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup MAIN_Private_Variables
  * @{
  */
/**
  * @}   
  */ 

/** @defgroup MAIN_Private_FunctionPrototypes
  * @{
  */ 


/**
  * @}
  */ 

/** @defgroup MAIN_Private_Functions
  * @{
  */ 

/**
  * @brief  Main program
  * @param  None
  * @retval int
  */
int main(void)
{
    APP_Init();
    /* We should never get here as control is now taken by the scheduler */
    for( ;; );
}


#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

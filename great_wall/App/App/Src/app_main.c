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
#include "app_main.h"
//#include "biz_init.h"
#include "cmsis_os.h"
#include "common_def.h"
#include "hal_led.h"
#include "hal_init.h"
#include "gui.h"
#include "WM.h"
#include "serial_debug_log.h"
#include "hal_touch.h"
//#include "bsp_touch.h"



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
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void GUIThread(void const * argument);
static void TimerCallback(void const *n);
/* Private functions ---------------------------------------------------------*/
INTERFACE void HAL_HardWareInit(void);
/**
  * @brief  Initializes Hardware.
  * @param  None
  * @retval None
  */
static void APP_SetupHardware(void)
{
    /* Configure HardWareInit */
    //BIZ_Init();
    HAL_HardWareInit();
}

/**
  * @brief  Initializes LEDs, SDRAM, touch screen, CRC and SRAM.
  * @param  None
  * @retval None
  */
void APP_Init(void)
{
    osTimerId lcd_timer;
    /* hardware init. */
    APP_SetupHardware();

    /* Create GUI task */
    osThreadDef(GUI_Thread, GUIThread, osPriorityHigh, 0, 1000);
    osThreadCreate (osThread(GUI_Thread), NULL);
    /* Initialize GUI */


    //WM_SetCreateFlags(WM_CF_MEMDEV);
    GUI_Init();
    WM_MULTIBUF_Enable(1);

    GUI_SetBkColor(GUI_ORANGE);
    GUI_Clear();

    /* 设置文本显示模式 */
    //GUI_SetTextMode(GUI_TM_TRANS);


    GUI_SetFont(GUI_DEFAULT_FONT);
    GUI_SetColor(GUI_RED);
    GUI_DispStringAt("baixiaodong", 0, 0);
    GUI_DispStringAt("*************************************************************", 0, 100);
    GUI_SetBkColor(GUI_RED);
    GUI_ClearRect(5,20,95,110);
      /* Create Touch screen Timer */
    osTimerDef(TS_Timer, TimerCallback);
    lcd_timer =  osTimerCreate(osTimer(TS_Timer), osTimerPeriodic, (void *)0);
    /* Start the TS Timer */
    osTimerStart(lcd_timer, 200);

    GUI_X_InitOS();
    /* Start scheduler */
    osKernelStart();

    /* never go here */
    while(1);
}

/**
  * @brief This function provides accurate delay (in milliseconds) based
  *        on SysTick counter flag.
  * @note This function is declared as __weak to be overwritten in case of other
  *       implementations in user file.
  * @param Delay: specifies the delay time length, in milliseconds.
  * @retval None
  */

//void HAL_Delay (__IO uint32_t Delay)
//{
//  while(Delay)
//  {
//    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk)
//    {
//      Delay--;
//    }
//  }
//}

/**
  * @brief  Start task
  * @param  argument: pointer that is passed to the thread function as start argument.
  * @retval None
  */
static void GUIThread(void const * argument)
{
  /* Initialize Storage Units */
  //k_StorageInit();

  /* Check for calibration */
//  if(k_CalibrationIsDone() == 0)
//  {
//    k_CalibrationInit();
//  }

  /* Demo Startup */
  //k_StartUp();

  /* Show the main menu */
  //k_InitMenu();
   //MainTask();
  /* Gui background Task */
    while(1) 
    {
        //GUI_Exec(); /* Do the background work ... Update windows etc.) */
        osDelay(200); /* Nothing left to do for the moment ... Idle processing */
        HAL_LedTrogle();
//        cli_main_loop();
    
//    HAL_TouchScan();
//    if(g_tTouchInfo.flag == 1)
//    {
//        LOG(LEVEL_INFO,"1. Touch_x = %d,Touch_y = %d\r\n",g_tTouchInfo.x[0],g_tTouchInfo.y[0]);
//    }
    }
}

/**
  * @brief  Timer callbacsk (40 ms)
  * @param  n: Timer index
  * @retval None
  */
static void TimerCallback(void const *n)
{
 // HAL_LedTrogle();
 //BSP_Uart1SendChar('b');
 //LOG(5,"baixiaodong\r\n");
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

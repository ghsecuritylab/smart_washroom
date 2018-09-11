/**
  ******************************************************************************
  * @file    common_all_macro_def.h
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-08-26
  * @brief
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_ALL_MACRO_DEF_H
#define __COMMON_ALL_MACRO_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "common_def.h"

/**
 * Board type defined
 */
#define G_CONFIG_BOARD_TYPE_MACAN_V01_ST

#define G_CONFIG_USE_RTOS    1
#define G_CONFIG_USE_TWIN    0

#define G_CONFIG_DEV_LED    0
#define G_CONFIG_DEV_LCD    0
#define G_CONFIG_DEV_SDRAM    0
#define G_CONFIG_DEV_KEY    0
#define G_CONFIG_DEV_TOUCH    0
#define G_CONFIG_DEV_I2C    0
/**
 * Board type defined
 */
#define G_FUN_DEMO_TEST_LED       1
#define G_FUN_DEMO_TEST_SDRAM     1
#define G_FUN_DEMO_TEST_LCD       1
#define G_FUN_DEMO_TEST_TOUCH     1
#define G_FUN_DEMO_TEST_UART      1
#define G_FUN_DEMO_TEST_I2C       1
#define G_FUN_DEMO_TEST_KEY       1

#define G_CONFIG_CMD_DEBUG_FUNCTION  1


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Grugbai *****END OF FILE****/

/**
  ******************************************************************************
  * @file    common_def.h 
  * @author  Grugbai
  * @version V0.0.1
  * @date    2018-09-28 
  * @brief   Header for common_def.c file
  ******************************************************************************
  * @attention
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __COMMON_DEF_H
#define __COMMON_DEF_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdint.h>
     
#include "cmsis_os.h"     

	 
	 
/* define __IO */
#ifndef __IO
     
#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

#endif

#ifndef bool_t
typedef unsigned char bool_t;
#endif

#ifndef INTERFACE
#define INTERFACE extern
#endif

#ifndef FALSE
#define FALSE    0
#endif

#ifndef TRUE
#define TRUE      (1)
#endif

#ifndef MIN
#define MIN(n,m)   (((n) < (m)) ? (n) : (m))
#endif

#ifndef MAX
#define MAX(n,m)   (((n) < (m)) ? (m) : (n))
#endif

#ifndef ABS
#define ABS(n)     (((n) < 0) ? -(n) : (n))
#endif

/* calculate array length */
#define ARRAY_LEN(a)            (sizeof(a)/sizeof(a[0]))


/* takes a byte out of a uint32 : var - uint32,  ByteNum - byte to take out (0 - 3) */
#define BREAK_UINT32( var, ByteNum ) \
          (uint8_t)((uint32)(((var) >>((ByteNum) * 8)) & 0x00FF))

#define BUILD_UINT32(Byte0, Byte1, Byte2, Byte3) \
          ((uint32)((uint32)((Byte0) & 0x00FF) \
          + ((uint32_t)((Byte1) & 0x00FF) << 8) \
          + ((uint32_t)((Byte2) & 0x00FF) << 16) \
          + ((uint32_t)((Byte3) & 0x00FF) << 24)))

#define BUILD_UINT16(loByte, hiByte) \
          ((uint16_t)(((loByte) & 0x00FF) + (((hiByte) & 0x00FF) << 8)))

#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)

#define BUILD_UINT8(hiByte, loByte) \
          ((uint8_t)(((loByte) & 0x0F) + (((hiByte) & 0x0F) << 4)))

#define HI_UINT8(a) (((a) >> 4) & 0x0F)
#define LO_UINT8(a) ((a) & 0x0F)


typedef enum 
{
  COMMON_OK = 0,
  COMMON_ERROR,
  COMMON_BUSY,
  COMMON_TIMEOUT,
  COMMON_PARA_ERROR
}COMMON_StatusTypeDef;

#ifdef __cplusplus
}
#endif


#endif /* __COMMON_DEF_H */

/************************ (C) COPYRIGHT Grugbai *****END OF FILE****/

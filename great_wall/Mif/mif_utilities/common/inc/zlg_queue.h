/*********************************************Copyright (c)***********************************************
**                                Guangzhou ZLG MCU Technology Co., Ltd.
**
**                                        http://www.zlgmcu.com
**
**      广州周立功单片机科技有限公司所提供的所有服务内容旨在协助客户加速产品的研发进度，在服务过程中所提供
**  的任何程序、文档、测试结果、方案、支持等资料和信息，都仅供参考，客户有权不使用或自行参考修改，本公司不
**  提供任何的完整性、可靠性等保证，若在客户使用过程中因任何原因造成的特别的、偶然的或间接的损失，本公司不
**  承担任何责任。
**                                                                        ——广州周立功单片机科技有限公司
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               queue.c
** Last modified date:      2013-10-08
** Last version:            V1.0
** Descriptions:            数据队列的中间件
**
**--------------------------------------------------------------------------------------------------------
** Created by:              陈明计
** Created date:            2003-07-02
** Version:                 V0.91
** Descriptions:            The original version 初始版本
**--------------------------------------------------------------------------------------------------------
** Modified by:             李翔
** Modified date:           2013-10-08
** Version:                 V1.0
** Descriptions:            按代码规范整理
**
*********************************************************************************************************/
#ifndef __ZLG_QUEUE_H__
#define __ZLG_QUEUE_H__

//#include "config.h"                                                         /* 包含数据类型定义等       */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NOT_OK
#define NOT_OK              0xff                                            /* 参数错误                 */
#endif

#define QUEUE_FULL          8                                               /* 队列满                   */
#define QUEUE_EMPTY         4                                               /* 无数据                   */
#define QUEUE_OK            1                                               /* 操作成功                 */

#define Q_WRITE_MODE        1                                               /* 操作成功                 */
#define Q_WRITE_FRONT_MODE  2                                               /* 操作成功                 */

typedef unsigned char   INT8U;
typedef unsigned short  INT16U;
typedef unsigned long   INT32U;
#define NULL 0
#define OS_ENTER_CRITICAL()  __disable_irq()
#define OS_EXIT_CRITICAL()   __enable_irq()
    
#ifndef QUEUE_DATA_TYPE
#define QUEUE_DATA_TYPE     INT8U
#endif

struct dataqueue{
    QUEUE_DATA_TYPE     *Out;                                               /* 指向数据输出位置         */
    QUEUE_DATA_TYPE     *In;                                                /* 指向数据输入位置         */
    QUEUE_DATA_TYPE     *End;                                               /* 指向Buf的结束位置        */
    INT16U              usNData;                                            /* 队列中数据个数           */
    INT16U              usMaxData;                                          /* 队列中允许存储的数据个数 */
    
    INT8U               (* pfuncReadEmpty)();                               /* 读空处理函数             */
    INT8U               (* pfuncWriteFull)();                               /* 写满处理函数             */
    QUEUE_DATA_TYPE     Buf[1];                                             /* 存储数据的空间           */
};

typedef struct dataqueue DATAQUEUE;                                         /* 定义结构体类型           */
typedef DATAQUEUE *PDATAQUEUE;                                              /* 定义指针类型             */

/*********************************************************************************************************
** Function name:           queueCreate
** Descriptions:            初始化数据队列
** input parameters:        pBuf            ：为队列分配的存储空间地址
**                          ulSizeOfBuf     ：为队列分配的存储空间大小（字节）
**                          pfuncReadEmpty  ：为队列读空时处理程序
**                          pfuncWriteFull  ：为队列写满时处理程序
** output parameters:       NOT_OK   ：参数错误
**                          QUEUE_OK ：成功
** Returned value:          none
** Created by:              陈明计
** Created date:            2003-07-02
**--------------------------------------------------------------------------------------------------------
** Modified by:             Li Xiang   
** Modified date:           2013-09-25
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern INT8U queueCreate(void *pBuf,
                         INT32U ulSizeOfBuf,
                         INT8U (* pfuncReadEmpty)(),
                         INT8U (* pfuncWriteFull)()
                        );
                  
/*********************************************************************************************************
** Function name:           queueRead
** Descriptions:            获取队列中的数据
** input parameters:        Ret        ：存储返回的消息的地址
**                          pBuf       ：指向队列的指针
** output parameters:       NOT_OK     ：参数错误
**                          QUEUE_OK   ：收到消息
**                          QUEUE_EMPTY：无消息
** Returned value:          none
** Created by:              陈明计
** Created date:            2003-07-02
**--------------------------------------------------------------------------------------------------------
** Modified by:             Li Xiang   
** Modified date:           2013-09-25
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern INT8U queueRead(QUEUE_DATA_TYPE *Ret, void *pBuf);

/*********************************************************************************************************
** Function name:           queueWrite
** Descriptions:            FIFO方式发送数据
** input parameters:        pBuf       ：指向队列的指针
**                          Data       ：消息数据
** output parameters:       NOT_OK     ：参数错误
**                          QUEUE_FULL ：队列满
**                          QUEUE_OK   ：发送成功
** Returned value:          none
** Created by:              陈明计
** Created date:            2003-07-02
**--------------------------------------------------------------------------------------------------------
** Modified by:             Li Xiang   
** Modified date:           2013-09-25
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern INT8U queueWrite(void *pBuf, QUEUE_DATA_TYPE Data);

/*********************************************************************************************************
** Function name:           queueWriteFront
** Descriptions:            LIFO方式发送数据
** input parameters:        pBuf       ：指向队列的指针
**                          Data       ：消息数据
** output parameters:       NOT_OK     ：参数错误
**                          QUEUE_FULL ：队列满
**                          QUEUE_OK   ：发送成功
** Returned value:          none
** Created by:              陈明计
** Created date:            2003-07-02
**--------------------------------------------------------------------------------------------------------
** Modified by:             Li Xiang   
** Modified date:           2013-09-25
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern INT8U queueWriteFront(void *pBuf, QUEUE_DATA_TYPE Data);

/*********************************************************************************************************
** Function name:           queueNData
** Descriptions:            取得队列中数据数
** input parameters:        pBuf       ：指向队列的指针
** output parameters:       消息数
** Returned value:          none
** Created by:              陈明计
** Created date:            2003-07-02
**--------------------------------------------------------------------------------------------------------
** Modified by:             Li Xiang   
** Modified date:           2013-09-25
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern INT16U queueNData(void *pBuf);

/*********************************************************************************************************
** Function name:           queueSize
** Descriptions:            取得队列总容量
** input parameters:        pBuf       ：指向队列的指针
** output parameters:       队列总容量
** Returned value:          none
** Created by:              陈明计
** Created date:            2003-07-02
**--------------------------------------------------------------------------------------------------------
** Modified by:             Li Xiang   
** Modified date:           2013-09-25
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern INT16U queueSize(void *pBuf);

/*********************************************************************************************************
** Function name:           queueFlush
** Descriptions:            清空队列
** input parameters:        pBuf       ：指向队列的指针
** output parameters:       none
** Returned value:          none
** Created by:              陈明计
** Created date:            2003-07-02
**--------------------------------------------------------------------------------------------------------
** Modified by:             Li Xiang   
** Modified date:           2013-09-25
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern void queueFlush(void *pBuf);

//#define EN_QUEUE_WRITE            0                               /* 禁止(0)或允许(1)FIFO发送数据     */
//#define EN_QUEUE_WRITE_FRONT      0                               /* 禁止(0)或允许(1)LIFO发送数据     */
//#define EN_QUEUE_NDATA            0                               /* 禁止(0)或允许(1)取得队列数据数目 */
//#define EN_QUEUE_SIZE             0                               /* 禁止(0)或允许(1)取得队列数据总容量*/
//#define EN_QUEUE_FLUSH            0                               /* 禁止(0)或允许(1)清空队列         */
#ifdef __cplusplus
}
#endif

#endif
/*********************************************************************************************************
**                                                END FILE 
*********************************************************************************************************/

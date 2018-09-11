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
#define IN_QUEUE
#include "queue.h"                                                      

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
INT8U queueCreate(void *pBuf,
                  INT32U ulSizeOfBuf,
                  INT8U (* pfuncReadEmpty)(),
                  INT8U (* pfuncWriteFull)()
                 )
{
    DATAQUEUE *Queue;
    
    if (pBuf != NULL && ulSizeOfBuf >= (sizeof(DATAQUEUE))) {               /* 判断参数是否有效         */
    
        Queue = (DATAQUEUE *)pBuf;

        OS_ENTER_CRITICAL();
                                                                            /* 初始化结构体数据         */
        Queue->usMaxData = (ulSizeOfBuf - (INT32U)(((DATAQUEUE *)0)->Buf)) / 
                         sizeof(QUEUE_DATA_TYPE);                           /* 计算队列可以存储的数据数目*/
        Queue->End = Queue->Buf + Queue->usMaxData;                         /* 计算数据缓冲的结束地址   */
        Queue->Out = Queue->Buf;
        Queue->In  = Queue->Buf;
        Queue->usNData = 0;
        Queue->pfuncReadEmpty = pfuncReadEmpty;
        Queue->pfuncWriteFull = pfuncWriteFull;

        OS_EXIT_CRITICAL();

        return QUEUE_OK;
    } else {
        return NOT_OK;
    }
}
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
INT8U queueRead(QUEUE_DATA_TYPE *Ret, void *pBuf)
{
    INT8U       ucErr;
    DATAQUEUE   *Queue;

    ucErr = NOT_OK;
    if (pBuf != NULL) {                                                     /* 队列是否有效             */
                                                                            /* 有效                     */
        Queue = (DATAQUEUE *)pBuf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->usNData > 0) {                                           /* 队列是否为空             */
                                                                            /* 不空                     */
            *Ret = Queue->Out[0];                                           /* 数据出队                 */
            Queue->Out++;                                                   /* 调整出队指针             */
            if (Queue->Out >= Queue->End) {
                Queue->Out = Queue->Buf;
            }
            Queue->usNData--;                                               /* 数据减少                 */
            ucErr = QUEUE_OK;
        } else {                                                            /* 空                       */
            ucErr = QUEUE_EMPTY;
            if (Queue->pfuncReadEmpty != NULL) {                            /* 调用用户处理函数         */
            
                ucErr = Queue->pfuncReadEmpty(Ret, Queue);
            }
        }
        OS_EXIT_CRITICAL();
    }
    return ucErr;
}
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
#ifndef EN_QUEUE_WRITE
#define EN_QUEUE_WRITE      1
#endif

#if EN_QUEUE_WRITE > 0

INT8U queueWrite(void *pBuf, QUEUE_DATA_TYPE Data)
{
    INT8U       ucErr;
    DATAQUEUE   *Queue;

    ucErr = NOT_OK;
    if (pBuf != NULL) {                                                     /* 队列是否有效             */
    
        Queue = (DATAQUEUE *)pBuf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->usNData < Queue->usMaxData) {                            /* 队列是否满               */
                                                                            /* 不满                     */
            Queue->In[0] = Data;                                            /* 数据入队                 */
            Queue->In++;                                                    /* 调整入队指针             */
            if (Queue->In >= Queue->End) {
            
                Queue->In = Queue->Buf;
            }
            Queue->usNData++;                                               /* 数据增加                 */
            ucErr = QUEUE_OK;
        } else {                                                            /* 满                       */
            ucErr = QUEUE_FULL;
            if (Queue->pfuncWriteFull != NULL) {                            /* 调用用户处理函数         */
            
                ucErr = Queue->pfuncWriteFull(Queue, Data, Q_WRITE_MODE);
            }
        }
        OS_EXIT_CRITICAL();
    }
    return ucErr;
}

#endif
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
#ifndef EN_QUEUE_WRITE_FRONT
#define EN_QUEUE_WRITE_FRONT    0
#endif

#if EN_QUEUE_WRITE_FRONT > 0

INT8U queueWriteFront(void *pBuf, QUEUE_DATA_TYPE Data)
{
    INT8U ucErr;
    DATAQUEUE *Queue;

    ucErr = NOT_OK;
    if (pBuf != NULL) {                                                     /* 队列是否有效             */
    
        Queue = (DATAQUEUE *)pBuf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->usNData < Queue->usMaxData) {                            /* 队列是否满               */
                                                                            /* 不满                     */
            Queue->Out--;                                                   /* 调整出队指针             */
            if (Queue->Out < Queue->Buf) {
            
                Queue->Out = Queue->End - 1;
            }
            Queue->Out[0] = Data;                                           /* 数据入队                 */
            Queue->usNData++;                                               /* 数据数目增加             */
            ucErr = QUEUE_OK;
        } else {                                                            /* 满                       */
            ucErr = QUEUE_FULL;
            if (Queue->pfuncWriteFull != NULL) {                            /* 调用用户处理函数         */
            
                ucErr = Queue->pfuncWriteFull(Queue, Data, Q_WRITE_FRONT_MODE);
            }
        }
        OS_EXIT_CRITICAL();
    }
    return ucErr;
}

#endif
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
#ifndef EN_QUEUE_NDATA
#define EN_QUEUE_NDATA    1
#endif

#if EN_QUEUE_NDATA > 0

INT16U queueNData(void *pBuf)
{
    INT16U usTemp;
    
    usTemp = 0;                                                             /* 队列无效返回0            */
    if (pBuf != NULL) {
    
        OS_ENTER_CRITICAL();
        usTemp = ((DATAQUEUE *)pBuf)->usNData;
        OS_EXIT_CRITICAL();
    }
    return usTemp;
}

#endif
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
#ifndef EN_QUEUE_SIZE
#define EN_QUEUE_SIZE    0
#endif

#if EN_QUEUE_SIZE > 0

INT16U queueSize(void *pBuf)
{
    INT16U usTemp;
    
    usTemp = 0;                                                             /* 队列无效返回0            */
    if (pBuf != NULL) {
    
        OS_ENTER_CRITICAL();
        usTemp = ((DATAQUEUE *)pBuf)->usMaxData;
        OS_EXIT_CRITICAL();
    }
    return usTemp;
}

#endif
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
#ifndef EN_QUEUE_FLUSH
#define EN_QUEUE_FLUSH    0
#endif

#if EN_QUEUE_FLUSH > 0

void queueFlush(void *pBuf)
{
    DATAQUEUE *Queue;
    
    if (pBuf != NULL) {                                                     /* 队列是否有效             */
                                                                            /* 有效                     */
        Queue = (DATAQUEUE *)pBuf;
        OS_ENTER_CRITICAL();
        Queue->Out = Queue->Buf;
        Queue->In  = Queue->Buf;
        Queue->usNData = 0;                                                 /* 数据数目为0              */
        OS_EXIT_CRITICAL();
    }
}

#endif

/*********************************************************************************************************
**                                                END FILE 
*********************************************************************************************************/


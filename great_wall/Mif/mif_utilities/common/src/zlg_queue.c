/*********************************************Copyright (c)***********************************************
**                                Guangzhou ZLG MCU Technology Co., Ltd.
**
**                                        http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               queue.c
** Last modified date:      2013-10-08
** Last version:            V1.0
** Descriptions:            ���ݶ��е��м��
**
**--------------------------------------------------------------------------------------------------------
** Created by:              ������
** Created date:            2003-07-02
** Version:                 V0.91
** Descriptions:            The original version ��ʼ�汾
**--------------------------------------------------------------------------------------------------------
** Modified by:             ����
** Modified date:           2013-10-08
** Version:                 V1.0
** Descriptions:            ������淶����
**
*********************************************************************************************************/
#define IN_QUEUE
#include "queue.h"                                                      

/*********************************************************************************************************
** Function name:           queueCreate
** Descriptions:            ��ʼ�����ݶ���
** input parameters:        pBuf            ��Ϊ���з���Ĵ洢�ռ��ַ
**                          ulSizeOfBuf     ��Ϊ���з���Ĵ洢�ռ��С���ֽڣ�
**                          pfuncReadEmpty  ��Ϊ���ж���ʱ�������
**                          pfuncWriteFull  ��Ϊ����д��ʱ�������
** output parameters:       NOT_OK   ����������
**                          QUEUE_OK ���ɹ�
** Returned value:          none
** Created by:              ������
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
    
    if (pBuf != NULL && ulSizeOfBuf >= (sizeof(DATAQUEUE))) {               /* �жϲ����Ƿ���Ч         */
    
        Queue = (DATAQUEUE *)pBuf;

        OS_ENTER_CRITICAL();
                                                                            /* ��ʼ���ṹ������         */
        Queue->usMaxData = (ulSizeOfBuf - (INT32U)(((DATAQUEUE *)0)->Buf)) / 
                         sizeof(QUEUE_DATA_TYPE);                           /* ������п��Դ洢��������Ŀ*/
        Queue->End = Queue->Buf + Queue->usMaxData;                         /* �������ݻ���Ľ�����ַ   */
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
** Descriptions:            ��ȡ�����е�����
** input parameters:        Ret        ���洢���ص���Ϣ�ĵ�ַ
**                          pBuf       ��ָ����е�ָ��
** output parameters:       NOT_OK     ����������
**                          QUEUE_OK   ���յ���Ϣ
**                          QUEUE_EMPTY������Ϣ
** Returned value:          none
** Created by:              ������
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
    if (pBuf != NULL) {                                                     /* �����Ƿ���Ч             */
                                                                            /* ��Ч                     */
        Queue = (DATAQUEUE *)pBuf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->usNData > 0) {                                           /* �����Ƿ�Ϊ��             */
                                                                            /* ����                     */
            *Ret = Queue->Out[0];                                           /* ���ݳ���                 */
            Queue->Out++;                                                   /* ��������ָ��             */
            if (Queue->Out >= Queue->End) {
                Queue->Out = Queue->Buf;
            }
            Queue->usNData--;                                               /* ���ݼ���                 */
            ucErr = QUEUE_OK;
        } else {                                                            /* ��                       */
            ucErr = QUEUE_EMPTY;
            if (Queue->pfuncReadEmpty != NULL) {                            /* �����û�������         */
            
                ucErr = Queue->pfuncReadEmpty(Ret, Queue);
            }
        }
        OS_EXIT_CRITICAL();
    }
    return ucErr;
}
/*********************************************************************************************************
** Function name:           queueWrite
** Descriptions:            FIFO��ʽ��������
** input parameters:        pBuf       ��ָ����е�ָ��
**                          Data       ����Ϣ����
** output parameters:       NOT_OK     ����������
**                          QUEUE_FULL ��������
**                          QUEUE_OK   �����ͳɹ�
** Returned value:          none
** Created by:              ������
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
    if (pBuf != NULL) {                                                     /* �����Ƿ���Ч             */
    
        Queue = (DATAQUEUE *)pBuf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->usNData < Queue->usMaxData) {                            /* �����Ƿ���               */
                                                                            /* ����                     */
            Queue->In[0] = Data;                                            /* �������                 */
            Queue->In++;                                                    /* �������ָ��             */
            if (Queue->In >= Queue->End) {
            
                Queue->In = Queue->Buf;
            }
            Queue->usNData++;                                               /* ��������                 */
            ucErr = QUEUE_OK;
        } else {                                                            /* ��                       */
            ucErr = QUEUE_FULL;
            if (Queue->pfuncWriteFull != NULL) {                            /* �����û�������         */
            
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
** Descriptions:            LIFO��ʽ��������
** input parameters:        pBuf       ��ָ����е�ָ��
**                          Data       ����Ϣ����
** output parameters:       NOT_OK     ����������
**                          QUEUE_FULL ��������
**                          QUEUE_OK   �����ͳɹ�
** Returned value:          none
** Created by:              ������
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
    if (pBuf != NULL) {                                                     /* �����Ƿ���Ч             */
    
        Queue = (DATAQUEUE *)pBuf;
        
        OS_ENTER_CRITICAL();
        
        if (Queue->usNData < Queue->usMaxData) {                            /* �����Ƿ���               */
                                                                            /* ����                     */
            Queue->Out--;                                                   /* ��������ָ��             */
            if (Queue->Out < Queue->Buf) {
            
                Queue->Out = Queue->End - 1;
            }
            Queue->Out[0] = Data;                                           /* �������                 */
            Queue->usNData++;                                               /* ������Ŀ����             */
            ucErr = QUEUE_OK;
        } else {                                                            /* ��                       */
            ucErr = QUEUE_FULL;
            if (Queue->pfuncWriteFull != NULL) {                            /* �����û�������         */
            
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
** Descriptions:            ȡ�ö�����������
** input parameters:        pBuf       ��ָ����е�ָ��
** output parameters:       ��Ϣ��
** Returned value:          none
** Created by:              ������
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
    
    usTemp = 0;                                                             /* ������Ч����0            */
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
** Descriptions:            ȡ�ö���������
** input parameters:        pBuf       ��ָ����е�ָ��
** output parameters:       ����������
** Returned value:          none
** Created by:              ������
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
    
    usTemp = 0;                                                             /* ������Ч����0            */
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
** Descriptions:            ��ն���
** input parameters:        pBuf       ��ָ����е�ָ��
** output parameters:       none
** Returned value:          none
** Created by:              ������
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
    
    if (pBuf != NULL) {                                                     /* �����Ƿ���Ч             */
                                                                            /* ��Ч                     */
        Queue = (DATAQUEUE *)pBuf;
        OS_ENTER_CRITICAL();
        Queue->Out = Queue->Buf;
        Queue->In  = Queue->Buf;
        Queue->usNData = 0;                                                 /* ������ĿΪ0              */
        OS_EXIT_CRITICAL();
    }
}

#endif

/*********************************************************************************************************
**                                                END FILE 
*********************************************************************************************************/


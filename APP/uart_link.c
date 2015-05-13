#include "includes.h"
#include "lcdconf.h"

END_OBJ g_EndObjectPool[MAX_COM_PORT] =
{
    {PC_COM_PORT, END_STATUS_IDLE, 0, 0, 0, NULL, NULL, NULL, NULL},
    {RS485_COM_PORT, END_STATUS_IDLE, 0, 0, 0, NULL, NULL, NULL, NULL},
    {PLC_COM_PORT, END_STATUS_IDLE, 0, 0, 0, NULL, NULL, NULL, NULL},    
};

pvoid g_EndObject[END_OBJECT_NUM];

queue *g_EndTxQueue[MAX_COM_PORT] = {NULL};
queue *g_EndRxQueue[MAX_COM_PORT] = {NULL};

unsigned char g_PCTxEndQueueBuf[((END_TX_QUEUE_SIZE+2) * 4)];
unsigned char g_PCRxEndQueueBuf[((END_RX_QUEUE_SIZE+2) * 4)];

unsigned char g_RS485TxEndQueueBuf[((END_TX_QUEUE_SIZE+2) * 4)];
unsigned char g_RS485RxEndQueueBuf[((END_RX_QUEUE_SIZE+2) * 4)];

unsigned char g_PLCTxEndQueueBuf[((END_TX_QUEUE_SIZE+2) * 4)];
unsigned char g_PLCRxEndQueueBuf[((END_RX_QUEUE_SIZE+2) * 4)];

unsigned char *pTxEndQueueBuf[] = {
    g_PCTxEndQueueBuf,
    g_RS485TxEndQueueBuf,
    g_PLCTxEndQueueBuf,
};

unsigned char *pRxEndQueueBuf[] = {
    g_PCRxEndQueueBuf,
    g_RS485RxEndQueueBuf,
    g_PLCRxEndQueueBuf, 
};

/***********************************************************
END���ʼ��< // ��Ҫ�ṩһ��reset �ӿڹ��ϲ����>
************************************************************/

OS_EVENT *g_sem_end;

UART_CCB g_uart_ccb[MAX_COM_PORT];

U8 g_UartPCRxBuf[UART_RECEIVE_BUF_SIZE];
U8 g_UartRS485RxBuf[UART_RECEIVE_BUF_SIZE];
U8 g_UartPLCRxBuf[UART_RECEIVE_BUF_SIZE];

U8 *pUartRxBuf[] = {
    g_UartPCRxBuf,
    g_UartRS485RxBuf,
    g_UartPLCRxBuf,
};

U32 UART_ReceiveData(U8 end_id, UCHAR* rxbuf, USHORT rxnum)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr = 0;
#endif
    P_UART_CCB p_uc = &g_uart_ccb[end_id];

    if( (rxnum < 1) || (end_id >= MAX_COM_PORT) )
    {
        return FALSE;
    }
    OS_ENTER_CRITICAL();
    //p_uc->gUartRxCnt = 0;
    //p_uc->gUartRxLen = rxnum;
    p_uc->gpUartRxAddress = rxbuf;
    p_uc->gpUartRxStartAddress = rxbuf;
    p_uc->gpUartRxReadAddress = rxbuf;
    p_uc->gpUartRxEndAddress = (UCHAR*)((ULONG)rxbuf + rxnum);
    OS_EXIT_CRITICAL()
    return TRUE;
}


void End_Init(void)
{
    P_END_OBJ pEndObj = NULL;
    unsigned char i;
    

    g_EndObject[PC_COM_PORT] = (pvoid)&PC_UART;
    g_EndObject[RS485_COM_PORT] = (pvoid)&RS485_UART;
    g_EndObject[PLC_COM_PORT] = (pvoid)&PLC_UART;

    mem_msg_buffer_init((MSG_INFO *)gShortMsgPool, (P_MSG_INFO *)pShortMsgPool, MAX_MSG_SHORT, sizeof(MSG_SHORT_INFO));

    //alan test  ��Ҫ��ʱע�͵�, ��֪ΪɶIIC Start һ����, MCU �ͷ���.
    for( i = START_COM_PORT; i < MAX_COM_PORT; i++)
    {
        // �ҵ���ǰEnd Object
        pEndObj = g_EndObjectPool + i;

        /* end queue[x] initialize */ /* each end object define '50 block' queue */
        g_EndTxQueue[i] = define_new_queue((queue *)pTxEndQueueBuf[i], END_TX_QUEUE_SIZE);
        g_EndRxQueue[i] = define_new_queue((queue *)pRxEndQueueBuf[i], END_RX_QUEUE_SIZE);
       
        pEndObj->end_recv_buffer = (unsigned char *)pUartRxBuf[i];

        pEndObj->last_receive_len = 0;
        pEndObj->receive_len = 0;

        pEndObj->recv_timeout = 0;

        UART_ReceiveData(i, pEndObj->end_recv_buffer, UART_RECEIVE_BUF_SIZE);

        // ���д���״̬ת��REVC STATUS
        pEndObj->end_send_status = END_STATUS_IDLE;

        __HAL_UART_ENABLE_IT((UART_HandleTypeDef *)g_EndObject[i], UART_IT_RXNE);
    }
}


/***********************************************************
Tick������ã����ÿ��END�ӿ��Ƿ����µ�frame����
************************************************************/
unsigned short End_tick_check(void)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr = 0;
#endif
    unsigned char i;
    U16 cp_len, msg_len;

    P_END_OBJ     pEndObj = NULL;
    P_MSG_INFO     pnewmsg = NULL;

    P_UART_CCB p_uc;

    for(i = START_COM_PORT; i < MAX_COM_PORT; i++ )
    {
        /////////////////////////////
        pEndObj = g_EndObjectPool + i;

        p_uc = &g_uart_ccb[i];

        if(pEndObj->end_send_status == END_STATUS_SENDING)
        {
            /* ��鵱ǰ�����Ƿ������. ��ϵĻ�, ת����,   ����ת����*/
            if( End_check_send(pEndObj->end_id) == TRUE )
            {

                //Send  �Ѿ������� ת��receive status ��ʼ״̬
                pEndObj->end_send_status = END_STATUS_IDLE;

                pnewmsg = (P_MSG_INFO)pEndObj->pMsgInfo;

                pnewmsg->msg_header.block_state = SENDED;

                // alan dynamic sending buffer.
                // ��ǰend sending ��������������ֱ���ͷ�sending buffer
                if( pnewmsg->msg_header.need_buffer_free == TRUE)    /* ����Ҫ�������ϲ�application �����ͷ�*/
                {
                    free_send_buffer(pnewmsg);
                }
            }
        }
        else
        {
            if( NULL != (pnewmsg = dequeue( g_EndTxQueue[pEndObj->end_id])) )
            {
                End_send(pnewmsg);
            }
        }

        if( End_check_recv(pEndObj) == TRUE ) /* û�������ݵ���, ��ʼ����*/
        {
#if 0            
            // ���ڴ�����copy  ����, ѹ��queue ���к�������
            if(pEndObj->receive_len >= MSA_MESSAGAE_MAX_SIZE)
                pnewmsg = alloc_send_buffer(MSG_LARGE);
            else if(pEndObj->receive_len >= PLC_BUFFER_UART_SIZE)
                pnewmsg = alloc_send_buffer(MSG_LONG);
            else
                pnewmsg = alloc_send_buffer(MSG_SHORT);
#else
            pnewmsg = alloc_send_buffer(MSG_SHORT);
#endif

            if(pnewmsg == NULL)
            {               
                continue;
            }

            msg_len = pEndObj->receive_len;

            pnewmsg->msg_header.msg_len = msg_len;
            pnewmsg->msg_header.end_id = i;

            OS_ENTER_CRITICAL();
            cp_len = (U16)(p_uc->gpUartRxEndAddress - p_uc->gpUartRxReadAddress);
            

            if(cp_len >= msg_len)
            {
                //OS_ENTER_CRITICAL();
                memcpy(pnewmsg->msg_buffer, p_uc->gpUartRxReadAddress, msg_len );                
                p_uc->gpUartRxReadAddress += msg_len;
                if(p_uc->gpUartRxReadAddress == p_uc->gpUartRxEndAddress)
                    p_uc->gpUartRxReadAddress = p_uc->gpUartRxStartAddress;
                //OS_EXIT_CRITICAL();
            }
            else
            {
                //OS_ENTER_CRITICAL();
                memcpy(pnewmsg->msg_buffer, p_uc->gpUartRxReadAddress, cp_len);
                memcpy((pnewmsg->msg_buffer + cp_len), p_uc->gpUartRxStartAddress, (msg_len-cp_len));                
                p_uc->gpUartRxReadAddress = p_uc->gpUartRxStartAddress + (msg_len-cp_len);
                //OS_EXIT_CRITICAL();
            }
            OS_EXIT_CRITICAL();

            pEndObj->last_receive_len = 0;
            pEndObj->receive_len = 0;

            pEndObj->endStatistics.rxPacketCount++;

            /* �����ݹ���queue ��*/
            if(enqueue(g_EndRxQueue[pEndObj->end_id], (P_MSG_INFO)(pnewmsg)))
            {			
                // ����UART Rx �����¼���
                OSSemPost(g_sem_end);
            }
            else
            {
                free_send_buffer(pnewmsg);
            }       
        }
    }

    return TRUE;
}


P_END_OBJ End_get_end_obj(UCHAR end_id)
{
    unsigned char i = 0;
    while( i < MAX_COM_PORT)
    {
        if( g_EndObjectPool[i].end_id == end_id)
            return &g_EndObjectPool[i];
        i++;
    }

    return NULL;
}

U32 End_uart_send(UCHAR end_id,  UCHAR* txbuf, USHORT    txnum )
{
    UART_HandleTypeDef huart;
    P_UART_CCB p_uc;
    UCHAR      send_byte=0;

    if( txnum < 1 )
    {
        return FALSE;
    }

    switch(end_id)
    {    
    case PC_COM_PORT:      
        huart = PC_UART;
        break; 
        
    case RS485_COM_PORT:      
        huart= RS485_UART;
        break; 
        
    case PLC_COM_PORT:      
        huart = PLC_UART;
        break; 
        
    default:
        return FALSE;
    }

    p_uc = &g_uart_ccb[end_id];

    p_uc->gpUartTxAddress = txbuf;
    p_uc->gUartTxCnt = txnum;

    send_byte = *(p_uc->gpUartTxAddress);

    p_uc->gpUartTxAddress++;
    p_uc->gUartTxCnt--;

    while(RESET == __HAL_UART_GET_FLAG(&huart, UART_FLAG_TXE)); //�ؼ�

    huart.Instance->DR = (send_byte & (uint8_t)0xFF);

    while(RESET == __HAL_UART_GET_FLAG(&huart, UART_FLAG_TC)); //�ؼ�

    if(p_uc->gUartTxCnt)
        __HAL_UART_ENABLE_IT(&huart, UART_IT_TXE);
        
    return TRUE;
};

/***********************************************************
pEndObj:  ���ͽӿ�
pMsgInfo: ������Ϣ����ͨ����·�㷢����Ϣ
************************************************************/
unsigned short End_send( P_MSG_INFO pMsgInfo)
{
    P_END_OBJ pEndObj;

    if( (( pEndObj = End_get_end_obj(pMsgInfo->msg_header.end_id)) == NULL) ||
            ( pMsgInfo->msg_header.msg_len == 0) )
    {
        // alan dynamic sending buffer.
        // ��Щ����ʧ��ʱ�����ﲻֱ���ͷ�sending buffer����end send �����ͷš�
        if( pMsgInfo->msg_header.need_buffer_free == TRUE)
        {
            free_send_buffer(pMsgInfo);
        }

        return FALSE;
    }


    if( END_IDLE != End_IsIdle(pEndObj) )
    {
        /* ����IDLE State, �����ݹ���queue ��*/
        enqueue(g_EndTxQueue[pEndObj->end_id], pMsgInfo);

        pMsgInfo->msg_header.time_stamp = OSTimeGet();

        return TRUE;
    }


    if( TRUE == End_uart_send( pEndObj->end_id, pMsgInfo->msg_buffer, pMsgInfo->msg_header.msg_len) )
    {

        // ����buffer ֱ�ӹ���end object �� ��������ֱ���ͷ�!
        pEndObj->pMsgInfo = pMsgInfo;

        // �ײ�״̬Ǩ�Ƶ�SENDING
        pEndObj->end_send_status = END_STATUS_SENDING;

        pMsgInfo->msg_header.block_state = SENDING;

        pEndObj->endStatistics.txPacketCount++;

        pMsgInfo->msg_header.time_stamp = OSTimeGet();

        
        return TRUE;
    }
    else
    {
        if( pMsgInfo->msg_header.need_buffer_free == TRUE)
        {
            free_send_buffer(pMsgInfo);
        }

        return FALSE;
    }

}

unsigned char End_check_recv(P_END_OBJ pEndObj)
{
    P_UART_CCB p_uc = &g_uart_ccb[pEndObj->end_id];

    // ʵʱ��¼��ǰUART Rx ���ݳ���
    pEndObj->last_receive_len = pEndObj->receive_len;

    //pEndObj->receive_len = gUart1RxCnt;
    if(p_uc->gpUartRxReadAddress <= p_uc->gpUartRxAddress)
        pEndObj->receive_len = p_uc->gpUartRxAddress - p_uc->gpUartRxReadAddress;//gIic0RxCnt;
    else
        pEndObj->receive_len = (USHORT)((ULONG)p_uc->gpUartRxAddress + UART_RECEIVE_BUF_SIZE - (ULONG)p_uc->gpUartRxReadAddress);

    if(pEndObj->receive_len > (220))//if(pEndObj->receive_len > (UART_RECEIVE_BUF_SIZE/2))
    {
        pEndObj->recv_timeout = 0;
        return TRUE;
    }


    if((pEndObj->receive_len != 0) && (pEndObj->receive_len == pEndObj->last_receive_len))
    {
        /* û���µĽ������ݵ���,ֱ�ӷ���FALSE �����ϲ���Դ�����*/
        pEndObj->recv_timeout++;
        if(pEndObj->recv_timeout > 1)
        {
            pEndObj->recv_timeout = 0;
            return TRUE;
        }
        else
        {
            return FALSE;
        }

    }
    else
    {
        /* �����µĽ�������*/
        pEndObj->recv_timeout = 0;
        return FALSE;
    }

}

/**********************************************

�жϵ�ǰEnd Send ���ݷ�����û��. �����귵��TRUE,  û�����귵��FALSE

**********************************************/
unsigned char End_check_send(UCHAR end_id)
{

    if( g_uart_ccb[end_id].gUartTxCnt == 0)
        /* �����������, ����TRUE �����ϲ�״̬Ǩ��*/
        return TRUE;
    else
        /* ��������û�������, ��������*/
        return FALSE;

}


/**************************************************************************************

��������:       �ײ㳭�����ݻ����ص�����. �����ｫ����д����ڴ�, ������һ�����д.

�������:      �س����ݽ��.

**************************************************************************************/
unsigned char End_postProcess(unsigned char end_type,  pvoid h)
{
    P_MSG_INFO pMsg = (P_MSG_INFO)h;
    USHORT iRet = FALSE;
    

    switch(end_type)
    {
    case PC_COM_PORT:
        iRet = PC_postProcess(pMsg);
        break;

    case RS485_COM_PORT:
        iRet = RS485_postProcess(pMsg);
        break;    

    case PLC_COM_PORT:
        iRet = PLC_postProcess(pMsg);
        break;        

    default:
        break;  
    }

    free_send_buffer(pMsg);

    return iRet;
}


// ��ѯ�ײ�end ��ǰ״̬�Ƿ����??
unsigned char End_IsIdle(P_END_OBJ pEndObj)
{

    if( pEndObj->end_send_status == END_STATUS_IDLE)
        return END_IDLE;
    else
        return END_BUSY;
    

}

/*
*********************************************************************************************************
*                                             App_TaskEndTick()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskEndTick()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
void  App_TaskEndTick (void *p_arg)
{    
    INT32U key_press_count = 0;


    (void)p_arg;
    
    while (DEF_TRUE) {    
        End_tick_check();
        
#if (LED_UART_EN > 0u)
        LED_UART_OFF();
#endif

        GUI_Msg_Upload(OFF);
        GUI_Msg_Download(OFF);
        
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
        {
            key_press_count++;
            
            if(key_press_count > 150)
            {
                DEV_Power_Off();
            }
        }
        else
        {
            key_press_count = 0;
        }
        
        OSTimeDlyHMSM(0, 0, 0, 20);
    }
}

/*
*********************************************************************************************************
*                                             App_TaskEndProc()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskEndProc()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
void  App_TaskEndProc (void *p_arg)
{
    INT8U  i, err;
    P_END_OBJ  pEndObj = NULL;
    P_MSG_INFO  pMsg = NULL;
    

    (void)p_arg;
    
    while (DEF_TRUE) { 
        OSSemPend(g_sem_end, 0, &err);

        for(i = START_COM_PORT; i < MAX_COM_PORT; i++)
        {
            pEndObj = g_EndObjectPool + i;

            while((pMsg = dequeue(g_EndRxQueue[pEndObj->end_id])) != NULL)
            {
                if(pMsg->msg_header.end_id == pEndObj->end_id)
                {
                    End_postProcess(pEndObj->end_id, pMsg);
                }
                else
                {
                    free_send_buffer(pMsg);
                }
            }

            
        }
    }
}

void USART_IRQProc(UART_CCB *uccb, UART_HandleTypeDef *huart)
{
  uint32_t tmp1 = 0, tmp2 = 0;

    
  tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_PE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE); 

  /* UART parity error interrupt occurred ------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  { 
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_PE);
    
    huart->ErrorCode |= HAL_UART_ERROR_PE;
  }
  
  tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_FE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
  /* UART frame error interrupt occurred -------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  { 
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_FE);
    
    huart->ErrorCode |= HAL_UART_ERROR_FE;
  }
  
  tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_NE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
  /* UART noise error interrupt occurred -------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  { 
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_NE);
    
    huart->ErrorCode |= HAL_UART_ERROR_NE;
  }
  
  tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
  /* UART Over-Run interrupt occurred ----------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  { 
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);
    
    huart->ErrorCode |= HAL_UART_ERROR_ORE;
  }
  
  tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
  /* UART in mode Receiver ---------------------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  { 
    //UART_Receive_IT(huart);
    
    *uccb->gpUartRxAddress++ = (uint8_t)(huart->Instance->DR & (uint8_t)0x00FF);
    
    if(uccb->gpUartRxAddress == uccb->gpUartRxEndAddress)
    {
      //��ͷ
      uccb->gpUartRxAddress = uccb->gpUartRxStartAddress;
    }
	
    if(uccb->gpUartRxReadAddress == uccb->gpUartRxAddress)
    {
      //���Կ��ǼӴ���ͳ��
    } 
    
    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_RXNE);

#if (LED_UART_EN > 0u)
    LED_UART_ON();
#endif    
  }
  
  tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TXE);
  tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE);
  /* UART in mode Transmitter ------------------------------------------------*/
  if((tmp1 != RESET) && (tmp2 != RESET))
  {
    //UART_Transmit_IT(huart);

    if(uccb->gUartTxCnt > 0)
    { 
      huart->Instance->DR = (uint8_t)(*uccb->gpUartTxAddress++ & (uint8_t)0x00FF);
      uccb->gUartTxCnt--;
    }
    else
    {
      __HAL_UART_DISABLE_IT(huart, UART_IT_TXE);
    }

    __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_TXE);

#if (LED_UART_EN > 0u)
    LED_UART_ON();
#endif	    
  }
  
  if(huart->ErrorCode != HAL_UART_ERROR_NONE)
  {
    /* Set the UART state ready to be able to start again the process */
    huart->State = HAL_UART_STATE_READY;
    
    HAL_UART_ErrorCallback(huart);
  }    
}

void UART4_IRQHandler(void)
{
    USART_IRQProc(&g_uart_ccb[PC_COM_PORT], &huart4);    
}

void USART2_IRQHandler(void)
{
    USART_IRQProc(&g_uart_ccb[PLC_COM_PORT], &huart2);    
}

void USART1_IRQHandler(void)
{
    USART_IRQProc(&g_uart_ccb[RS485_COM_PORT], &huart1);    
}

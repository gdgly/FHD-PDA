/******************************************************************************

 *
 * ---------------------------------------------------------------------------
 * Copyright (c) 2013 Mega Hunt Micro Technology Inc. All Rights Reserved.
 *****************************************************************************/
#include "includes.h"
#include "si4438_sub.h"
#include "si4438.h"

#define RF_FREQ_470MHz





#ifdef RF_FREQ_470MHz

rf_param  g_rf_param;
u8 RF_LED_Timeout;

u8 g_plc_buf[100]={0};



	
/*****************************************************************************
 * Name			: SI_Init
 * Function		: ��ʼ��MH1803 �Լ���صļĴ���
 * ---------------------------------------------------------------------------
 * Input Parameters	: ��
 * Output Parameters	: ��
 * Return Value		: 
 	��������:rf_status_t
 	RF_STATUS_FALSE:��ʼ��ʧ��
 	RF_STATUS_SUCCESS:��ʼ���ɹ�
 	RF_STATUS_RESET:��λ�쳣 	
 * ---------------------------------------------------------------------------
 * Description		:
 *****************************************************************************/
 RF_chen_C	RF_chen={0,0};
 void RF_Set_Channel(u8 chen)
 {
	RF_chen.chen_new = chen;
 }

/*****************************************************************************
 * Name			: RF_Tx
 * Function		: �������ݣ����ⲿģ�����
 * ---------------------------------------------------------------------------
 * Input Parameters	: buf :�������ݻ��������׵�ַ
 *			  len :���������ݵĳ��ȣ���λ�ֽ�
 * Output Parameters	: ��
 * Return Value		: 
 		RF_STATUS_SUCCESS: ���ͳɹ�
 *		RF_STATUS_RESET:�����и�λ
 		RF_STATUS_TIMEOUT:����ʧ��
 
 * ---------------------------------------------------------------------------
 * Description		:�ڵ��ñ�����ǰ��Ҫ��ѯis_rf_tx_blocked()�Ƿ�Ϊ��(����Ƶ����)��
 ֻ����Ƶ���в��ܵ��� ��rf_tx������������ܻᵼ�³��������ڱ�������	 	
 *****************************************************************************/
rf_status_t RF_Tx(u8 * buf, u8 len)
{
    u16 crc;

    
    DISABLE_RF_INT();
    memmove(buf + 1, buf, len); //����ȫ������Ųһ��λ��
    buf[0] = len;
    crc = cal_crc_ITU(buf, len + 1);
    
    buf[len + 1] = (u8)crc;
    buf[len + 2] = (u8)(crc >> 8);
    g_rf_param.tx.buf = buf;
    g_rf_param.tx.tx_len = len + 3;
    
    g_rf_param.tx.Index = SI_Send_Packet(len + 3, buf);

    ENABLE_RF_INT();
    g_rf_param.rf_state = RF_STATE_TX; 
    g_rf_param.tx.Timeout = 1;
    
    
    
    
    
    return (RF_STATUS_SUCCESS);
}




	
/*****************************************************************************
 * Name			: rf_interrupt
 * Function		: �жϴ�����
 * ---------------------------------------------------------------------------
 * Input Parameters	: ��
 * Output Parameters	: ��
 * Return Value		: ��
 *
 * ---------------------------------------------------------------------------
 * Description		:����������MH1803���ж�,��������жϡ�����
 * ����жϡ����� �����жϡ����ͽ����жϡ����ͽ����ж� 	
 * 	
 ******************************************************************************/
//------------------------------��Ƶ�¼�����----------------------------------------

void RF_Event_Handle(void)
{
    u8 sta1,L;

    //LED_Toggle(0);

    //GPIO_ResetBits(GPIOA, GPIO_PIN_0);
    
    sta1 = SI_Read_INT_Info(); 

    if(sta1 & RXPKT_DONE) 
    {//�������ݰ�����жϵĴ���

        //LED_On(0);
		
        g_rf_param.RSSI = SI_Read_RSSI();
        if (RF_STATE_RX != g_rf_param.rf_state)
        {
            _sys_plc_stop();
            g_rf_param.rx.rx_index = 0;
            g_rf_param.rf_state = RF_STATE_RX;
            g_rf_param.rx.rx_len = SI_Read_Length();
            
            //Read_PayLoad(g_rf_param.rx.buf+g_rf_param.rx.rx_index);
        }
        
        SI_Read_Fifo(   g_rf_param.rx.rx_len - g_rf_param.rx.rx_index,
                        g_rf_param.rx.buf + g_rf_param.rx.rx_index );
        g_rf_param.rx.rx_index = g_rf_param.rx.rx_len;

        
        RF_Listen();

        //LED_Off(0);



        //g_rf_param.rx.rx_len=0; //����
        g_rf_param.rx.Timeout=0;
		//_sys_plc_reset();
        OSSemPost(g_sem_rf); //����
        OSSemPost(g_sem_chk_rf); //����
	}
	 else if (sta1 & RXFF_AF) 
	{   
		if (RF_STATE_RX != g_rf_param.rf_state)
		{              
			 _sys_plc_stop();
			g_rf_param.rx.rx_index = 0;
			g_rf_param.rf_state = RF_STATE_RX;
            g_rf_param.rx.rx_len =SI_Read_Length();//Read_PayLoad(g_rf_param.rx.buf+g_rf_param.rx.rx_index);
		}
		SI_Read_Fifo(RX_AF_THRESHOLD,g_rf_param.rx.buf+g_rf_param.rx.rx_index);
		g_rf_param.rx.rx_index+=RX_AF_THRESHOLD;
		g_rf_param.rx.Timeout=1;
		//_sys_plc_reset();

	}
	else if(g_rf_param.rf_state == RF_STATE_TX)
	{
		if (sta1 & TXFF_AE) 
		{
			if(g_rf_param.tx.tx_len>g_rf_param.tx.Index)
				{
					L = SI_Write_Fifo(g_rf_param.tx.tx_len - g_rf_param.tx.Index,
                                            g_rf_param.tx.buf + g_rf_param.tx.Index);
					g_rf_param.tx.Index += L;
					g_rf_param.tx.Timeout = 1;
					TX_LED_ON(); 
                    //RF_Cnt=0;
				}
                     
		}
                //else
		if (sta1 & TXPKT_DONE) 
		{
		    //�������ݰ�����жϵĴ���
			TX_LED_ON();         
            //��������LED
//#ifdef RF_Mon_TS
            if(RF_chen.chen_new^RF_chen.chen_old)
            {
                SI_Freq_Set(RF_chen.chen_new/33,RF_chen.chen_new%33);
                RF_chen.chen_old=RF_chen.chen_new;
            }
//#endif
            RF_Listen();
            g_rf_param.tx.Timeout=0;
            //LED_Off(0);
            _sys_plc_reset();
		}
	}
    else
    {
        RF_Listen();
    }

    //GPIO_SetBits(GPIOA, GPIO_PIN_0);

   
}   
void RF_Channel_Update(u8 chen)
{
	SI_Freq_Set(chen/2,chen%2);
}

u8 RF_Read_Sig(void)
{
   // memcpy(buf,(u8 *)&g_rf_param.RSSI,4);

    return(g_rf_param.RSSI);
}

void RF_Timeout_Sub(void)
{
   
    if(g_rf_param.rx.Timeout)
    {
        g_rf_param.rx.Timeout++;
        if(g_rf_param.rx.Timeout>30)//����300ms��ʱ
        {
            RF_Listen();
            g_rf_param.rx.Timeout=0;
        }
       
    }
}
/*****************************************************************************
 * Name			:  RF_Reset()
 * Function		: ��λ��ƵоƬ����ʹ�䴦�ڼ���״̬������������ͨ��������
 * ---------------------------------------------------------------------------
 * Input Parameters	: ��	
 * Output Parameters	: ��
 * Return Value		: ��
 *
 * ---------------------------------------------------------------------------
 * Description		:
 * *****************************************************************************/	
void RF_Reset()
{
	SI_Init();
}

/*****************************************************************************
 * Name			:  RF_Listen()
 * Function		: ʹ��Ƶת��������״̬������������ͨ��������
 * ---------------------------------------------------------------------------
 * Input Parameters	: ��	
 * Output Parameters	: ��
 * Return Value		: ��
 *
 * ---------------------------------------------------------------------------
 * Description              :
 * *****************************************************************************/
void RF_Listen()
{
	SI_Enter_Rx();
	g_rf_param.rf_state = RF_STATE_LISTEN;
}

void RF_Int_Proc()
{
    if(RESET != __HAL_GPIO_EXTI_GET_IT(GPIO_PIN_14)) 
    {   
        __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_14);  
        RF_Event_Handle();
    }  

}

void RF_Init(void)
{
    //u8 val;

    if((SpiHandle.Instance->CR1 &SPI_CR1_SPE) != SPI_CR1_SPE)
    {
      /* Enable SPI peripheral */
      __HAL_SPI_ENABLE(&SpiHandle);
    }

	//DISABLE_RF_INT();	//�ر��ⲿ�ж�
	RF_PWR_ON();
	
	OSTimeDly(2);    

    BSP_IntVectSet(BSP_INT_ID_EXTI15_10, RF_Int_Proc);
    DISABLE_RF_INT();

    DISABLE_RF_CHIP(); // �ر���ƵоƬ
    	
    OSTimeDly(2);
    
	ENABLE_RF_CHIP(); // ʹ����ƵоƬ

    OSTimeDly(300);   //��3�������в鿴��λ�Ƿ����	


    

    g_rf_param.rx.buf=RF_buf;
    g_rf_param.rx.rx_index=0;
    g_rf_param.rx.rx_len=0;
    g_rf_param.rx.Timeout=0;
    g_rf_param.tx.buf=RF_buf;
    g_rf_param.tx.Index=0;
    g_rf_param.tx.tx_len=0;
    g_rf_param.tx.Timeout=0;



    SI_Init();
  
    

    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_14);
    
    ENABLE_RF_INT();	           //�����ⲿ�ж�
    
    
}


//#endif
#endif

/*****************************************************************************

 * ---------------------------------------------------------------------------
 * Modification Record:
 *   Version		Name			date 		Content
 *     1.0		lihongxu         2013-05-08	        Create this file.
 *
 * Note			: �жϷ�ʽ�շ�ͷ�ļ�
 *
 * ---------------------------------------------------------------------------
 * Copyright (c) 2013 Mega Hunt Micro Technology Inc. All Rights Reserved.
 *****************************************************************************/
#include"Includes.h"

#ifndef RADIO_MH1803_INTERRUPT_H
#define RADIO_MH1803_INTERRUPT_H

#define MAX_RF_PKT_LEN	256 	//�ɻ����������

#define RF_DEBUG	0		//��Ƶ���Կ��� 1:������ӡ���� 0:�رմ�ӡ����

#define RF_COMMON_REG_CHANGED	1	//0:ʹ��оƬĬ������(����)	1:�޸ĵ���ģʽ��ǰ�����ȡ�ͬ���֡�Header��


#define TX_LED_ON()     RF_LED_Timeout=1;//(P2 &= 0xFD)		//��������LED
#define TX_LED_OFF()    RF_LED_Timeout=0;//(P2 |= 0x02)		//�رշ���LED
#define RX_LED_ON()     RS485_Wait_Timeout(0,40)//(P7 &= 0xF7)		//��������LED
#define RX_LED_OFF()    (P3 |= 0x08)	//�رս���LED	



#define ENABLE_RF_CHIP()	GPIO_ResetBits(GPIOG,GPIO_PIN_15);   //sdn 0//P2&=0xBF;//P8&=0xFD;//1803Ϊ�͵�ƽʹ��оƬ
#define DISABLE_RF_CHIP()	GPIO_SetBits(GPIOG,GPIO_PIN_15);   //sdn 1//P2|=0x40;//P8|=0x02;//1803Ϊ�ߵ�ƽ����оƬ]

#define HAVE_INTERRUPT()	//P8&0x04
#define DISABLE_RF_INT()    HAL_NVIC_DisableIRQ(EXTI15_10_IRQn)     //INTP7_Disable()		           //�����ⲿ�ж�
#define ENABLE_RF_INT()	    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn)      //INTP7_Enable()
#define RF_INT_CLEAR()	    __HAL_GPIO_EXTI_CLEAR_FLAG(GPIO_PIN_14);  
//�����ⲿ�ж�

#define  RF_buf g_plc_buf

#define _sys_plc_stop() 
#define _sys_plc_reset()

#define SI_TYPE     0x4438

enum
{
	RF_STATE_IDLE,	//��Ƶ����״̬
	RF_STATE_LISTEN,	//��Ƶ����״̬
	RF_STATE_RX,		//��Ƶ����״̬
	RF_STATE_TX		//��Ƶ����״̬
};

 
#pragma pack(1)
typedef struct
{
    u16 Val:15;
    u16 Sig:1;
}RF_Sig_C;
typedef struct
{
    RF_Sig_C RSSI;
    RF_Sig_C FREQ;
}RF_Signal_C;
typedef struct
{
	u8 chen_new;
	u8 chen_old;
}RF_chen_C;


typedef struct rf_param
{
	u8 rf_state;          			 //��ǰ��Ƶ����״̬
	struct rx 
	{//�������ݽṹ	
		u8 *buf;		    //���ջ��������255
		u8 rx_index;	    //ͳ�ƽ��յ��ĳ��ȵ��ܺͣ�ÿ��һ���жϾͽ�pkt_len����ֵ�ۼ�����
		u8 rx_len;			//ͨ����ֵ��ȷ�����жϵĳ��ȣ�������ɵĳ��Ȼ��߷�ֵ�ĳ���
		u8 Timeout;
	}rx;
	
	struct tx 
	{//�������ݽṹ
		u8 *buf;  		//�������ݵĻ�����
		u8 Index;			//���������ݵ�ָ�룬ָ����һ��Ҫ���͵��ֽڵ�ַ
		u8 tx_len;			//ʣ����������ݵĳ���
		u8 Timeout;
	}tx;
    u8 RSSI;
    short cfo;
}rf_param;

#pragma pack()
/////////////////////////// End �弶��ض��� ///////////////////////////

typedef enum {
	RF_STATUS_SUCCESS = 0,		//�������ݰ��ɹ����������ݰ��ɹ����ʼ���ɹ�
	RF_STATUS_FALSE,			//����յ��ĳ���Ϊ0�򳤶ȳ������������޷����ա�������ʧ��
	RF_STATUS_TIMEOUT,		//���ջ��ͳ�ʱ
	RF_STATUS_RESET,			//оƬ������λ������ϳ���������оƬ�����˸�λ����
	RF_STATUS_RXWAIT,			//����ȴ����չ�����
}rf_status_t;
#define RF_AIR_BAUD_RATE 2400
#define RF_STATISTICS
#define MH1803_frm_change(x,y) RF_Frame_chang(x,y)
extern rf_param g_rf_param;
/*****************************************************************************
 * Name			: SI_Init
 * Function		: ��ʼ��MH1803��ƵоƬ����ģ���ȫ�ֱ���
 * ---------------------------------------------------------------------------
 * Input Parameters	: ��	
 * Output Parameters	: ��
 * Return Value		: ��
 *
 * ---------------------------------------------------------------------------
 * Description		: �ڽ��в���SPIǰ��Ҫ�ر��ж�(���ٱ�֤��Ƶ�жϹر�)��Ȼ����
 *	 		  ͨ��spi	����оƬ��оƬ��ʼ����Ϻ�Ϊ����״̬���ٽ�оƬת����
 *	 		  ����״̬��������жϡ�
 *	 	             ��ģ���ʼ��uart/spi/timer����ô˳�ʼ������
 * 	
 *****************************************************************************/
rf_status_t SI_Init();


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
rf_status_t RF_Tx(u8 * buf, u8 len);


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
void RF_Reset();





/*****************************************************************************
 * Name				: read_reg_intr
 * Function			: ����ƵоƬ�Ĵ��������ݣ�����������ͨ��������
 * ---------------------------------------------------------------------------
 * Input Parameters	: addr:��Ҫ��ȡ�Ĵ����ĵ�ַ
 * Output Parameters	: ��
 * Return Value		: ��ȡ�ļĴ�����ֵ��
 *			
 * ---------------------------------------------------------------------------
 * Description			: ��������������Ƶ���жϴ��
 *			
 *****************************************************************************/
u8 read_reg(u8 addr);


/*****************************************************************************
 * Name			: write_reg
 * Function		: д��ƵоƬ�ļĴ���������������ͨ��������
 * ---------------------------------------------------------------------------
 * Input Parameters	: addr :�Ĵ����ĵ�ַ
 *			  val   :��д��ļĴ�����ֵ
 * Output Parameters	: ��
 * Return Value		: ��
 *
 * ---------------------------------------------------------------------------
 * Description		:
 *****************************************************************************/
void write_reg(u8 addr, u8 val);
void RF_Event_Handle(void);
void Rf_recv_frm(void);
void RF_Timeout_Sub(void);
u8 RF_Read_Sig(void);
u8 Read_RF_Dat_EN(u8 dir);
void RF_check_RevFin(void);
 void RF_Set_Channel(u8 chen);
 void RF_Channel_Update(u8 chen);

void RF_Init(void);
void RF_Listen();
 
#ifdef RF_STATISTICS
void rf_rssi_monitor(void);
//void rf_fdev_monitor(u8 STA2_Val);
u16 get_current_afc();
u16 get_current_rssi(void);
void carrier_output(void);
u8 RF_Config_Power_INC(void);



#endif
#endif

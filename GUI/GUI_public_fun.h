#ifndef __GUI_PUBLIC_FUN__
#define __GUI_PUBLIC_FUN__



#define HARDWARE_VERSION    22
#define SOFTWARE_VERSION    20


#define GUI_645_ADDR_LENGTH 12

/*dropdown�������б��е���������*/
#define DL_T_97    0
#define DL_T_07    1


#define PLC_CMD_TYPE_L2R          0  //read to listening
#define PLC_CMD_TYPE_R2L          1  //listening to read
#define PLC_CMD_TYPE_COMMON       2  //
#define PLC_CMD_TYPE_NODE         3
#define PLC_CMD_BROAD_READ        4  //�㲥�����
#define PLC_CMD_FREQ_SET          5  //��������
#define PLC_CMD_TYPE_ROUTE        6  //����·�ɱ�


#define PLC_ROUTE_OFF             0 
#define PLC_ROUTE_ON              1


#define ICON_FLOW_FLASH_TIMEOUT   60


#define CHANNEL_PLC       0
#define CHANNEL_WIRELESS   1
#define CHANNEL_485        2

#define ONE_STOPBIT        0
#define TWO_STOPBIT        1


#define PLC_READ_DEVADDR_0     0
#define PLC_READ_DATA_1        1
#define PLC_WRITE_DATA_2       2

#define PLC_CTRL_MAX_NUM    16

#define DATA_SIGN_LENGTH    4  //���ݱ�ʶ������



#define GUI_MSBOX_ADDR_ERROR       0
#define GUI_MSBOX_DIDO_ERROR       1
#define GUI_MSBOX_PROC_ERROR       2
#define GUI_MSBOX_MONTH_ERROR      3
#define GUI_MSBOX_DAY_ERROR        4
#define GUI_MSBOX_HOUR_ERROR       5
#define GUI_MSBOX_MIN_ERROR        6
#define GUI_MSBOX_FORMAT_ERROR     7

//#define KEY_PRESS_CNT_MIN          0  //��������
//#define KEY_PRESS_CNT_MAX          8  //��󰴼�����


#define _BUTTON_PRESSED            1
#define _BUTTON_RELEASED           0

#define ON                         0
#define OFF                        1

typedef enum
{
    FHD_GUI_TRM_CAL = 0,
    FHD_GUI_TRM_CONF,
    FHD_GUI_TRM_STATE,
    FHD_GUI_TRM_LOG,
    MAX_FHD_GUI_TYPE
} FHD_GUI_TYPE; //����

typedef enum
{
    FHD_CMD_READ_TRM_VOLTAGE = 0,
    FHD_CMD_CALIBRATE_TRM_VOLTAGE,
    FHD_CMD_READ_TRM_TIME,
    FHD_CMD_CALIBRATE_TRM_TIME,
    FHD_CMD_READ_TRM_CONF,
    FHD_CMD_WRITE_TRM_CONF,
    FHD_CMD_READ_TRM_STATE,
    FHD_CMD_READ_TRM_LOG,
    MAX_FHD_CMD_TYPE
} FHD_CMD_TYPE; //����


//#define ERROR_BOX(error_no)     MESSAGEBOX_Create(&gc_messageBoxText[error_no][0],"Error",0)


typedef struct _sys_parameter_pkg_
{
    //U16 g_protocal;      //��Լ
    U8  cmdType;      
    U8  ctlCode;   //������
    U8  srcAddr[6]; //�����  reserved
    U8  dstAddr[6]; //�����  reserved
    U8  relayAddr[8][6];  //�м̵�ַ,ʵ����7���м̣��ڵ������м�֡�ӿڵ�ʱ����Ҫ��Ŀ���ַҲд��ȥ,8��
    U16 dataLen;        //����
    U8  dataFlag[4];      //���ݱ�ʶ
    U8  dataBuf[256];     //����
    //U8  meterPWD[4] = {0x01,0x31,0x41,0x51};      //���� 
    //U16 g_crc;           //У���
    
}SEND_PARA_PKG,*P_SEND_PARA_PKG;

typedef struct _gui_prm {
    u8 state;
	u8 cmd;
	u8 data_buf[256];
	u16 data_len;
} GUI_PRM, P_GUI_PRM; //����

extern GUI_PRM g_gui_prm;

extern SEND_PARA_PKG g_send_para_pkg;      //������

//extern const u8 c_645ctrlDef[2][PLC_CTRL_MAX_NUM] ;

extern const u32 c_645DidoDef[2][PLC_CTRL_MAX_NUM] ;
extern u8 g_gui_time[MAX_RTC_ITEM];



/* ���� */
void GUI_Trm_Cal_Proc(void);
void GUI_Trm_Conf_Proc(void);
void GUI_Trm_State_Proc(void);
void GUI_Trm_Log_Proc(void);


/*button�԰����ķ�Ӧ����˸һ��*/
void ButtonBlink(WM_MESSAGE * pMsg,int Id);


//�洢����

void GUI_Msg_Upload(u16 sw);
void GUI_Msg_Download(u16 sw);
void GUI_Msg_Proc(void);


//WM_HWIN STM_Get_PROGBAR();
WM_HWIN TSK_Get_Upload_Text();
WM_HWIN TSK_Get_Download_Text();


u8* GUI_hex2PowerDataStr(u8 * srcBuf, u32 len);
u32 GUI_GetMeterAddr(u8 * dbuf, u8 * gbuf);



void Select_Focus(void);



u32 GUI_GetStrDataFlag(u8 * dbuf, u32 pro_ver);
u8* GUI_hex2MeterAddrStr(u8 * srcBuf, u32 len);
void GUI_Fill_Zero(u8 *tempbuf); //�Զ���ȫ��
void GUI_Set_FD_PROGBAR(u32 val);


#endif /*__GUI_PUBLIC_FUN__*/

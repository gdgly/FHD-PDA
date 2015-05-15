#ifndef __APP_STORAGE_H__
#define __APP_STORAGE_H__


#define BOOT_REQUEST_ACT   0xffffbbcc
#define BOOT_FINISH_ACT    0xffff0000

/*
Sector 0 0x0800 0000 - 0x0800 3FFF 16 Kbyte
Sector 1 0x0800 4000 - 0x0800 7FFF 16 Kbyte
Sector 2 0x0800 8000 - 0x0800 BFFF 16 Kbyte
Sector 3 0x0800 C000 - 0x0800 FFFF 16 Kbyte
Sector 4 0x0801 0000 - 0x0801 FFFF 64 Kbyte
Sector 5 0x0802 0000 - 0x0803 FFFF 128 Kbyte
Sector 6 0x0804 0000 - 0x0805 FFFF 128 Kbyte
Sector 7 0x0806 0000 - 0x0807 FFFF 128 Kbyte
*/

#define ROM_ADDR_SYS_PARA    (0x8004000uL)
#define ROM_ADDR_PARA_SIZE   (sizeof(ROM_PRM))

#define RAM_COPY_MAX_SIZE_K    64
#define SRM_PARA_NUMBER      512
#define TAG_WORD_TABLE_MAP     0XDA3C7B92


#define ROM_PARA_TOTAL_SIZE       (16*1024)      //k Bytes;
#define ROM_PARA_ERASE_SECTOR      FLASH_SECTOR_1
#define ROM_PARA_WRITE_TIMES      (ROM_PARA_TOTAL_SIZE/ROM_ADDR_PARA_SIZE)    //(16/2)

#define DEV_TRUE            1
#define DEV_FALSE           0


#define DEV_ERROR            1
#define DEV_OK               0

#define SYS_POWER_INIT          0
#define SYS_POWER_IDLE          1
#define SYS_POWER_WAKEUP        2
#define SYS_POWER_SHUTDOWN      3

#define SYS_TASK_FORMAT_DISK     0X00000001


#define GET_USB_VOL()    HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_13)
#define GET_CHARG_CHK()  HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)

#define SYS_POWER_ON        0
#define SYS_POWER_OFF       1

#define SYS_BEEP_ON         0
#define SYS_BEEP_OFF        1

typedef struct _rom_prm {
unsigned int magic_word;
unsigned int bootFlag;
unsigned int crc;
unsigned int versionDate;

// ------------------------------

unsigned int beep_switch;
unsigned int auto_sleep_time;
unsigned int auto_shutdown_time;

// ------------------------------

unsigned int meterPassword[8];//����
unsigned int recvDelayTime;//����������ʱ
unsigned int execInterval;//ִ��ʱ��
unsigned int scrTimeout;           //��Ļ��ʱ

unsigned int plcProtocol; //��Լ
unsigned int channel; //ͨ��
unsigned int baudrate; //������

unsigned int preamble; //ǰ����
unsigned int stopbit;
unsigned int number;

unsigned int freqSel; //����ѡ��,���������

unsigned int bpsSpeed;             //�������ݵȴ�ʱ��,��Ҫ���棬�Ա��´�������Ĭ������

// ------------------------------

unsigned int para_data[SRM_PARA_NUMBER - 26];
} ROM_PRM, *P_ROM_PRM;

typedef struct _sys_ctrl {
    OS_EVENT *up_mbox; //��������
    OS_EVENT *down_mbox; //��������    
    u32 shutdown_timeout; //�Զ��ػ���ʱʱ��
    u32 sleep_timeout; //˯�߳�ʱʱ��
    u32 sd_total_capacity; //SD��������
    u32 sd_free_capacity; //SD��ʣ������
    u32 sd_file_num; //�ļ���Ŀ 
    u32 usb_state; //USB��λ״̬
    u32 pwr_val; //��ص���
    u8 dev_addr; //�豸��ַ 

    // ---------------------------------
    
    unsigned int paraAddr;
    u32   guiState;
    u32   testProgBarVal;
    u32   procTask;    //��ǰϵͳ��Ҫ���������
    u32   sysPowerState;
    u32   selectWidget;         //���ݴ�ֵѡ��ͬ�Ļص�����
    u32   led_count;          //������һ��LED��һ��
    u8    DevCheckCode[9];      //�Լ�����
} SYS_CTRL, *P_SYS_CTRL;

#define SYS_ADD_TASK(tn)        g_sys_ctrl.procTask|=tn
#define SYS_DEL_TASK(tn)        g_sys_ctrl.procTask&=(~tn)
#define SYS_IS_TASK(tn)         g_sys_ctrl.procTask&tn
 
extern ROM_PRM g_rom_prm;
extern SYS_CTRL g_sys_ctrl;

void DEV_Init(void);

unsigned int DEV_Parameters_Write(void);
void DEV_Parameters_Read(void);
void DEV_Power_Off();
void DEV_SoftReset(void);

void APP_Sleep(void);
void APP_Wakeup();
void APP_Shutdown();

#endif

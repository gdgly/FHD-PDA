#ifndef __APP_DEVICE_H__
#define __APP_DEVICE_H__


#define HARDWARE_VERSION        22
#define SOFTWARE_VERSION        27
#define VERSION_DATE    0x20150709

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
#define ROM_ADDR_PARA_SIZE   (sizeof(ROM_PARA))

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


#define GET_USB_STATE()         HAL_GPIO_ReadPin(GPIOG,GPIO_PIN_13)
#define USB_CRG_CHK()           HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2)

#define GET_KEY_PWR_STATE()     HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11)

#define SYS_POWER_ON        0
#define SYS_POWER_OFF       1

#define SYS_BEEP_ON         0
#define SYS_BEEP_OFF        1

typedef struct _rom_para_ {
unsigned int magic_word;
unsigned int bootFlag;
unsigned int crc;
unsigned int versionDate;

// ------------------------------

unsigned int beep_switch;
unsigned int auto_sleep_time;
unsigned int auto_shutdown_time;

// ------------------------------

unsigned int meterPassword[8];//密码
unsigned int recvDelayTime;//接受数据延时
unsigned int execInterval;//执行时间
unsigned int scrTimeout;           //屏幕超时

unsigned int plcProtocol; //规约
unsigned int channel; //通道
unsigned int baudrate; //波特率

unsigned int preamble; //前导符
unsigned int stopbit;
unsigned int number;

unsigned int freqSel; //速率选择,下面减过了

unsigned int bpsSpeed;             //传输数据等待时间,需要保存，以便下次启动有默认设置

// ------------------------------

unsigned int para_data[SRM_PARA_NUMBER - 26];
} ROM_PARA, *P_ROM_PARA;

typedef struct _sys_ctrl_ {
    OS_EVENT *up_mbox; //上行邮箱
    OS_EVENT *down_mbox; //下行邮箱    
    u32 shutdown_timeout; //自动关机超时时间
    u32 sleep_timeout; //睡眠超时时间
    u32 sd_total_capacity; //SD卡总容量
    u32 sd_free_capacity; //SD卡剩余容量
    u8 sd_format_flag; //SD卡格式化标志
    u32 sd_file_num; //文件数目 
    u32 usb_state; //USB在位状态
    u32 pwr_val; //电池电量
    u8 dev_addr; //设备地址
    u8 new_dev_addr; //新设备地址
    u8 fhd_sw; //防晃电开关
    u8 self_check; //自检

    // ---------------------------------
    
    unsigned int paraAddr;
    u32   guiState;
    u32   testProgBarVal;
    u32   procTask;    //当前系统需要处理的任务
    u32   sysPowerState;
    u32   selectWidget;         //根据此值选择不同的回调函数
    u32   led_count;          //按键按一次LED亮一次
    u8    DevCheckCode[9];      //自检密码
} SYS_CTRL, *P_SYS_CTRL;

#define SYS_ADD_TASK(tn)        g_sys_ctrl.procTask|=tn
#define SYS_DEL_TASK(tn)        g_sys_ctrl.procTask&=(~tn)
#define SYS_IS_TASK(tn)         g_sys_ctrl.procTask&tn
 
extern ROM_PARA g_rom_para;
extern SYS_CTRL g_sys_ctrl;

void DEV_Init(void);
void dev_para_recover(void);

unsigned int DEV_Parameters_Write(void);
void DEV_Parameters_Read(void);
void DEV_Power_Off();
void DEV_SoftReset(void);

void APP_Sleep(void);
void APP_Wakeup();
void APP_Shutdown();


#endif

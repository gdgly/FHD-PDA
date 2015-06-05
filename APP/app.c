/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3220G-EVAL
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <includes.h>
#include  "gui.h"
#include "DIALOG.h"
#include "lcdconf.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
#define VECT_TAB_OFFSET  0x8000 


                                                                /* ----------------- APPLICATION GLOBALS ------------------ */
static  OS_STK         App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK         App_TaskEndTickStk[APP_CFG_TASK_END_TICK_STK_SIZE];
static  OS_STK         App_TaskEndProcStk[APP_CFG_TASK_END_PROC_STK_SIZE];
static  OS_STK         App_TaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE]; //added on  01.15
static  OS_STK         App_TaskKeyStk[APP_CFG_TASK_KEY_STK_SIZE];
static  OS_STK         App_TaskProtoStk[APP_CFG_TASK_PROTO_STK_SIZE];
static  OS_STK         App_TaskGMPStk[APP_CFG_TASK_GMP_STK_SIZE];
static  OS_STK         App_TaskPowerStk[APP_CFG_TASK_POWER_STK_SIZE];
static  OS_STK         App_TaskPCStk[APP_CFG_TASK_PC_STK_SIZE];
static  OS_STK         App_TaskRS485Stk[APP_CFG_TASK_RS485_STK_SIZE];
static  OS_STK         App_TaskCheckStk[APP_CFG_TASK_CHECK_STK_SIZE];
static  OS_STK         App_TaskFHDPStk[APP_CFG_TASK_FHDP_STK_SIZE];
static  OS_STK         App_TaskWDTStk[APP_CFG_TASK_WDT_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void           App_TaskStart                (void *p_arg);
static  void           App_TaskGUI                  (void *p_arg); //GUI任务 added on 01.15
static  void           App_TaskGMP                  (void *p_arg);
static  void           App_TaskPower                (void *p_arg);
static  void           App_TaskCheck                (void *p_arg);
static  void           App_TaskWDT                  (void *p_arg);

static  void           App_MemAlloc                 (void);
static  void           App_TaskCreate               (void);
static  void           App_EventCreate              (void);


static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Configure the Vector Table location add offset address ------------------*/

  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */


  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main(void)
{
#if (OS_TASK_NAME_EN > 0)
    INT8U  err;
#endif   


    BSP_IntDisAll();                                            /* Disable all interrupts.                              */

    SystemClock_Config();
    
    CPU_Init();                                                 /* Initialize uC/CPU services.                          */
    
    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"          */

    OSTaskCreateExt((void (*)(void *)) App_TaskStart,           /* Create the start task                                */
                    (void           *) 0,
                    (OS_STK         *)&App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_START_PRIO,
                    (INT16U          ) APP_CFG_TASK_START_PRIO,
                    (OS_STK         *)&App_TaskStartStk[0],
                    (INT32U          ) APP_CFG_TASK_START_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_START_PRIO, "Start", &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

WM_HWIN g_hWin_menu;


WM_HWIN g_hWin_task;//任务栏
WM_HWIN g_hWin_TimeSet; //时间设置
WM_HWIN g_hWin_about;
WM_HWIN g_hWin_Err;

WM_HWIN g_hWin_TimeBar;  //主页时间
WM_HWIN g_hWin_Date;     //显示日期
WM_HWIN g_hWin_Input;    //各种输入小框体
WM_HWIN g_hWin_SysSet;
WM_HWIN g_hWin_SysInfo;

/**********

防晃电

**********/

WM_HWIN g_hWin_TrmCal;
WM_HWIN g_hWin_TrmConf;
WM_HWIN g_hWin_TrmState;
WM_HWIN g_hWin_TrmLog;

//int test_multiedit;

u8 rf_int_status[8];
u8 rf_part_info[8];
u8 rf_device_state[2];
extern u8 g_get_pro[4];

//int G_II;

//uc8 g_testString[] = "Hello \xe7\xa1\xae! \n";

//uc16 g_16string [] = {0x53D6, 0x56DE, 0x62,'\r\n',0};

void APP_Shutdown()
{
#if 1
    if(g_sys_ctrl.usb_state)
    {
        return;
    }

    g_sys_ctrl.sysPowerState = SYS_POWER_SHUTDOWN;
    SYS_PWR_OFF();   //3.27
    LCD_BL_OFF();
    LCD_PWR_OFF();   //3.27
    LED_KEY_OFF();
    LED_SLEEP_OFF();
#endif
}

void APP_Sleep(void)
{
#if 1
    g_sys_ctrl.sysPowerState = SYS_POWER_IDLE;
    LCD_BL_OFF();
    LED_SLEEP_ON();
#endif    
}

void APP_Wakeup()
{
#if 0
    g_sys_ctrl.sysPowerState = SYS_POWER_WAKEUP;
    //SYS_PWR_OFF();
    //LCD_BL_ON();
    //LCD_PWR_ON();

    LCD_X_DisplayDriver(0, LCD_X_INITCONTROLLER, 0);
    //LED_KEY_OFF();
    //LED_SLEEP_OFF();
#endif    
}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void APP_StartButtonTest()
{
    u32 keyCnt = 0;

    
    while(1)
    {
        OSTimeDly(20);
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
        {
            keyCnt++;
            if(keyCnt > 80)
            {
                SYS_PWR_ON();
                OSTimeDly(20);
                return;                
            }
        }
        else
        {
            APP_Shutdown();
            keyCnt = 0;

            if(GET_USB_VOL() == 0)
            {
                return;
            }
            else
            {
                DEBUG_PRINT(("Power error!\n")); //错误处理
                
                while(1)
                {
                    APP_Shutdown();
                }
            }
        }
    }
}

/*
*********************************************************************************************************
*                                             App_TaskStart()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskStart (void *p_arg)
{
    INT8U n;


    (void)p_arg; 
        
    DEV_Init();

    BSP_Init();                                                 /* Init BSP fncts.                                          */

    Mem_Init();                                                 /* Initialize Memory managment                              */

    BSP_CPU_TickInit();                                         /* Start Tick Initialization                                */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif

#if 1
    APP_StartButtonTest();
#endif

    App_EventCreate();                                          /* Create Application Events                                */

    App_TaskCreate();/* Create application Tasks                                 */ 

    /* 挂起不相关的任务 */
    OSTaskSuspend(APP_CFG_TASK_PROTO_PRIO);

    BSP_IWDG_Init();  //4.1

    GUI_Init();                                                 /* Init the STemWin GUI Library */

	KEY_Init();

    End_Init();

    RF_Init();
    
    RTC_ReadTime(g_rtc_time); 
    RTC_CheckTime(g_rtc_time);   
    
    DEBUG_PRINT(("OS Tasks Run!\n"));

    while (DEF_TRUE) { 
        for(n = 0; n < 32; n++)
        {
            if((1 << n) & g_sys_ctrl.procTask)
            {
                switch(n)
                {
                case 0:  
                    g_sys_ctrl.sd_format_flag = TRUE;
                    
                    FM_Format_Disk();
                    
                    g_sys_ctrl.sd_format_flag = FALSE;
                    
                    SYS_DEL_TASK(SYS_TASK_FORMAT_DISK);
                    break;
                }
            }
        }

        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}

/*
*********************************************************************************************************
*                                             App_TaskGUI()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskGUI()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskGUI (void *p_arg)
{ 
    WM_HWIN hItem;
    unsigned char timebuf[16];
    int n = 0;
    u32 val = 0;
    INT32U count = 0;


    (void)p_arg;

    g_hWin_menu = CreatePDA_IconMenu();
    WM_ShowWindow(g_hWin_menu);
    WM_SetFocus(g_hWin_menu);

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */
        GUI_Exec();
        
        if(n < 10)
        {
            val += BSP_ADC_ReadPwr();
            //检测到USB并且电没有充满的时候，充电标志闪烁,
            if((GPIO_PIN_RESET == GET_USB_VOL())&&(GPIO_PIN_SET == GET_CHARG_CHK()))
            {
                TSK_Battery_Charge(n);
            }
            n++;
        }
        else if(n >= 10)
        {
            g_sys_ctrl.pwr_val = val/10;
            if((GPIO_PIN_SET == GET_USB_VOL())||(GPIO_PIN_RESET == GET_CHARG_CHK()))
            {
                Battery_State(g_sys_ctrl.pwr_val);
            }
            n = 0;
            val = 0;
            
            if(g_hWin_SysInfo > 0)
            {
                SDI_SetVoltage();
            }
           
            if((((float)g_sys_ctrl.pwr_val * 3.3) / 2048 * 10) <= 30)
            {
                //ERR_NOTE(g_hWin_menu,11);
                //APP_Shutdown();
            }
        }

        if(!(count % 5))
        {
            RTC_ReadTime(g_rtc_time); 
            
            RTC2Text(timebuf);
            TEXT_SetText(TSK_Get_Time(), timebuf);

            RTC2Text(timebuf);
            TEXT_SetText(g_hWin_TimeBar, TextDay[g_rtc_time[DAY_POS] % 7]);
            
            RTC2Text_Date(timebuf);
            TEXT_SetText(g_hWin_Date, timebuf);            
        }

        if(!(count % 30))
        {
            SetBeepState();
        }
        
        if((!(count % 150)) && (FALSE == g_sys_ctrl.sd_format_flag))
        {
            hItem = TSK_GetSD();
            
            if(TRUE == fdisk_detect())
            {
                TEXT_SetText(hItem, SD_Mount);
            }
            else
            {
                TEXT_SetText(hItem, " ");
            }
        }
        
        count++;
        
        OSTimeDlyHMSM(0, 0, 0, 100);     
    }
}

/*
*********************************************************************************************************
*                                             App_TaskPower()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskPower()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskPower (void *p_arg)
{
    (void)p_arg;

    while(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
    {
        OSTimeDlyHMSM(0, 0, 0, 10);
    }
        
    while (DEF_TRUE) {
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
        {
            OSTimeDlyHMSM(0, 0, 0, 30);

            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
            {
                while(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
                {
                    OSTimeDlyHMSM(0, 0, 0, 10);
                }
                
                LCD_BL_OFF();
                LED_SLEEP_ON();
            }
        }

        if(GPIO_PIN_RESET == GET_USB_VOL())
        {
            g_sys_ctrl.usb_state = TRUE;
        }
        else
        {
            g_sys_ctrl.usb_state = FALSE;
        }        

        g_sys_ctrl.shutdown_timeout++;
        if(g_sys_ctrl.shutdown_timeout > (g_rom_para.auto_shutdown_time * 100))
        {
            g_sys_ctrl.shutdown_timeout = 0;
            
            APP_Shutdown();
        }
        
        g_sys_ctrl.sleep_timeout++;
        if(g_sys_ctrl.sleep_timeout > (g_rom_para.auto_sleep_time * 100))
        {
            g_sys_ctrl.sleep_timeout = 0;
            
            APP_Sleep();
        }                

        OSTimeDlyHMSM(0, 0, 0, 10);
    }
}

#if 0
/*
*********************************************************************************************************
*                                             App_TaskGMP()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskGMP()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskGMP (void *p_arg)
{
    INT8U err;

    u32   tm;

    WM_HWIN  wh;
    
    (void)p_arg;
    //g_sys_ctrl.numMultiedit = 0; //控制消息多行文本的打印次数
    while (DEF_TRUE) {  
        /* Task body, always written as an infinite loop.           */
        if((g_sys_ctrl.guiState == GUI_PLC_MSG_TEST)
            ||(g_sys_ctrl.guiState == GUI_PLC_MSG_READ)
            ||(g_sys_ctrl.guiState == GUI_PLC_MSG_LISTING)
            ||(g_sys_ctrl.guiState == GUI_PLC_MSG_FREQ))
        {
            OSMboxPend(g_sys_ctrl.down_mbox, 10, &err);        
            if(OS_ERR_NONE == err)
            {
                //g_sys_ctrl.numMultiedit++;
                GUI_Msg_Proc();
            }
            else if(OS_ERR_TIMEOUT == err)
            {
                wh = GUI_Get_PROGBAR();
                if(wh != WM_HWIN_NULL)
                {
                    
                    PROGBAR_SetValue(wh, g_sys_ctrl.testProgBarVal);
                    if(g_sys_ctrl.testProgBarVal < 90)
                    {
                       PROGBAR_SetBarColor(wh, 0, GUI_GREEN);
                       g_sys_ctrl.testProgBarVal += 1;
                    }
                }
            }
        }
        else
        {
            OSTimeDly(10);
        }
    }
}
#else //华兄
/*
*********************************************************************************************************
*                                             App_TaskGMP()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskGMP()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskGMP (void *p_arg)
{
    INT8U err;

    
    (void)p_arg;
    
    while (DEF_TRUE) {
        OSMboxPend(g_sys_ctrl.down_mbox, 0, &err);

        if(OS_ERR_NONE == err)
        {
            GUI_Msg_Proc();
        }
        else
        {

        }
    }
}
#endif

/*
*********************************************************************************************************
*                                             App_TaskWDT()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskWDT()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskWDT (void *p_arg)
{
    (void)p_arg;

    while (DEF_TRUE) {
        clr_wdt();
        
        OSTimeDlyHMSM(0, 0, 0, 500);
    }
}

/*
*********************************************************************************************************
*                                             App_TaskCheck()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskCheck()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/

#define CHECK_INFO_OK_POS       (28 * 8)
#define CHECK_INFO_ERROR_POS    (25 * 8)

typedef enum
{
    START_CHECK_INFO = 0,
    CHECK_INFO_INC = 0, 
    CHECK_INFO_COPYRIGHT,
    CHECK_INFO_NULL,
    CHECK_INFO_VERSION,
    CHECK_INFO_CPU,
    CHECK_INFO_MEM,
    CHECK_INFO_LCD,
    CHECK_INFO_SD,
    CHECK_INFO_FATFS,
    CHECK_INFO_KEY,
    CHECK_INFO_POWER,
    MAX_CHECK_INFO,
} CHECK_INFO_TYPE;

typedef struct _check_info_pos
{
    int x;
    int y;
    int x1;
    int x2;
} CHECK_INFO_POS, *P_CHECK_INFO_POS;

const char *g_check_info[] = 
{
    "HRK Technology Inc.           ",
    "Copyright (C) 2011-2015       ",
    "                              ",
    "HARDWARE:     , SOFTWARE:     ",
    "CPU: STM32F207ZET6, 120MHz    ",
    "FLASH: 1MB, SRAM: 132KB       ",
    "1. LCD Type:                  ",
    "2. SD Card:                   ",
    "3. FATFS Check                ",
    "Waiting for the key:          ",
    "System Shutting Down ... (30s)"
};

CHECK_INFO_POS g_check_info_pos[] = 
{
    {30,  0 * 16, 0, 0}, //INC
    {30,  1 * 16, 199, 0}, //COPYRIGHT
    { 0,  2 * 16, 0, 0}, //NULL
    { 0,  3 * 16, 80, 208}, //VERSION 
    { 0,  4 * 16, 0, 0}, //CPU
    { 0,  5 * 16, 0, 0}, //MEM
    { 0,  6 * 16, 104, 0}, //LCD  
    { 0,  7 * 16, 96, 0}, //SD
    { 0,  8 * 16, 0, 0}, //FATFS   
    { 0,  9 * 16, 192, 0}, //KEY 
    { 0, 10 * 16, 208, 0}, //POWER
};

const int key_chk_map[KEYBOARD_COL_NUM * KEYBOARD_ROW_NUM] = 
{
    GUI_KEY_GREEN, GUI_KEY_YELLOW, GUI_KEY_ESCAPE, GUI_KEY_TAB, GUI_KEY_UP, GUI_KEY_DOWN,
    GUI_KEY_LEFT, GUI_KEY_RIGHT, GUI_KEY_ENTER, GUI_KEY_BACKSPACE, '1', '2',
    '3', '4', '5', '6', '7', '8',
    '9', '*', '0', '#', GUI_KEY_F1, GUI_KEY_F2,
};

const char *key_chk_msg[KEYBOARD_COL_NUM * KEYBOARD_ROW_NUM + 2] = 
{
    "GREEN ", "YELLOW", "ESC   ", "TAB   ", "UP    ", "DOWN  ", 
    "LEFT  ", "RIGHT ", "OK    ", "C     ", "1     ", "2     ",
    "3     ", "4     ", "5     ", "6     ", "7     ", "8     ",
    "9     ", "*     ", "0     ", "#     ", "F1    ", "F2    ",
    "PWR   ", "      ",
};

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
static  void  App_TaskCheck (void *p_arg)
{
    INT8U i, err, count, buf[32];
    int *p_key_msg;
    

    g_sys_ctrl.self_check = FALSE;
    OSSemPend(g_sem_check, 0, &err);
    g_sys_ctrl.self_check = TRUE;
    
    (void)p_arg;

    count = 30;

    /* 挂起不相关的任务 */
    OSTaskSuspend(APP_CFG_TASK_START_PRIO);
    OSTaskSuspend(APP_CFG_TASK_GUI_PRIO);
    OSTaskSuspend(APP_CFG_TASK_GMP_PRIO);
    OSTaskSuspend(APP_CFG_TASK_PROTO_PRIO);
    OSTaskSuspend(APP_CFG_TASK_POWER_PRIO);
    OSTaskSuspend(APP_CFG_TASK_PC_PRIO);
    OSTaskSuspend(APP_CFG_TASK_RS485_PRIO);

    /* 初始化显示环境 */
    WM_HideWin(g_hWin_task);
    WM_HideWin(g_hWin_SysSet);
    WM_SetFocus(WM_HBKWIN);
    GUI_SetFont(&GUI_Font_Song_16);
    GUI_SetBkColor(GUI_BLACK);
    GUI_SetColor(GUI_WHITE);
    GUI_Clear();    

    /* 显示版权信息 */
    GUI_DispStringAt(g_check_info[CHECK_INFO_INC], g_check_info_pos[CHECK_INFO_INC].x, g_check_info_pos[CHECK_INFO_INC].y);

    GUI_DispStringAt(g_check_info[CHECK_INFO_COPYRIGHT], g_check_info_pos[CHECK_INFO_COPYRIGHT].x, g_check_info_pos[CHECK_INFO_COPYRIGHT].y);
    sprintf(buf, "%02x", g_rtc_time[YEAR_POS]);
    GUI_DispStringAt(buf, g_check_info_pos[CHECK_INFO_COPYRIGHT].x1, g_check_info_pos[CHECK_INFO_COPYRIGHT].y);

    GUI_DispStringAt(g_check_info[CHECK_INFO_NULL], g_check_info_pos[CHECK_INFO_NULL].x, g_check_info_pos[CHECK_INFO_NULL].y);

    /* 显示版本信息 */
    GUI_DispStringAt(g_check_info[CHECK_INFO_VERSION], g_check_info_pos[CHECK_INFO_VERSION].x, g_check_info_pos[CHECK_INFO_VERSION].y);
    sprintf(buf, "v%d.%d", HARDWARE_VERSION / 10, HARDWARE_VERSION % 10);
    GUI_DispStringAt(buf, g_check_info_pos[CHECK_INFO_VERSION].x1, g_check_info_pos[CHECK_INFO_VERSION].y);
    sprintf(buf, "v%d.%d", SOFTWARE_VERSION / 10, SOFTWARE_VERSION % 10);
    GUI_DispStringAt(buf, g_check_info_pos[CHECK_INFO_VERSION].x2, g_check_info_pos[CHECK_INFO_VERSION].y);    

    /* 显示CPU信息 */
    GUI_DispStringAt(g_check_info[CHECK_INFO_CPU], g_check_info_pos[CHECK_INFO_CPU].x, g_check_info_pos[CHECK_INFO_CPU].y);   
    GUI_DispStringAt(g_check_info[CHECK_INFO_MEM], g_check_info_pos[CHECK_INFO_MEM].x, g_check_info_pos[CHECK_INFO_MEM].y);

    /* 检测LCD */
    GUI_DispStringAt(g_check_info[CHECK_INFO_LCD], g_check_info_pos[CHECK_INFO_LCD].x, g_check_info_pos[CHECK_INFO_LCD].y);
    sprintf(buf, "ILI%x", LCD_TYPE);
    GUI_DispStringAt(buf, g_check_info_pos[CHECK_INFO_LCD].x1, g_check_info_pos[CHECK_INFO_LCD].y);
    sprintf(buf, "OK");
    GUI_DispStringAt(buf, CHECK_INFO_OK_POS, g_check_info_pos[CHECK_INFO_LCD].y);  

    /* 检测SD卡和文件系统 */
    if(get_sd_info())
    {
        GUI_DispStringAt(g_check_info[CHECK_INFO_SD], g_check_info_pos[CHECK_INFO_SD].x, g_check_info_pos[CHECK_INFO_SD].y);
        sprintf(buf, "%dMB", g_sys_ctrl.sd_total_capacity / 1024);
        GUI_DispStringAt(buf, g_check_info_pos[CHECK_INFO_SD].x1, g_check_info_pos[CHECK_INFO_SD].y);
        sprintf(buf, "OK");
        GUI_DispStringAt(buf, CHECK_INFO_OK_POS, g_check_info_pos[CHECK_INFO_SD].y);       
        
        GUI_DispStringAt(g_check_info[CHECK_INFO_FATFS], g_check_info_pos[CHECK_INFO_FATFS].x, g_check_info_pos[CHECK_INFO_FATFS].y);
        sprintf(buf, "OK");
        GUI_DispStringAt(buf, CHECK_INFO_OK_POS, g_check_info_pos[CHECK_INFO_FATFS].y);
    }
    else
    {
        GUI_DispStringAt(g_check_info[CHECK_INFO_SD], g_check_info_pos[CHECK_INFO_SD].x, g_check_info_pos[CHECK_INFO_SD].y);
        sprintf(buf, "%dMB", g_sys_ctrl.sd_total_capacity / 1024);
        GUI_DispStringAt(buf, g_check_info_pos[CHECK_INFO_SD].x1, g_check_info_pos[CHECK_INFO_SD].y);
        sprintf(buf, "ERROR");
        GUI_DispStringAt(buf, CHECK_INFO_ERROR_POS, g_check_info_pos[CHECK_INFO_SD].y);

        GUI_DispStringAt(g_check_info[CHECK_INFO_FATFS], g_check_info_pos[CHECK_INFO_FATFS].x, g_check_info_pos[CHECK_INFO_FATFS].y);
        sprintf(buf, "ERROR");
        GUI_DispStringAt(buf, CHECK_INFO_ERROR_POS, g_check_info_pos[CHECK_INFO_FATFS].y);        
    }

    /* 检测按键 */
    GUI_DispStringAt(g_check_info[CHECK_INFO_KEY], g_check_info_pos[CHECK_INFO_KEY].x, g_check_info_pos[CHECK_INFO_KEY].y);

    for(i = 0; i < (KEYBOARD_COL_NUM * KEYBOARD_ROW_NUM); i++)
    { 
        GUI_DispStringAtCEOL(key_chk_msg[i], g_check_info_pos[CHECK_INFO_KEY].x1, g_check_info_pos[CHECK_INFO_KEY].y);

        OSMboxAccept(g_mbox_chk_key);

        do {
            p_key_msg = (int *)OSMboxPend(g_mbox_chk_key, 0, &err);
        } while((OS_ERR_NONE != err) || (key_chk_map[i] != *p_key_msg));
    }
    
    GUI_DispStringAtCEOL(key_chk_msg[i], g_check_info_pos[CHECK_INFO_KEY].x1, g_check_info_pos[CHECK_INFO_KEY].y);

    while(1) //PWR
    {
        OSTimeDlyHMSM(0, 0, 0, 10);
        
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
        {
            OSTimeDlyHMSM(0, 0, 0, 30);

            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
            {
                break;
            }
        }
    }
    
    i++;
    
    GUI_DispStringAtCEOL(key_chk_msg[i], g_check_info_pos[CHECK_INFO_KEY].x1, g_check_info_pos[CHECK_INFO_KEY].y);
    
    /* 关机 */
    GUI_DispStringAt(g_check_info[CHECK_INFO_POWER], g_check_info_pos[CHECK_INFO_POWER].x, g_check_info_pos[CHECK_INFO_POWER].y);

    while (DEF_TRUE) {
        sprintf(buf, "%02d", count);
        GUI_DispStringAt(buf, g_check_info_pos[CHECK_INFO_POWER].x1, g_check_info_pos[CHECK_INFO_POWER].y);

        if(count)
        {
            count--;
        }
        else
        {    
            SYS_PWR_OFF();

            OSTimeDlyHMSM(0, 0, 1, 0);

            DEV_SoftReset();       
        }
        
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

/*
*********************************************************************************************************
*                                      App_EventCreate()
*
* Description : Create the application Events
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TasStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_EventCreate (void)
{    
    g_sem_end = OSSemCreate(0);
    g_sem_plc = OSSemCreate(0);
    g_sem_rf = OSSemCreate(0);
    g_sem_pc = OSSemCreate(0);
    g_sem_rs485 = OSSemCreate(0);
    g_sem_fhdp = OSSemCreate(0);
	g_sem_check = OSSemCreate(0);
    g_sem_chk_plc = OSSemCreate(0);
    g_sem_chk_rf = OSSemCreate(0);
	g_key_ctrl.key_sem = OSSemCreate(0);    
    
    g_sys_ctrl.up_mbox = OSMboxCreate(NULL); /*创建消息邮箱用来发送调试参数的结构体*/
    g_sys_ctrl.down_mbox = OSMboxCreate(NULL); /*创建消息邮箱用来发送调试参数的结构体*/   

    g_mbox_chk_key = OSMboxCreate(NULL);
}

/*
*********************************************************************************************************
*                                      App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TasStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_TaskCreate (void)
{
    INT8U  err;

    
    OSTaskCreateExt((void (*)(void *)) App_TaskGUI,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_GUI_PRIO,
                    (INT16U          ) APP_CFG_TASK_GUI_PRIO,
                    (OS_STK         *)&App_TaskGUIStk[0],
                    (INT32U          ) APP_CFG_TASK_GUI_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_GUI_PRIO, "GUI", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskGMP,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskGMPStk[APP_CFG_TASK_GMP_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_GMP_PRIO,
                    (INT16U          ) APP_CFG_TASK_GMP_PRIO,
                    (OS_STK         *)&App_TaskGMPStk[0],
                    (INT32U          ) APP_CFG_TASK_GMP_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    
#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_GMP_PRIO, "GMP", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskKey,              
                    (void           *) 0,
                    (OS_STK         *)&App_TaskKeyStk[APP_CFG_TASK_KEY_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_KEY_PRIO,
                    (INT16U          ) APP_CFG_TASK_KEY_PRIO,
                    (OS_STK         *)&App_TaskKeyStk[0],
                    (INT32U          ) APP_CFG_TASK_KEY_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_KEY_PRIO, "Key", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskEndTick,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskEndTickStk[APP_CFG_TASK_END_TICK_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_END_TICK_PRIO,
                    (INT16U          ) APP_CFG_TASK_END_TICK_PRIO,
                    (OS_STK         *)&App_TaskEndTickStk[0],
                    (INT32U          ) APP_CFG_TASK_END_TICK_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_END_TICK_PRIO, "EndTick", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskEndProc,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskEndProcStk[APP_CFG_TASK_END_PROC_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_END_PROC_PRIO,
                    (INT16U          ) APP_CFG_TASK_END_PROC_PRIO,
                    (OS_STK         *)&App_TaskEndProcStk[0],
                    (INT32U          ) APP_CFG_TASK_END_PROC_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_END_PROC_PRIO, "EndProc", &err);      
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskProto,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskProtoStk[APP_CFG_TASK_PROTO_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_PROTO_PRIO,
                    (INT16U          ) APP_CFG_TASK_PROTO_PRIO,
                    (OS_STK         *)&App_TaskProtoStk[0],
                    (INT32U          ) APP_CFG_TASK_PROTO_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_PROTO_PRIO, "Protocol", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskPower,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskPowerStk[APP_CFG_TASK_POWER_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_POWER_PRIO,
                    (INT16U          ) APP_CFG_TASK_POWER_PRIO,
                    (OS_STK         *)&App_TaskPowerStk[0],
                    (INT32U          ) APP_CFG_TASK_POWER_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_POWER_PRIO, "Power", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskPC,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskPCStk[APP_CFG_TASK_PC_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_PC_PRIO,
                    (INT16U          ) APP_CFG_TASK_PC_PRIO,
                    (OS_STK         *)&App_TaskPCStk[0],
                    (INT32U          ) APP_CFG_TASK_PC_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_PC_PRIO, "PC", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskRS485,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskRS485Stk[APP_CFG_TASK_RS485_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_RS485_PRIO,
                    (INT16U          ) APP_CFG_TASK_RS485_PRIO,
                    (OS_STK         *)&App_TaskRS485Stk[0],
                    (INT32U          ) APP_CFG_TASK_RS485_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_RS485_PRIO, "RS485", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskCheck,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskCheckStk[APP_CFG_TASK_CHECK_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_CHECK_PRIO,
                    (INT16U          ) APP_CFG_TASK_CHECK_PRIO,
                    (OS_STK         *)&App_TaskCheckStk[0],
                    (INT32U          ) APP_CFG_TASK_CHECK_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_CHECK_PRIO, "Check", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskWDT,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskWDTStk[APP_CFG_TASK_WDT_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_WDT_PRIO,
                    (INT16U          ) APP_CFG_TASK_WDT_PRIO,
                    (OS_STK         *)&App_TaskWDTStk[0],
                    (INT32U          ) APP_CFG_TASK_WDT_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_WDT_PRIO, "WDT", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskFHDP,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskFHDPStk[APP_CFG_TASK_FHDP_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_FHDP_PRIO,
                    (INT16U          ) APP_CFG_TASK_FHDP_PRIO,
                    (OS_STK         *)&App_TaskFHDPStk[0],
                    (INT32U          ) APP_CFG_TASK_FHDP_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_FHDP_PRIO, "FHDP", &err);    
#endif
}


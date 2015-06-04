/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.20                          *
*        Compiled Mar 19 2013, 15:01:00                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "includes.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0  (GUI_ID_USER + 0x00)
#define ID_TEXT_0    (GUI_ID_USER + 0x01)
#define ID_TEXT_1    (GUI_ID_USER + 0x02)
#define ID_TEXT_2    (GUI_ID_USER + 0x03)
#define ID_TEXT_3    (GUI_ID_USER + 0x04)
#define ID_TEXT_4    (GUI_ID_USER + 0x05)
#define ID_TEXT_5    (GUI_ID_USER + 0x06)
#define ID_TEXT_6    (GUI_ID_USER + 0x07)


#define ID_EDIT_0    (GUI_ID_USER + 0x08)
#define ID_EDIT_1    (GUI_ID_USER + 0x09)
#define ID_EDIT_2    (GUI_ID_USER + 0x0A)
#define ID_EDIT_3    (GUI_ID_USER + 0x0B)
#define ID_EDIT_4    (GUI_ID_USER + 0x0C)
#define ID_EDIT_5    (GUI_ID_USER + 0x0D)
#define ID_EDIT_6    (GUI_ID_USER + 0x0E)
#define ID_EDIT_7    (GUI_ID_USER + 0x0F)


#define ID_BUTTON_1  (GUI_ID_USER + 0x10)
#define ID_BUTTON_2  (GUI_ID_USER + 0x11)
//#define ID_BUTTON_3  (GUI_ID_USER + 0x0F)
#define ID_BUTTON_4  (GUI_ID_USER + 0x12)

#define ID_PROGBAR_0 (GUI_ID_USER + 0x13)
#define ID_PROGBAR_1 (GUI_ID_USER + 0x14)



// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect,   "MemManage",  ID_WINDOW_0,  0,   0,   240, 295,0, 0x0,  0 },
  { TEXT_CreateIndirect,     HardVersion,  ID_TEXT_3,    12,  16,  80,  20, 0, 0x0,  0 },
  { TEXT_CreateIndirect,     SoftVersion,  ID_TEXT_4,    12,  47,  80,  20, 0, 0x0,  0 },
  { TEXT_CreateIndirect,     Voltage,      ID_TEXT_5,    12,  79,  80,  20, 0, 0x0,  0 },
  { TEXT_CreateIndirect,     MemSize,      ID_TEXT_0,    12,  107,  80,  20, 0, 0x0,  0 },
  { TEXT_CreateIndirect,     MemUsage,     ID_TEXT_1,    12,  169,  80,  20, 0, 0x0,  0 },
  { TEXT_CreateIndirect,     FileNum,      ID_TEXT_2,    12,  138,  80,  20, 0, 0x0,  0 },
    
  { EDIT_CreateIndirect,     "memsize",    ID_EDIT_0,    140, 106,  95,  20, EDIT_CF_HCENTER, 0x64, 0 },
  { EDIT_CreateIndirect,     "filenum",    ID_EDIT_1,    140, 137,  95,  20, EDIT_CF_HCENTER, 0x64, 0 },
  { EDIT_CreateIndirect,     "hard",       ID_EDIT_2,    140, 12, 95,  20, EDIT_CF_HCENTER, 0x64,  0 },
  { EDIT_CreateIndirect,     "soft",       ID_EDIT_3,    140, 43, 95,  20, EDIT_CF_HCENTER, 0x64,  0 },
  { EDIT_CreateIndirect,     "vtg",        ID_EDIT_4,    140, 75, 95,  20, 0, 0x64,  0 },
    
  { PROGBAR_CreateIndirect,  "Progbar",    ID_PROGBAR_0, 140, 169, 95, 20, 0, 0x0,  0 },
  { BUTTON_CreateIndirect,   MemFormat,    ID_BUTTON_1,  10,  260, 65,  25, 0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Quit,         ID_BUTTON_2,  165, 260, 65,  25, 0, 0x0,  0 },
  { PROGBAR_CreateIndirect,  "Progbar",    ID_PROGBAR_1, 9,   228, 222, 20, 0, 0x0,  0 },
    
  // USER START (Optionally insert additional widgets)
  // USER END
};


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
WM_HWIN MMD_Get_PROGBAR()
{    
     return WM_GetDialogItem(g_hWin_SysInfo, ID_PROGBAR_0);            
}

void GUI_Set_FD_PROGBAR(u32 val)
{
    WM_HWIN hItem;

    
    hItem = WM_GetDialogItem(g_hWin_SysInfo, ID_PROGBAR_1);  
    
    if(WM_HWIN_NULL != hItem)
    {
        PROGBAR_SetValue(hItem, val);              
    }
}


void SDI_SetVoltage(void)
{
    WM_HWIN hItem;


    hItem = WM_GetDialogItem(g_hWin_SysInfo, ID_EDIT_4);
    EDIT_SetFloatValue(hItem, ((float)g_sys_ctrl.pwr_val * 3.3) / 2048);
}


// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) 
{
      WM_HWIN hItem;
      int     NCode;
      int     Id;
      u8    sbuf[64];

      switch (pMsg->MsgId) 
      {
          case WM_INIT_DIALOG:
            get_sd_info();
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
            sprintf(sbuf, "%dMB", (g_sys_ctrl.sd_total_capacity/1024));
            EDIT_SetText(hItem, sbuf);
            
            //WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
            WM_DisableWindow(hItem);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
            g_sys_ctrl.sd_file_num = get_file_num();
            sprintf(sbuf, "%d", g_sys_ctrl.sd_file_num);
            EDIT_SetText(hItem, sbuf);
            WM_DisableWindow(hItem);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0);
            PROGBAR_SetBarColor(hItem, 0, GUI_RED);
            //PROGBAR_SetBarColor(hItem, 1, GUI_GREEN);
  
            NCode = (100*g_sys_ctrl.sd_free_capacity)/g_sys_ctrl.sd_total_capacity;
            NCode %= 100;
            NCode = 100-NCode;
            PROGBAR_SetValue(hItem, NCode);

            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
            sprintf(sbuf, "v%d.%d", HARDWARE_VERSION / 10, HARDWARE_VERSION % 10);
            EDIT_SetText(hItem, sbuf);
            WM_DisableWindow(hItem);
            WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
  
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
            sprintf(sbuf, "v%d.%d", SOFTWARE_VERSION / 10, SOFTWARE_VERSION % 10);
            EDIT_SetText(hItem, sbuf);
            WM_DisableWindow(hItem);
            WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
  
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
            EDIT_SetFloatMode(hItem, ((float)g_sys_ctrl.pwr_val * 3.3) / 2048, 0, 99999, 2, GUI_EDIT_SUPPRESS_LEADING_ZEROES);
            WM_DisableWindow(hItem);
            WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
            
            //sprintf(sbuf, "%dMbytes", (g_sys_control.sd_total_capacity/1024));
            hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_1);
            BUTTON_SetBkColor(hItem,0,GUI_GREEN);
            WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
            
            hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_2);
            BUTTON_SetBkColor(hItem,0,GUI_YELLOW);
            WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
            //hItem = WM_GetDialogItem(pMsg->hWin,ID_PROGBAR_0);
            
            hItem = WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_1); 
            PROGBAR_SetBarColor(hItem, 0, GUI_GREEN);
            //PROGBAR_SetValue(hItem, 100);
            break;
          case WM_KEY:
            
            if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt)==1)
            {
                switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
                {
                    case GUI_KEY_GREEN:
                        //WM_DeleteWindow(g_hWin_SysInfo);
                        //g_hWin_SysInfo = WM_HWIN_NULL;
                        //WM_ShowWindow(g_hWin_TimeBar);
                        //WM_ShowWindow(g_hWin_Date);
                        //WM_SetFocus(g_hWin_SysSet);
                        ERR_NOTE(g_hWin_SysInfo,WARN_FORMAT);
                        break;
                    case GUI_KEY_YELLOW:
                        WM_DeleteWindow(g_hWin_SysInfo);
                        g_hWin_SysInfo = WM_HWIN_NULL;
                        //WM_ShowWindow(g_hWin_TimeBar);
                        //WM_ShowWindow(g_hWin_Date);
                        WM_SetFocus(g_hWin_SysSet);
                        break;
                    case GUI_KEY_F1:
                        break;
                }
            }   
                
            break;
          default:
            WM_DefaultProc(pMsg);
            break;
      }
}

/*********************************************************************
*
*       Public code
*SDInfo
**********************************************************************
*/
/*********************************************************************
*
*       CreateMemManage
*/
WM_HWIN CreateSDInfo(void);
WM_HWIN CreateSDInfo(void) {
  WM_HWIN hWin;
  //WM_HWIN hTimer;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_SysSet, 0, 0);
  //hTimer = WM_CreateTimer(g_hWin_mem, 0, 2000, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/

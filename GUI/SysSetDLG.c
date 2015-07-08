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
#define ID_WINDOW_0 (GUI_ID_USER + 0x00)
#define ID_TEXT_0 (GUI_ID_USER + 0x01)
#define ID_TEXT_1 (GUI_ID_USER + 0x02)
#define ID_TEXT_2 (GUI_ID_USER + 0x03)
#define ID_TEXT_3 (GUI_ID_USER + 0x04)
#define ID_TEXT_4 (GUI_ID_USER + 0x05)
#define ID_EDIT_0 (GUI_ID_USER + 0x08)
#define ID_EDIT_1 (GUI_ID_USER + 0x09)
#define ID_EDIT_2   (GUI_ID_USER + 0x0A)
#define ID_EDIT_3   (GUI_ID_USER + 0x0B)

#define ID_BUTTON_0 (GUI_ID_USER + 0x0C)
#define ID_BUTTON_1 (GUI_ID_USER + 0x0D)
#define ID_BUTTON_2 (GUI_ID_USER + 0x0E)
#define ID_BUTTON_3 (GUI_ID_USER + 0x0F)
#define ID_BUTTON_4 (GUI_ID_USER + 0x10)

#define ID_TEXT_5   (GUI_ID_USER + 0x11)
#define ID_TEXT_6   (GUI_ID_USER + 0x12)


#define ID_TEXT_7   (GUI_ID_USER + 0x13)
#define ID_EDIT_4   (GUI_ID_USER + 0x14)

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "SysSet",    ID_WINDOW_0, 0, 0, 240, 295, 0, 0x0, 0 },
  { TEXT_CreateIndirect, SrcTime,     ID_TEXT_0,   15,  14,  110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, ShutTime,    ID_TEXT_1,   15,  44,  120, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, MemState,    ID_TEXT_2,   15,  104,  110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, BeepSound,   ID_TEXT_3,   15,  134, 110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, Promotion,   ID_TEXT_4,   15,  164, 110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, CommAddr,    ID_TEXT_5,   15,  74, 120, 20, 0, 0x0, 0 },
    
  { EDIT_CreateIndirect, "Edit",      ID_EDIT_0,   150, 10,  80, 20, EDIT_CF_HCENTER, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit",      ID_EDIT_1,   150, 40,  80, 20, EDIT_CF_HCENTER, 0x64, 0 },
  { EDIT_CreateIndirect, "Edit",      ID_EDIT_2,   150, 70, 80, 20, EDIT_CF_HCENTER, 0x64, 0 },
    
  { BUTTON_CreateIndirect, "F1",      ID_BUTTON_0, 150, 100,  80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "F2",      ID_BUTTON_1, 150, 130, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "*",       ID_BUTTON_2, 150, 160, 80, 20, 0, 0x0, 0 },


  //{ TEXT_CreateIndirect, HardVersion, ID_TEXT_6, 15, 191, 110, 20, 0, 0x0, 0 },
  //{ EDIT_CreateIndirect, "Edit",      ID_EDIT_3, 140, 191, 80, 20, EDIT_CF_HCENTER, 0x64, 0 },
  //{ TEXT_CreateIndirect, BtrVoltage, ID_TEXT_7, 15, 221, 110, 20, 0, 0x0, 0 },
  //{ EDIT_CreateIndirect, "Edit",      ID_EDIT_4, 140, 221, 80, 20, 0, 0x64, 0 },

  
  { BUTTON_CreateIndirect, TimeSet, ID_BUTTON_3, 10,   262, 70, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, Quit,      ID_BUTTON_4, 160,  262, 70, 25, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END


    
static int SST_KeyCnt = 0;

void SST_SelectUp(void)
{
    WM_HWIN hItem;
    if(SST_KeyCnt == 0)
    {
        hItem=WM_GetDialogItem(g_hWin_SysSet, ID_EDIT_2);
        WM_SetFocus(hItem);
        SST_KeyCnt = (SST_EDT_NUMBER - 1);
    }
    else
    {
        SST_KeyCnt--;
        hItem=WM_GetDialogItem(g_hWin_SysSet,(ID_EDIT_0 + SST_KeyCnt));
        WM_SetFocus(hItem);
    }
}

void SST_SelectDown(void)
{
    WM_HWIN hItem;
    if(SST_KeyCnt == (SST_EDT_NUMBER - 1))
    {
        hItem=WM_GetDialogItem(g_hWin_SysSet,ID_EDIT_0);
        WM_SetFocus(hItem);
        SST_KeyCnt = 0;
    }
    else
    {
        SST_KeyCnt++;
        hItem=WM_GetDialogItem(g_hWin_SysSet,(ID_EDIT_0 + SST_KeyCnt));
        WM_SetFocus(hItem);
    }
}

void SST_ColorChange(void)
{
    WM_HWIN hItem;
    int i;
    for(i = 0;i < SST_EDT_NUMBER;i++)
    {
        hItem =  WM_GetDialogItem(g_hWin_SysSet, (ID_EDIT_0 + i));
        if( 1 == WM_HasFocus(hItem))
        {
            EDIT_SetBkColor(hItem, 0, GUI_GREEN);
        }
        else if(0 == WM_HasFocus(hItem))
        {
            EDIT_SetBkColor(hItem, 0, 0xC0C0C0);
        }
    }
}

void SST_FocusSel(void)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(g_hWin_SysSet, (ID_EDIT_0 + SST_KeyCnt));
    WM_SetFocus(hItem);
}

WM_HWIN SST_GetSrcTime(void)
{
    return WM_GetDialogItem(g_hWin_SysSet,ID_EDIT_0);
}

WM_HWIN SST_GetShutDown(void)
{
    return WM_GetDialogItem(g_hWin_SysSet,ID_EDIT_1);
}

WM_HWIN SST_GetCommAddr(void)
{
    return WM_GetDialogItem(g_hWin_SysSet,ID_EDIT_2);
}

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  u8      tmpBuf[10];
  switch (pMsg->MsgId) 
  {
      case WM_INIT_DIALOG:
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
          sprintf(tmpBuf, "%d", g_rom_para.auto_sleep_time);
          EDIT_SetText(hItem, tmpBuf);
          WM_DisableWindow(hItem);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
          sprintf(tmpBuf, "%d", g_rom_para.auto_shutdown_time);
          EDIT_SetText(hItem, tmpBuf);
          WM_DisableWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
          sprintf(tmpBuf, "%d", g_sys_ctrl.dev_addr);
          EDIT_SetText(hItem, tmpBuf);
          WM_DisableWindow(hItem);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
          BUTTON_SetBkColor(hItem, 0, GUI_CYAN);
          WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
          BUTTON_SetBkColor(hItem, 0, GUI_CYAN);
          WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
          BUTTON_SetBkColor(hItem, 0, GUI_CYAN);
          WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
          
          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
          BUTTON_SetBkColor(hItem, 0, GUI_GREEN);
          WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

          hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
          BUTTON_SetBkColor(hItem, 0, GUI_YELLOW);
          WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

          break;
      case WM_KEY:
          Id    = WM_GetId(pMsg->hWinSrc);
          if( strlen(g_sys_ctrl.DevCheckCode) == 8)
          {
              g_sys_ctrl.DevCheckCode[8] = '\0';
              if(0 == strcmp(g_sys_ctrl.DevCheckCode, g_self_check_pwd))
              {
                  OSSemPost(g_sem_check);  
              }
              memset(g_sys_ctrl.DevCheckCode,0,sizeof(g_sys_ctrl.DevCheckCode));
          }
          if(_BUTTON_PRESSED == ((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)
          {
              switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key) 
              {
                case '*':
                    WARN(g_hWin_SysSet,WARN_UPDATE);
                    break;
                case GUI_KEY_YELLOW:
                    memset(g_sys_ctrl.DevCheckCode,0,sizeof(g_sys_ctrl.DevCheckCode));
                    WM_DeleteWindow(g_hWin_SysSet);
                    g_hWin_SysSet = WM_HWIN_NULL;
                    WM_ShowWindow(g_hWin_TimeBar);
                    WM_ShowWindow(g_hWin_Date);
                    WM_SetFocus(g_hWin_Menu);
                    SST_KeyCnt = 0;
                    break;
                    
                case GUI_KEY_F1:
                    g_hWin_SysInfo = CreateSDInfo();
                    WM_SetFocus(g_hWin_SysInfo);  
                    break;
                    
                case GUI_KEY_F2:
                    if(SYS_BEEP_ON == g_rom_para.beep_switch)
                    {
                         g_rom_para.beep_switch = SYS_BEEP_OFF;
                    }
                    else 
                    {
                         g_rom_para.beep_switch = SYS_BEEP_ON;
                    }
                    
                    DEV_Parameters_Write();

                    SetBeepState();
                    break;
                    
                case GUI_KEY_GREEN:
                    g_hWin_TimeSet = CreateTimeSet();
                    WM_SetFocus(g_hWin_TimeSet);
                    TMS_SetFocus();
                    TMS_Color_Change();
                    break;
 
                case GUI_KEY_UP:
                    SST_SelectUp();
                    SST_ColorChange();
                    break;
                case GUI_KEY_DOWN:
                    SST_SelectDown();
                    SST_ColorChange();
                    break;
                case GUI_KEY_ENTER:
                    switch(Id)
                    {
                        case ID_EDIT_0:
                            g_sys_ctrl.selectWidget = EDT_SRC_TIME;
                            g_hWin_Edit = Create_Edit_Set(g_hWin_SysSet);
                            break;
                        case ID_EDIT_1:
                            g_sys_ctrl.selectWidget = EDT_SHUTDOWN;
                            g_hWin_Edit = Create_Edit_Set(g_hWin_SysSet);
                            break;
                        case ID_EDIT_2:
                            g_sys_ctrl.selectWidget = EDT_COMMUNICATE_ADDR;
                            g_hWin_Edit = Create_Edit_Set(g_hWin_SysSet);
                            break;
                    }
                    break;
                case '0':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '0';
                    break;
                case '1':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '1';
                    break;
                case '2':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '2';
                    break;
                case '3':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '3';
                    break;
                case '4':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '4';
                    break;
                case '5':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '5';
                    break;
                case '6':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '6';
                    break;
                case '7':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '7';
                    break;
                case '8':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '8';
                    break;
                case '9':
                    g_sys_ctrl.DevCheckCode[strlen(g_sys_ctrl.DevCheckCode)] = '9';
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
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateSysSet
*/
WM_HWIN CreateSysSet(void);
WM_HWIN CreateSysSet(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_Menu, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)
// USER END

/*************************** End of file ****************************/

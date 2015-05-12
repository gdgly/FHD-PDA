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
#define ID_LISTVIEW_0 (GUI_ID_USER + 0x01)
#define ID_TEXT_0 (GUI_ID_USER + 0x02)
#define ID_BUTTON_0 (GUI_ID_USER + 0x03)
#define ID_BUTTON_1 (GUI_ID_USER + 0x04)
#define ID_BUTTON_2 (GUI_ID_USER + 0x05)

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
  { WINDOW_CreateIndirect, "SysLog",     ID_WINDOW_0,   0,   0,   240, 295, 0, 0x0, 0 },
  { TEXT_CreateIndirect,   AntiLog,  ID_TEXT_0, 8, 9, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, ReadData,    ID_BUTTON_0, 8, 265, 70, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, Quit,     ID_BUTTON_1, 160, 265, 70, 25, 0, 0x0, 0 },
  //{ BUTTON_CreateIndirect, ReadData, ID_BUTTON_2, 160, 233, 70, 25, 0, 0x0, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 8, 35, 225, 214, 0, 0x0, 0 },
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

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int     RowNum;
  int     i;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
    LISTVIEW_AddColumn(hItem, 150, OccurTime, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 70, Period, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 0);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetBkColor(hItem, 0, GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetBkColor(hItem, 0, GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    // USER START (Optionally insert additional code for further widget initialization)
    // USER END
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetBkColor(hItem, 0, GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    break;
  case WM_KEY:
    if( _BUTTON_PRESSED == ((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)
    {
        switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key) 
        {
            case GUI_KEY_YELLOW:
                WM_DeleteWindow(g_hWin_TrmLog);
                g_hWin_TrmLog = HBWIN_NULL;
                WM_SetFocus(g_hWin_menu);
                WM_ShowWindow(g_hWin_TimeBar);
                WM_ShowWindow(g_hWin_Date);
                break;
                         
            case GUI_KEY_GREEN:    
                g_gui_prm.state = FHD_GUI_TRM_LOG;
                g_gui_prm.cmd = FHD_CMD_READ_TRM_LOG;
                OSMboxPost(g_sys_ctrl.down_mbox, &g_gui_prm);                 
                break;
#if 0
            case GUI_KEY_F2:
                hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
                RowNum = LISTVIEW_GetNumRows(hItem);
                
                for(i = 0; i < RowNum; i++)
                {
                    LISTVIEW_DeleteRow(hItem, i);
                }
                break;
#endif
        }
    }
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {}
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
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
*       CreateSysLog
*/
WM_HWIN CreateTrmLog(void);
WM_HWIN CreateTrmLog(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_menu, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)

void GUI_Sys_Log_Proc(void)
{
    WM_HWIN hItem;
    u8 i, row, buf[128];
    

    switch(g_gui_prm.cmd)
    {
    case FHD_CMD_READ_TRM_LOG:
        u8 *pdata;

#define POWER_DROP_TIMESTAMP_COL       0
#define POWER_DROP_KEEP_TIME_COL       1

        pdata = (u8 *)g_fhd_prm.data_buf;

        row = *((u16 *)pdata);
        pdata += 2;

        for(i = 0; i < row; i++)
        {
            sprintf(buf, "%02x-%02x-%02x %02x:%02x:%02x",
                    *((u8 *)(pdata + 6)),
                    *((u8 *)(pdata + 5)),
                    *((u8 *)(pdata + 4)),
                    *((u8 *)(pdata + 2)),
                    *((u8 *)(pdata + 1)),
                    *((u8 *)(pdata + 0)));
            
            pdata += 7;
            
            hItem = WM_GetDialogItem(g_hWin_TrmLog, ID_LISTVIEW_0);
            LISTVIEW_SetItemText(hItem, POWER_DROP_TIMESTAMP_COL, i, buf);
            
            sprintf(buf, "%d", mb_swap(*((u16 *)pdata)));
            pdata += 2;
            
            hItem = WM_GetDialogItem(g_hWin_TrmLog, ID_LISTVIEW_0);
            LISTVIEW_SetItemText(hItem, POWER_DROP_KEEP_TIME_COL, i, buf);

            LISTVIEW_AddRow(hItem, NULL);
        }       
        break;

    default:
        break;
    }   
}

// USER END

/*************************** End of file ****************************/

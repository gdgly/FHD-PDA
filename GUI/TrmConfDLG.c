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
#define ID_EDIT_0 (GUI_ID_USER + 0x01)
#define ID_EDIT_1 (GUI_ID_USER + 0x02)
#define ID_EDIT_2 (GUI_ID_USER + 0x03)
#define ID_TEXT_0 (GUI_ID_USER + 0x0D)
#define ID_TEXT_1 (GUI_ID_USER + 0x0E)
#define ID_TEXT_2 (GUI_ID_USER + 0x0F)
#define ID_BUTTON_0 (GUI_ID_USER + 0x19)
#define ID_BUTTON_1 (GUI_ID_USER + 0x1A)
#define ID_BUTTON_2 (GUI_ID_USER + 0x1B)
#define ID_TEXT_3   (GUI_ID_USER + 0x1C)
#define ID_TEXT_4   (GUI_ID_USER + 0x1D)
#define ID_BUTTON_3  (GUI_ID_USER + 0x1E)
#define ID_BUTTON_4  (GUI_ID_USER + 0x1F)
    
    
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
  { WINDOW_CreateIndirect, "ParaRdWt",       ID_WINDOW_0, 0, 1, 240, 295, 0, 0x0, 0 },
  { EDIT_CreateIndirect,   "edit", ID_EDIT_0, 155, 10, 80, 20, EDIT_CF_HCENTER, 0x64, 0 },
  { EDIT_CreateIndirect,   "edit",          ID_EDIT_1, 155, 40, 80, 20, EDIT_CF_HCENTER, 0x64, 0 },
  { EDIT_CreateIndirect, "addr",             ID_EDIT_2, 155, 70, 80, 20, EDIT_CF_HCENTER, 0x64, 0 },
  { TEXT_CreateIndirect, InterElecPrt,          ID_TEXT_0, 7, 10, 122, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, ProtectTime,    ID_TEXT_1,   5,   40,  128, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, DevAddr,          ID_TEXT_2, 5, 71, 120, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, SetPara,         ID_BUTTON_0, 8,   262, 70, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, Quit,         ID_BUTTON_1, 160, 262, 70, 25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "F1",     ID_BUTTON_2, 155, 100, 80, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect, SysReset,        ID_TEXT_3,   5, 101,  110, 20, 0, 0x0, 0 },
  //{ BUTTON_CreateIndirect, "F1",         ID_BUTTON_3, 155,   101, 80, 20, 0, 0x0, 0 },
  //{ TEXT_CreateIndirect, SetPara,        ID_TEXT_4,   5,   131,  80, 20, 0, 0x0, 0 },
  //{ BUTTON_CreateIndirect, "F2",         ID_BUTTON_4, 155,   131, 80, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};


/*********************************************************************
*
*       Static code
*
**********************************************************************
*/
    
static int PRW_KeyCnt = 0;

void PRW_SelectUp(void)
{
    WM_HWIN hItem;
    if(PRW_KeyCnt == 0)
    {
        hItem=WM_GetDialogItem(g_hWin_TrmConf, ID_EDIT_2);
        WM_SetFocus(hItem);
        PRW_KeyCnt = (PRW_EDT_NUMBER - 1);
    }
    else
    {
        PRW_KeyCnt--;
        hItem=WM_GetDialogItem(g_hWin_TrmConf,(ID_EDIT_0 + PRW_KeyCnt));
        WM_SetFocus(hItem);
    }
}

void PRW_SelectDown(void)
{
    WM_HWIN hItem;
    if(PRW_KeyCnt == (PRW_EDT_NUMBER - 1))
    {
        hItem=WM_GetDialogItem(g_hWin_TrmConf,ID_EDIT_0);
        WM_SetFocus(hItem);
        PRW_KeyCnt = 0;
    }
    else
    {
        PRW_KeyCnt++;
        hItem=WM_GetDialogItem(g_hWin_TrmConf,(ID_EDIT_0 + PRW_KeyCnt));
        WM_SetFocus(hItem);
    }
}

void PRW_ColorChange(void)
{
    WM_HWIN hItem;
    int i;
    for(i = 0;i < PRW_EDT_NUMBER;i++)
    {
        hItem =  WM_GetDialogItem(g_hWin_TrmConf, (ID_EDIT_0 + i));
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

void PRW_FocusSel(void)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(g_hWin_TrmConf, (ID_EDIT_0 + PRW_KeyCnt));
    WM_SetFocus(hItem);
}
    



// USER START (Optionally insert additional static code)
static void _init_ParaDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetText(hItem, " ");
    WM_DisableWindow(hItem);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
    EDIT_SetText(hItem, " ");
    WM_DisableWindow(hItem);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
    EDIT_SetText(hItem, " ");
    WM_DisableWindow(hItem);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetBkColor(hItem, 0, GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetBkColor(hItem, 0, GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetBkColor(hItem, 0, GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

#if 0
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetBkColor(hItem, 0, GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_4);
    BUTTON_SetBkColor(hItem, 0, GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
#endif    
}
// USER END
//晃电保护开关
WM_HWIN PRW_GetElecPrtSwth(void)
{
    return WM_GetDialogItem(g_hWin_TrmConf,ID_EDIT_0);
}

//晃电保护时长
WM_HWIN PRW_GetPrtTime(void)
{
    return WM_GetDialogItem(g_hWin_TrmConf,ID_EDIT_1);
}

//设备地址
WM_HWIN PRW_GetDevAddr(void)
{
    return WM_GetDialogItem(g_hWin_TrmConf,ID_EDIT_2);
}








/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
      _init_ParaDialog(pMsg);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {}
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  case WM_KEY:
    Id    = WM_GetId(pMsg->hWinSrc);
    if(_BUTTON_PRESSED == ((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)
    {
        switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key) 
        {
            case GUI_KEY_YELLOW:
                 WM_DeleteWindow(g_hWin_TrmConf);
                 g_hWin_TrmConf = WM_HWIN_NULL;
                 WM_SetFocus(g_hWin_menu);
                 WM_ShowWindow(g_hWin_TimeBar);
                 WM_ShowWindow(g_hWin_Date);
                 PRW_KeyCnt = 0;
                break;
#if 0
            case GUI_KEY_GREEN:
                WM_DeleteWindow(g_hWin_ParaRdWt);
                g_hWin_ParaRdWt=WM_HWIN_NULL;
                WM_SetFocus(g_hWin_menu);
                WM_ShowWindow(g_hWin_TimeBar);
                WM_ShowWindow(g_hWin_Date);
                PRW_KeyCnt = 0;
                break;
#endif
            case GUI_KEY_UP:
                PRW_SelectUp();
                PRW_ColorChange();
                break;
            case GUI_KEY_DOWN:
                PRW_SelectDown();
                PRW_ColorChange();
                break;
#if 0
            case GUI_KEY_GREEN:
                g_gui_para.state = FHD_GUI_TRM_CONF;
                g_gui_para.cmd = FHD_CMD_READ_TRM_CONF;
                OSMboxPost(g_sys_ctrl.up_mbox, &g_gui_para);                              
                break;
#endif  

            case GUI_KEY_GREEN:
                u8 buf[16], index;
                u16 temp;
                
                
                index = 0;
                
                if(ON == g_sys_ctrl.fhd_sw)
                {
                    g_gui_para.data_buf[index] = 1;
                }
                else
                {
                    g_gui_para.data_buf[index] = 0;
                }
                
                index++;

                hItem = WM_GetDialogItem(g_hWin_TrmConf, ID_EDIT_1);
                EDIT_GetText(hItem, buf, 16);
                temp = atoi(buf);
                memcpy(&g_gui_para.data_buf[index], (u8 *)&temp, 2);
                index += 2;

                hItem = WM_GetDialogItem(g_hWin_TrmConf, ID_EDIT_2);
                EDIT_GetText(hItem, buf, 16);
                temp = atoi(buf);
                memcpy(&g_gui_para.data_buf[index], (u8 *)&temp, 2);
                index += 2;
                
                g_gui_para.state = FHD_GUI_TRM_CONF;
                g_gui_para.cmd = FHD_CMD_WRITE_TRM_CONF;
                OSMboxPost(g_sys_ctrl.up_mbox, &g_gui_para);    
                break;

            case GUI_KEY_F1:
                ERR_NOTE(g_hWin_TrmConf, WARN_CONFIRM_RESET);
                break;
                
            case '*':
                break;
            case '#':
                break;
            case GUI_KEY_ENTER:
                switch(Id)
                {
                    case ID_EDIT_0:
                        g_sys_ctrl.selectWidget = LST_ANIP_SWITCH;
                        g_hWin_Input = Create_ListBox_Set(g_hWin_TrmConf);
                        break;
                    case ID_EDIT_1:
                        //g_sys_ctrl.selectWidget = EDT_ANIP_TIME;
                        g_hWin_Input = Create_EditPrt(g_hWin_TrmConf);
                        hItem = WM_GetDialogItem(g_hWin_Input, ID_EDIT_0);
                        WM_SetFocus(hItem);
                        break;
                    case ID_EDIT_2:
                        g_sys_ctrl.selectWidget = EDT_DEV_ADDR;
                        g_hWin_Input = Create_Edit_Set(g_hWin_TrmConf);
                        break;
                }
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
*       CreateParaRdWt
*/
WM_HWIN CreateParaConf(void);
WM_HWIN CreateParaConf(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_menu, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)

void GUI_Trm_Conf_Proc(void)
{
    WM_HWIN hItem;
    u8 buf[128];
    

    switch(g_gui_para.cmd)
    {
    case FHD_CMD_READ_TRM_CONF:
        u8 *pdata;
        u32 tmp1, tmp2;
        
        
        pdata = (u8 *)g_fhdp_para.data_buf;

        tmp1 = mb_swap_32((u8 *)pdata);
        pdata += 4;

        tmp2 = (tmp1 >> 0) & 0x00000001; //晃电保护开关

        if(tmp2)
        {
            sprintf(buf, SwitchOn);

            g_sys_ctrl.fhd_sw = ON;
        }
        else
        {
            sprintf(buf, SwitchOff);

            g_sys_ctrl.fhd_sw = OFF;
        }
        
        if(g_hWin_TrmState > 0)
        {
            hItem = SSD_GetPrtSwitch();
        }
        
        if(g_hWin_TrmConf > 0)
        {
            hItem = WM_GetDialogItem(g_hWin_TrmConf, ID_EDIT_0);
        }
        
        EDIT_SetText(hItem, buf); 
        
        tmp2 = (tmp1 >> 1) & 0x00000001;
        tmp2 = (tmp1 >> 2) & 0x00000001;

        tmp1 = mb_swap(*((u16 *)pdata));
        pdata += 2;

        sprintf(buf, "%d", tmp1);
        
        if(g_hWin_TrmState > 0)
        {
            hItem = SSD_GetPrtTime();
        }
        
        if(g_hWin_TrmConf > 0)
        {
            hItem = WM_GetDialogItem(g_hWin_TrmConf, ID_EDIT_1);
        }
        
        EDIT_SetText(hItem, buf);

        tmp1 = mb_swap(*((u16 *)pdata));
        pdata += 2;

        tmp1 = mb_swap(*((u16 *)pdata));
        pdata += 2;        

        tmp1 = mb_swap(*((u16 *)pdata));
        pdata += 2;

        tmp1 = mb_swap(*((u16 *)pdata));
        pdata += 2;        

        sprintf(buf, "%d", tmp1);
        
        if(g_hWin_TrmState > 0)
        {
            hItem = SSD_GetDevAddr();
        }
        
        if(g_hWin_TrmConf > 0)
        {
            hItem = WM_GetDialogItem(g_hWin_TrmConf, ID_EDIT_2);
        }
        
        EDIT_SetText(hItem, buf);
        break;
        
    case FHD_CMD_WRITE_TRM_CONF:
        g_sys_ctrl.dev_addr = g_sys_ctrl.new_dev_addr;
        break;
        
    default:
        break;
    }
}

// USER END

/*************************** End of file ****************************/

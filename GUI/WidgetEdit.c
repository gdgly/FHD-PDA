#include "DIALOG.h"
#include "includes.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x00)
#define ID_LISTBOX_0   (GUI_ID_USER + 0x02)
#define ID_BUTTON_0    (GUI_ID_USER + 0x03)
#define ID_BUTTON_1    (GUI_ID_USER + 0x04)
#define ID_TEXT_0      (GUI_ID_USER + 0x05)
#define ID_EDIT_0      (GUI_ID_USER + 0x06)



static const GUI_WIDGET_CREATE_INFO _aListBoxCreate[] = {
  { FRAMEWIN_CreateIndirect,  "ListBox",  ID_FRAMEWIN_0,  20,  40,  200, 200,  0, 0x0, 0 },
  { LISTBOX_CreateIndirect,   "Listbox",  ID_LISTBOX_0,   5,   7,   182, 120,  0, 0x0, 0 },
  { BUTTON_CreateIndirect,    TextOK,     ID_BUTTON_0,    5,   140, 55,  25,   0, 0x0, 0 },
  { BUTTON_CreateIndirect,    Quit,       ID_BUTTON_1,    138, 140, 55,  25,   0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aEditCreate[] = {
  { FRAMEWIN_CreateIndirect, "Edit",  ID_FRAMEWIN_0, 20,  60, 200, 160, 0, 0x0,  0 },
  { BUTTON_CreateIndirect,   TextOK,  ID_BUTTON_0,   5,   82, 55,  25,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Quit,    ID_BUTTON_1,   138, 82, 55,  25,  0, 0x0,  0 },
  { EDIT_CreateIndirect,     "Edit",  ID_EDIT_0,     14,  38, 165, 25,  0, 0x64, 0 },
};

void Select_Focus(void)
{
    if(g_hWin_TrmCal > 0)
    {
        WM_SetFocus(g_hWin_TrmCal);
    }

    if(g_hWin_TrmConf > 0)
    {
        WM_SetFocus(g_hWin_TrmConf);
        PRW_FocusSel();
    }

    if(g_hWin_TrmState > 0)
    {
        WM_SetFocus(g_hWin_TrmState);
        //SSD_FocusSel();
    }
    
    if((g_hWin_SysSet > 0)&&(g_hWin_SDInfo > 0)&&(g_hWin_TimeSet <= 0))
    {
        WM_SetFocus(g_hWin_SDInfo);
    }
    
    if((g_hWin_SysSet > 0)&&(g_hWin_SDInfo <= 0)&&(g_hWin_TimeSet > 0))
    {
        WM_SetFocus(g_hWin_TimeSet);
        TMS_SetFocus();
    }

    if((g_hWin_SysSet > 0)&&(g_hWin_SDInfo <= 0)&&(g_hWin_TimeSet <= 0))
    {
        WM_SetFocus(g_hWin_SysSet);
        SST_FocusSel();
    }

    if(g_hWin_TrmLog > 0)
    {
        WM_SetFocus(g_hWin_TrmLog);
    }
}

#if 1
/***********************************************
*
*  把编辑的edit中的数据传递到相应的页面
*  editnum是分辨传进来的edit是哪个
*
************************************************/

static void SelectInputEdit(int  EditNum)
{
    WM_HWIN hItem;
    u8 tmpTextBuf[32];
    u8 tmpListBuf[32];
    u32 tmp;
    
    hItem=WM_GetDialogItem(g_hWin_Input,ID_EDIT_0);
    switch(EditNum)
    {
        case EDT_VOLTAGE_REFER:
            EDIT_GetText(hItem, tmpTextBuf,7);
            hItem = CPS_GetReferVtg();
            break;
        case EDT_ANIP_TIME:
            EDIT_GetText(hItem, tmpTextBuf,5);
            tmp = atoi(tmpTextBuf);
            if(tmp > 2000)
            {
                //ERR_NOTE(g_hWin_Input)
            }
            hItem = PRW_GetPrtTime();
            break;
        case EDT_DEV_ADDR:
            EDIT_GetText(hItem, tmpTextBuf,5);
            g_sys_ctrl.new_dev_addr = atoi(tmpTextBuf);
            hItem = PRW_GetDevAddr();
            break;
        case EDT_YEAR:
            EDIT_GetText(hItem, tmpTextBuf, 3);
            g_gui_time[YEAR_POS] = atoi(tmpTextBuf);
            hItem = TMS_GetYear();
            break;
        case EDT_MONTH:
            EDIT_GetText(hItem, tmpTextBuf, 3);
            g_gui_time[MONTH_POS] = atoi(tmpTextBuf);
            hItem = TMS_GetMonth();
            break;
        case EDT_DAY:
            EDIT_GetText(hItem, tmpTextBuf, 3);
            g_gui_time[DATE_POS] = atoi(tmpTextBuf);
            hItem = TMS_GetDay();
            break;
        case EDT_HOUR:
            EDIT_GetText(hItem, tmpTextBuf, 3);
            g_gui_time[HOUR_POS] = atoi(tmpTextBuf);
            hItem = TMS_GetHour();
            break;
        case EDT_MINITE:
            EDIT_GetText(hItem, tmpTextBuf, 3);
            g_gui_time[MIN_POS] = atoi(tmpTextBuf);
            hItem = TMS_GetMin();
            break;
        case EDT_SECOND:
            EDIT_GetText(hItem, tmpTextBuf, 3);
            g_gui_time[SEC_POS] = atoi(tmpTextBuf);
            hItem = TMS_GetSec();
            break;
        case EDT_SRC_TIME:
            EDIT_GetText(hItem, tmpTextBuf,8);
            g_rom_para.auto_sleep_time = atoi(tmpTextBuf);
            if(g_rom_para.auto_sleep_time < 5)
            {
                g_rom_para.auto_sleep_time = 5;

                sprintf(tmpTextBuf, "%d", g_rom_para.auto_sleep_time);
            }
            DEV_Parameters_Write();
            hItem = SST_GetSrcTime();
            break;
        case EDT_SHUTDOWN:
            EDIT_GetText(hItem, tmpTextBuf,8);
            g_rom_para.auto_shutdown_time = atoi(tmpTextBuf);
            if(g_rom_para.auto_shutdown_time < 60)
            {
                g_rom_para.auto_shutdown_time = 60;

                sprintf(tmpTextBuf, "%d", g_rom_para.auto_shutdown_time);
            }
            DEV_Parameters_Write();
            hItem = SST_GetShutDown();
            break;
        default:
            break;
    }
    
    EDIT_SetText(hItem,tmpTextBuf);
}


static void _init_edit(WM_MESSAGE *pMsg,int EditNum)
{

    WM_HWIN hItem;
    u8 tmpTextBuf[32];
    u8 addr_len;
    u8 tmpZero[13];
    int RowSel;
    int i;
    //float tmpFloatVal;
    switch(EditNum)
    {
        case EDT_VOLTAGE_REFER:
            hItem = CPS_GetReferVtg();
            EDIT_GetText(hItem, tmpTextBuf,4);
            break;
        case EDT_ANIP_TIME:
            hItem = PRW_GetPrtTime();
            EDIT_GetText(hItem, tmpTextBuf,5);
            break;
        case EDT_DEV_ADDR:
            hItem = PRW_GetDevAddr();
            EDIT_GetText(hItem, tmpTextBuf,5);
            break;
        case EDT_YEAR:
            hItem = TMS_GetYear();
            EDIT_GetText(hItem, tmpTextBuf, 3);
            break;
        case EDT_MONTH:
            hItem = TMS_GetMonth();
            EDIT_GetText(hItem, tmpTextBuf, 3);
            break;
        case EDT_DAY:
            hItem = TMS_GetDay();
            EDIT_GetText(hItem, tmpTextBuf, 3);
            break;
        case EDT_HOUR:
            hItem = TMS_GetHour();
            EDIT_GetText(hItem, tmpTextBuf, 3);
            break;
        case EDT_MINITE:
            hItem = TMS_GetMin();
            EDIT_GetText(hItem, tmpTextBuf, 3);
            break;
        case EDT_SECOND:
            hItem = TMS_GetSec();
            EDIT_GetText(hItem, tmpTextBuf, 3);
            break;
        case EDT_SRC_TIME:
            hItem = SST_GetSrcTime();
            EDIT_GetText(hItem, tmpTextBuf,4);
            break;
        case EDT_SHUTDOWN:
            hItem = SST_GetShutDown();
            EDIT_GetText(hItem, tmpTextBuf,4);
            break;
        default:
            break;
     }
     FRAMEWIN_SetText(pMsg->hWin,EditFrameTitle[EditNum]);
     hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_0);
     EDIT_SetText(hItem,tmpTextBuf);

        
}

#endif

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/


void Button_Color_Set(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    
    hItem = WM_GetDialogItem(pMsg->hWin,ID_BUTTON_1);
    BUTTON_SetBkColor(hItem,0,GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
}



static void _cbEditDlg(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
  
    switch (pMsg->MsgId) 
    {
      case WM_INIT_DIALOG:
        FRAMEWIN_SetTitleHeight(pMsg->hWin,25);
        Button_Color_Set(pMsg);
        _init_edit(pMsg,g_sys_ctrl.selectWidget);
        //hItem = WM_GetDialogItem(pMsg->hWin,ID_EDIT_0);
        //EDIT_SetText(hItem, "1");
        break;
        
      case WM_KEY:
        if((((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)==0)
        {
            switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
            {
              case GUI_KEY_YELLOW:
                  WM_DeleteWindow(g_hWin_Input);
                  g_hWin_Input = WM_HWIN_NULL;
                  Select_Focus();
                  break;
              case GUI_KEY_GREEN:
                  SelectInputEdit(g_sys_ctrl.selectWidget);
                  WM_DeleteWindow(g_hWin_Input);
                  Select_Focus();
                  g_hWin_Input = WM_HWIN_NULL;
                  break;
            }
        }
        break;
      
      default:
        WM_DefaultProc(pMsg);
        break;
      }
}

#if 1
static void _Init_ListBox(WM_MESSAGE *pMsg, int ListBoxNum)
{
    
    //u8 tmpTextBuf[32];
    u32 Sel_Num;
    WM_HWIN hItem;
    int i;
    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTBOX_0);
    
    switch(ListBoxNum)
    {
        case LST_ANIP_SWITCH:
            LISTBOX_AddString(hItem, SwitchOn);
            LISTBOX_AddString(hItem, SwitchOff);
            break;
        case LST_WEEK:
            for(i = 0;i<7;i++)
            {
                LISTBOX_AddString(hItem, TextWeek[i]);
            }
            break;
        default:
           break;
    }
    FRAMEWIN_SetText(pMsg->hWin,ListViewFrameTitle[ListBoxNum]);

}



static void SelectLSTRow(int  WidgetNum)
{
    WM_HWIN hItem;
    WM_HWIN hWin;
    int  SelNum;

    
    hItem=WM_GetDialogItem(g_hWin_Input,ID_LISTBOX_0);
    SelNum=LISTBOX_GetSel(hItem);
    
    switch(WidgetNum) /*WidgetNum是为了把不同的页面的不同Edit工具句柄传回来*/
    {
    case LST_ANIP_SWITCH: 
        hItem = PRW_GetElecPrtSwth();
        EDIT_SetText(hItem,TextSwitch[SelNum]);

        if(0 == SelNum)
        {
            g_sys_ctrl.fhd_sw = ON;
        }
        else
        {
            g_sys_ctrl.fhd_sw = OFF;
        }
        break;
        
    case LST_WEEK:
        g_gui_time[DAY_POS] = SelNum;
        hItem = TMS_GetWeek();
        EDIT_SetText(hItem,TextWeek[SelNum]);
        break;
        
    default:
        break;
    }
}

#endif

static void _cbListBoxDlg(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    // int     Id;
    //int     SelNum;

    switch(pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
          GUI_UC_SetEncodeUTF8();
          FRAMEWIN_SetTitleHeight(pMsg->hWin,25);
          _Init_ListBox(pMsg,g_sys_ctrl.selectWidget);
          Button_Color_Set(pMsg);
          
          break;
          
        case WM_KEY:
          if((((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)==0)
          {
              switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
              {
                case GUI_KEY_YELLOW:
                    WM_DeleteWindow(g_hWin_Input);
                    Select_Focus();
                    break;
                case GUI_KEY_GREEN:
                    SelectLSTRow(g_sys_ctrl.selectWidget);
                    WM_DeleteWindow(g_hWin_Input);
                    Select_Focus();
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
**********************************************************************/


//listbox对话框

WM_HWIN Create_ListBox_Set(WM_HWIN parentWin);

WM_HWIN Create_ListBox_Set(WM_HWIN parentWin)
{
    WM_HWIN hWin;
    hWin=GUI_CreateDialogBox(_aListBoxCreate , GUI_COUNTOF(_aListBoxCreate),_cbListBoxDlg,parentWin,0,0);
    return hWin;
}



//edit对话框
WM_HWIN Create_Edit_Set(WM_HWIN parentWin);

WM_HWIN Create_Edit_Set(WM_HWIN parentWin)
{
    WM_HWIN hWin;
    hWin=GUI_CreateDialogBox(_aEditCreate , GUI_COUNTOF(_aEditCreate), _cbEditDlg ,parentWin,0,0);
    return hWin;
}





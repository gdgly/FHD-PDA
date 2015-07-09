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

#define ID_TEXT_0      (GUI_ID_USER + 0x07)

static const GUI_WIDGET_CREATE_INFO _aSelCreate[] = {
  { FRAMEWIN_CreateIndirect,  "Select", ID_FRAMEWIN_0,  20,  40,  200, 200,  0, 0x0, 0 },
  { LISTBOX_CreateIndirect,   "Select", ID_LISTBOX_0,   6,   7,   182, 120,  0, 0x0, 0 },
  { BUTTON_CreateIndirect,    TextOK,   ID_BUTTON_0,    7,   138, 55,  25,   0, 0x0, 0 },
  { BUTTON_CreateIndirect,    TextBack, ID_BUTTON_1,    132, 138, 55,  25,   0, 0x0, 0 },
};

static const GUI_WIDGET_CREATE_INFO _aInptCreate[] = {
  { FRAMEWIN_CreateIndirect, "Input",   ID_FRAMEWIN_0, 20,  60, 200, 160, 0, 0x0,  0 },
  { BUTTON_CreateIndirect,   TextOK,    ID_BUTTON_0,   7,   82, 55,  25,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   TextBack,  ID_BUTTON_1,   132, 82, 55,  25,  0, 0x0,  0 },
  { EDIT_CreateIndirect,     "Input",   ID_EDIT_0,     14,  38, 165, 25,  0, 0x64, 0 },
};
static const GUI_WIDGET_CREATE_INFO _aChgCreate[] = {
  { FRAMEWIN_CreateIndirect, "Change",  ID_FRAMEWIN_0, 20,  60, 200, 160, 0, 0x0,  0 },
  { TEXT_CreateIndirect,     PTChgNote, ID_TEXT_0,     17,  5,  180, 20,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   TextOK,    ID_BUTTON_0,   7,   82, 55,  25,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   TextBack,  ID_BUTTON_1,   132, 82, 55,  25,  0, 0x0,  0 },
  { EDIT_CreateIndirect,     "Change",  ID_EDIT_0,     14,  38, 165, 25,  0, 0x64, 0 },
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
    
    if((g_hWin_SysSet > 0)&&(g_hWin_SysInfo > 0)&&(g_hWin_TimeSet <= 0))
    {
        WM_SetFocus(g_hWin_SysInfo);
    }
    
    if((g_hWin_SysSet > 0)&&(g_hWin_SysInfo <= 0)&&(g_hWin_TimeSet > 0))
    {
        WM_SetFocus(g_hWin_TimeSet);
        TMS_SetFocus();
    }

    if((g_hWin_SysSet > 0)&&(g_hWin_SysInfo <= 0)&&(g_hWin_TimeSet <= 0))
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
    int tmp;
    
    hItem=WM_GetDialogItem(g_hWin_Edit,ID_EDIT_0);
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
                tmp = 2000;
            }
            if(tmp < 800)
            {
                tmp = 800;
            }
            tmp = (tmp/100) * 100;
            sprintf(tmpTextBuf, "%d", tmp);
            hItem = PRW_GetPrtTime();
            break;
        case EDT_DEV_ADDR:
            EDIT_GetText(hItem, tmpTextBuf,5);

            if(atoi(tmpTextBuf) > 255)
            {
                g_sys_ctrl.new_dev_addr = 255;

                sprintf(tmpTextBuf, "%d", g_sys_ctrl.new_dev_addr);
            }
            else
            {
                g_sys_ctrl.new_dev_addr = atoi(tmpTextBuf);
            }
            
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

        case EDT_COMMUNICATE_ADDR:
            EDIT_GetText(hItem, tmpTextBuf, 6);

            if(atoi(tmpTextBuf) > 255)
            {
                g_sys_ctrl.dev_addr = 255;

                sprintf(tmpTextBuf, "%d", g_sys_ctrl.dev_addr);
            }
            else
            {
                g_sys_ctrl.dev_addr = atoi(tmpTextBuf);
            }

            g_sys_ctrl.new_dev_addr = g_sys_ctrl.dev_addr;
            
            hItem = SST_GetCommAddr();
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
        case EDT_COMMUNICATE_ADDR:
            hItem = SST_GetCommAddr();
            EDIT_GetText(hItem, tmpTextBuf, 6);
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
                  WM_DeleteWindow(g_hWin_Edit);
                  g_hWin_Edit = WM_HWIN_NULL;
                  Select_Focus();
                  break;
              case GUI_KEY_GREEN:
                  SelectInputEdit(g_sys_ctrl.selectWidget);
                  WM_DeleteWindow(g_hWin_Edit);
                  Select_Focus();
                  g_hWin_Edit = WM_HWIN_NULL;
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

    
    hItem=WM_GetDialogItem(g_hWin_Edit,ID_LISTBOX_0);
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
                    WM_DeleteWindow(g_hWin_Edit);
                    Select_Focus();
                    break;
                case GUI_KEY_GREEN:
                    SelectLSTRow(g_sys_ctrl.selectWidget);
                    WM_DeleteWindow(g_hWin_Edit);
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

void EdtValDown(void)
{
    WM_HWIN hItem;
    u32 tmp;
    u8 tmpBuf[8];
    hItem = WM_GetDialogItem(g_hWin_Edit, ID_EDIT_0);
    EDIT_GetText(hItem, tmpBuf, 5);
    tmp = atoi(tmpBuf);
    if(tmp < 100)
    {
        tmp = 100;
    }
    else if(tmp > 2000)
    {
        tmp = 2000;
    }
    else if((tmp > 100)&&(tmp <= 2000))
    {
        tmp -= 100;
    }
    sprintf(tmpBuf, "%d", tmp);
    EDIT_SetText(hItem, tmpBuf);
}
void EdtValUp(void)
{
    WM_HWIN hItem;
    u32 tmp;
    u8 tmpBuf[8];
    hItem = WM_GetDialogItem(g_hWin_Edit, ID_EDIT_0);
    EDIT_GetText(hItem, tmpBuf, 5);
    tmp = atoi(tmpBuf);
    if(tmp < 100)
    {
        tmp = 100;
    }
    else if(tmp > 2000)
    {
        tmp = 2000;
    }
    else if((tmp>= 100)&&(tmp < 2000))
    {
        tmp += 100;
    }
    sprintf(tmpBuf, "%d", tmp);
    EDIT_SetText(hItem, tmpBuf);
}
static void _cbProtWin(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    u8 tmpBuf[8];
    switch(pMsg->MsgId)
    {
        case WM_INIT_DIALOG:
            GUI_UC_SetEncodeUTF8();
            FRAMEWIN_SetTitleHeight(pMsg->hWin,25);
            FRAMEWIN_SetText(pMsg->hWin, ProtectTime);
            hItem = PRW_GetPrtTime();
            EDIT_GetText(hItem, tmpBuf, 5);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
            EDIT_SetText(hItem, tmpBuf);
            WM_DisableWindow(hItem);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
            BUTTON_SetBkColor(hItem, 0, GUI_GREEN);
            hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
            BUTTON_SetBkColor(hItem, 0, GUI_YELLOW);
            break;
        case WM_KEY:
            if((((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)==0)
            {
                switch(((WM_KEY_INFO *)(pMsg->Data.p))->Key)
                {
                    case GUI_KEY_YELLOW:
                        WM_DeleteWindow(g_hWin_Edit);
                        Select_Focus();
                        break;
                    case GUI_KEY_GREEN:
                        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
                        EDIT_GetText(hItem, tmpBuf, 5);
                        hItem = PRW_GetPrtTime();
                        EDIT_SetText(hItem, tmpBuf);
                        WM_DeleteWindow(g_hWin_Edit);
                        Select_Focus();
                        break;
                    case GUI_KEY_UP:
                        EdtValUp();
                        break;
                    case GUI_KEY_DOWN:
                        EdtValDown();
                        break;
                }
            }
            break;
    }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************/


//listbox对话框

WM_HWIN Create_Edit_Sel(WM_HWIN parentWin);

WM_HWIN Create_Edit_Sel(WM_HWIN parentWin)
{
    WM_HWIN hWin;
    hWin=GUI_CreateDialogBox(_aSelCreate , GUI_COUNTOF(_aSelCreate),_cbListBoxDlg,parentWin,0,0);
    return hWin;
}



//edit对话框
WM_HWIN Create_Edit_Inpt(WM_HWIN parentWin);

WM_HWIN Create_Edit_Inpt(WM_HWIN parentWin)
{
    WM_HWIN hWin;
    hWin=GUI_CreateDialogBox(_aInptCreate , GUI_COUNTOF(_aInptCreate), _cbEditDlg ,parentWin,0,0);
    return hWin;
}



WM_HWIN Create_Edit_Chg(WM_HWIN parentWin);

WM_HWIN Create_Edit_Chg(WM_HWIN parentWin)
{
    WM_HWIN hWin;
    hWin=GUI_CreateDialogBox(_aChgCreate , GUI_COUNTOF(_aChgCreate), _cbProtWin ,parentWin,0,0);
    return hWin;
}

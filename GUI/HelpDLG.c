#include "DIALOG.h"
#include "includes.h"
/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_MULTIEDIT_0     (GUI_ID_USER + 0x01)
#define ID_BUTTON_0     (GUI_ID_USER + 0x02)
#define ID_BUTTON_1     (GUI_ID_USER + 0x03)



/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, NULL, ID_WINDOW_0, 0, 0, 240, 295, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, NULL, ID_MULTIEDIT_0, 1, 2, 238, 255, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, TextBack,    ID_BUTTON_0, 175,  262, 55, 25, 0, 0x0, 0 },
  //{ BUTTON_CreateIndirect, "OK",  ID_BUTTON_1,  8,  264, 55, 25, 0, 0x0, 0 },
};


/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int i;
  switch (pMsg->MsgId) { 
  case WM_INIT_DIALOG:

    
    GUI_UC_SetEncodeUTF8();
    
     //
    // Initialization of 'Multiedit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetFont(hItem,&GUI_Font_Song_16);
    MULTIEDIT_SetWrapWord(hItem);//启用自动换行
    MULTIEDIT_SetReadOnly(hItem,1);//只读模式
    //MULTIEDIT_SetText(hItem, FHD_PDA);
    for(i = 0;i < 14;i++)
    {
        MULTIEDIT_AddText(hItem, helpText[i]);
    }
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0);
    BUTTON_SetBkColor(hItem,0,GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
#if 0
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_1);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
#endif
    
    break;
  

  case WM_KEY:
    {
        if((((WM_KEY_INFO *)(pMsg->Data.p))->PressedCnt)==1)
        {
            switch(((WM_KEY_INFO*)(pMsg->Data.p))->Key)
            {
                case GUI_KEY_YELLOW:
                    WM_DeleteWindow(g_hWin_Help);
                    g_hWin_Help=WM_HWIN_NULL;
                    WM_ShowWindow(g_hWin_TimeBar);
                    WM_ShowWindow(g_hWin_Date);
                    WM_SetFocus(g_hWin_Menu);
                    break;
                case GUI_KEY_GREEN:
                    //GUI_WARN(g_hWin_Help, GUI_MSBOX_UPDATE_CFM);
                    //GUI_WARN(g_hWin_Help,8);
                    
                    //DEV_Power_Off();

                    //while(1);
                    break;
            }
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
*       CreateHelp
*/
WM_HWIN CreateHelp(void);
WM_HWIN CreateHelp(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_Menu, 0, 0);
  return hWin;
}







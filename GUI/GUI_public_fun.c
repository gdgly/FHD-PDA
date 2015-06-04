#include "includes.h"
#include "WIDGET.h"
#include "DIALOG.h"


u8 g_gui_time[MAX_RTC_ITEM];
u8 s_prbf[512];


const u8 c_645ctrlDef[2][PLC_CTRL_MAX_NUM] = 
{ 
    //97规约
    {0x05, 0x01, 0x04,  4,5,6,7,8,9,10,11,12,13,14,15,16},
    //07规约
    {0x13, 0X11, 0X14,  0x19,5,6,7,8,9,10,11,12,13,14,15,16}
};


const u32 c_645DidoDef[2][PLC_CTRL_MAX_NUM] = 
{ 
    //97规约
    {0x901f,    0x902f,     0x9410,     0x9420,5,6,7,8,9,10,11,12,13,14,15,16},
    //07规约
    {0x0001ff00,0X0002ff00, 0X0001ff01, 0X0002ff01,4,5,6,7,8,9,10,11,12,13,14,15,}
};

const u8 g_self_check_pwd[] = {'2', '2', '8', '8', '4', '4', '6', '6', '\0'}; 


void GUI_Fill_Zero(u8 *tempbuf)
{
    int len;
    int i;
    
    len = strlen(tempbuf);
    if(len < 12)
    {
        for(i = 0; i < len; i++)
        {
            tempbuf[GUI_645_ADDR_LENGTH - i-1] = tempbuf[len-i-1];
        }
        
        for(i = 0; i < (GUI_645_ADDR_LENGTH - len); i++)
        {
            tempbuf[i] = '0';
        }
        tempbuf[12] = '\0';
    }
    else if(len > 12)
    {
        tempbuf[12] = '\0';
    }
         
    return ;
}


u8 GUI_Hex2Char(u8 ch)
{
    ch = ch & 0x0f;
    if(ch < 10)
    {
        ch += '0';
    }
    else
    {
        ch = (ch - 10) + 'A';
    }
    
    return ch;
}


u8* GUI_hex2MeterAddrStr(u8 * srcBuf, u32 len)
{

    u8 *send_ptr;
    send_ptr = s_prbf;

    if(len > 256)
    {
        s_prbf[0] = 0;
        return s_prbf;
    }
    
    *send_ptr++ = GUI_Hex2Char( srcBuf[5]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[5]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[4]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[4]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[3]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[3]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[2]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[2]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[1]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[1]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[0]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[0]&0x0f);
    
  
    *send_ptr++ = 0;
    
    return s_prbf;

}
#if 0
u32 GUI_GetMeterAddr(u8 * dbuf, u8 * gbuf)
{
    u8 rmd_ch;
    u8 i;
    
    for(i = 0; i < (GUI_645_ADDR_LENGTH/2); i++ )
    {
        if(((rmd_ch = GUI_char2hex(dbuf[i<<1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        gbuf[5-i] = rmd_ch<<4;
       
        
        if(((rmd_ch = GUI_char2hex(dbuf[(i<<1)+1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        gbuf[5-i] |= rmd_ch;
    }

    return DEV_OK;
}
#endif
void GUI_GetHexDataFlag(u8 * strbuf, u8* dataflag, u8 len)
{
    
    u8 i;
    
    if(len > 4)
        return;

    for(i = 0; i < len; i++)
    {
        *strbuf++ = GUI_Hex2Char( dataflag[len-i-1]>>4);
        *strbuf++ = GUI_Hex2Char( dataflag[len-i-1]&0x0f);
    }

    *strbuf = '\0';
}
#if 0
u32 GUI_GetStrDataFlag(u8 * dbuf, u32 pro_ver)
{
    u8 rmd_ch;
    u8 i;
    u8 len;

    if(pro_ver == DL_T_07)
        len = DL645_07_DATA_ITEM_LEN;
    else
        len = DL645_97_DATA_ITEM_LEN;
    
    for(i = 0; i < len; i++ )
    {
        if(((rmd_ch = GUI_char2hex(dbuf[i<<1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        g_send_para_pkg.dataFlag[len-1-i] = rmd_ch<<4;
        g_sys_ctrl.defaultDataFlag[len-1-i] = rmd_ch<<4;
        
        if(((rmd_ch = GUI_char2hex(dbuf[(i<<1)+1])) == 0xff))
        {
            //error proc
            return DEV_ERROR;
        }
        
        g_send_para_pkg.dataFlag[len-1-i] |= rmd_ch;
        g_sys_ctrl.defaultDataFlag[len-1-i] |= rmd_ch;
    }

    return DEV_OK;
}
#endif

//电量化为尖峰平谷小数
u8* GUI_hex2PowerDataStr(u8 * srcBuf, u32 len)
{

    u8 *send_ptr;
    send_ptr = s_prbf;

    if(len > 256)
    {
        s_prbf[0] = 0;
        return s_prbf;
    }

    *send_ptr++ = GUI_Hex2Char( srcBuf[3]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[3]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[2]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[2]&0x0f);
    *send_ptr++ = GUI_Hex2Char( srcBuf[1]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[1]&0x0f);
    *send_ptr++ = '.';
    *send_ptr++ = GUI_Hex2Char( srcBuf[0]>>4);
    *send_ptr++ = GUI_Hex2Char( srcBuf[0]&0x0f);
    
  
    *send_ptr++ = 0;
    
    return s_prbf;

}

#if 0
void GUI_print_recv_buf()
{
    MULTIEDIT_HANDLE hObj;
    u16 i;
    u8 *send_ptr;
    //int n = 0;
    if(g_proto_para.recv_len && (g_proto_para.recv_len < 80))
    {
        send_ptr = s_prbf;
        *send_ptr++ = 'R';
        *send_ptr++ = ':';
        for(i = 0; i < g_proto_para.recv_len; i++)
        {
            *send_ptr++ = GUI_Hex2Char( g_proto_para.recv_buf[i]>>4);
            *send_ptr++ = GUI_Hex2Char( g_proto_para.recv_buf[i]&0x0f);
            *send_ptr++ = ' ';
        }
        *send_ptr++ = '\n';
        *send_ptr++ = 0;

        if((g_sys_ctrl.guiState == GUI_PLC_MSG_TEST)
            ||(g_sys_ctrl.guiState == GUI_PLC_MSG_READ)
            ||(g_sys_ctrl.guiState == GUI_PLC_MSG_FREQ))
        {
#if 0
            hObj = MSG_Get_MultiEdit();
            MULTIEDIT_AddText(hObj, s_prbf); 
            if(g_sys_ctrl.guiState == GUI_PLC_MSG_READ)
            {
                RMD_proc_resp_data();
            }

            if(g_sys_ctrl.guiState == GUI_PLC_MSG_TEST)
            {
                STM_proc_resp_data();
            }
#endif
        }
        else if(g_sys_ctrl.guiState == GUI_PLC_MSG_LISTING)
        {
#if 0
            hObj = MNT_Get_MultiEdit();
            if(g_sys_ctrl.numMultiedit > 15)
            {
                g_sys_ctrl.numMultiedit = 0;
                MULTIEDIT_SetText(hObj, "\0"); 
            }
            MULTIEDIT_AddText(hObj, s_prbf); 
#endif
        }
    }
}
#else //华兄
void GUI_Recv_Msg_Proc(void)
{
    switch(g_gui_para.state)
    {
    case FHD_GUI_TRM_CAL:
        if(RECV_RES_SUCC == g_fhdp_para.recv_result)
        {
            if(g_fhdp_para.recv_len)
            {
                GUI_Trm_Cal_Proc();
            }
        }
        else if(RECV_RES_INVALID == g_fhdp_para.recv_result)
        {
            ERR_NOTE(g_hWin_TrmCal, ERR_RECEIVE_DATA);
        }
        else 
        {
            ERR_NOTE(g_hWin_TrmCal, ERR_COMMUNICATE);
        }
        break;

    case FHD_GUI_TRM_CONF:
        if(RECV_RES_SUCC == g_fhdp_para.recv_result)
        {
            if(g_fhdp_para.recv_len)
            {
                GUI_Trm_Conf_Proc();
            }
        }
        else if(RECV_RES_INVALID == g_fhdp_para.recv_result)
        {
            if(FHD_CMD_WRITE_TRM_CONF == g_gui_para.cmd)
            {
                ERR_NOTE(g_hWin_TrmConf, ERR_RECEIVE_DATA);
            }
        }
        else 
        {   
            if(FHD_CMD_WRITE_TRM_CONF == g_gui_para.cmd)
            {
                ERR_NOTE(g_hWin_TrmConf, ERR_COMMUNICATE);
            }
        }
        break;

    case FHD_GUI_TRM_STATE:
        if(RECV_RES_SUCC == g_fhdp_para.recv_result)
        {
            if(g_fhdp_para.recv_len)
            {
                GUI_Trm_State_Proc();
            }
        }
        else if(RECV_RES_INVALID == g_fhdp_para.recv_result)
        {
            if(FHD_CMD_READ_TRM_STATE == g_gui_para.cmd)
            {
                ERR_NOTE(g_hWin_TrmState, ERR_RECEIVE_DATA);
            }
        }
        else 
        {
            if(FHD_CMD_READ_TRM_STATE == g_gui_para.cmd)
            {
                ERR_NOTE(g_hWin_TrmState, ERR_COMMUNICATE);
            }
        }
        break;

    case FHD_GUI_TRM_LOG:
        if(RECV_RES_SUCC == g_fhdp_para.recv_result)
        {
            if(g_fhdp_para.recv_len)
            {
                GUI_Trm_Log_Proc();
            }
        }
        else if(RECV_RES_INVALID == g_fhdp_para.recv_result)
        {
            ERR_NOTE(g_hWin_TrmLog, ERR_RECEIVE_DATA);
        }
        else
        {
            ERR_NOTE(g_hWin_TrmLog, ERR_COMMUNICATE);
        }
        break;

    default:
        break;
    }
}
#endif

void Msg_Pool_Proc(void)
{
    switch(g_gui_para.cmd)
    {
    case FHD_CMD_READ_TRM_STATE:
        g_gui_para.state = FHD_GUI_TRM_CONF;
        g_gui_para.cmd = FHD_CMD_READ_TRM_CONF;
        OSMboxPost(g_sys_ctrl.up_mbox, &g_gui_para);        
        break;

    default:
        break;
    }
}

#if 0
void GUI_print_send_buf()
{
    MULTIEDIT_HANDLE hObj;
    u16 i;
    u8 *send_ptr;
    
    if(g_proto_para.send_len && (g_proto_para.send_len < 80))
    {        
        send_ptr = s_prbf;
        *send_ptr++ = 'S';
        *send_ptr++ = ':';
        for(i = 0; i < g_proto_para.send_len; i++)
        {
            *send_ptr++ = GUI_Hex2Char( g_proto_para.send_buf[i]>>4);
            *send_ptr++ = GUI_Hex2Char( g_proto_para.send_buf[i]&0x0f);
            *send_ptr++ = ' ';
        }
        *send_ptr++ = '\n';
        *send_ptr++ = 0;
        //hObj = MSG_Get_MultiEdit();
        //MULTIEDIT_AddText(hObj, s_prbf);        
    }
}
#else //华兄
void GUI_Send_Msg_Proc(void)
{
    
}
#endif

WM_HWIN GUI_Get_PROGBAR()
{

    return WM_HWIN_NULL;
    
}

#if 0
#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void GUI_Msg_Proc()
{
    WM_HWIN hItem;

    

    if(g_proto_para.sendStatus == PLC_MSG_SENDING)    
    {        
        //g_proto_para.send_buf
        //GUI_Msg_Upload();//数据发送图标变色
        //hItem=TSK_Get_Upload_Text();
        //TEXT_SetTextColor(hItem,GUI_GREEN);
        //GUI_Delay(15);
        GUI_Msg_Upload(1);
        GUI_print_send_buf();
        return;
       
    }

    if((g_proto_para.result == PLC_RES_FAIL) || (g_proto_para.result == PLC_RES_TIMEOUT))
    {
        g_proto_para.result = PLC_RES_NONE;
        //ERR_NOTE(g_hWin_menu, 10);
        hItem = GUI_Get_PROGBAR();
        PROGBAR_SetBarColor(hItem, 0, GUI_RED);
        return;
    }

    if(g_proto_para.result == PLC_RES_ERROR_FRAME)
    {
       
        GUI_Msg_Download(1);
        GUI_print_recv_buf();

        hItem = GUI_Get_PROGBAR();
        if(hItem != WM_HWIN_NULL)
        {
            PROGBAR_SetBarColor(hItem, 0, GUI_RED);
            g_sys_ctrl.testProgBarVal = 100;
            PROGBAR_SetValue(hItem, g_sys_ctrl.testProgBarVal);              
        }
           
        //g_sys_ctrl.guiState = GUI_PLC_MSG_IDLE;
        g_proto_para.sendStatus = PLC_MSG_IDLE; 
        g_proto_para.result = PLC_RES_NONE;
        return;
    }

    if((g_proto_para.sendStatus == PLC_MSG_RECEIVED) )
    {
        if( g_proto_para.result == PLC_RES_SUCC )
        {
            GUI_Msg_Download(1);
            GUI_print_recv_buf();

            hItem = GUI_Get_PROGBAR();
            if(hItem != WM_HWIN_NULL)
            {
                g_sys_ctrl.testProgBarVal = 100;
                PROGBAR_SetValue(hItem, g_sys_ctrl.testProgBarVal);              
            }
        }        
        //g_sys_ctrl.guiState = GUI_PLC_MSG_IDLE;
        g_proto_para.sendStatus = PLC_MSG_IDLE;
        g_proto_para.result = PLC_RES_NONE;
    }
    
}
#else //华兄
void GUI_Msg_Proc(void)
{
    if(MSG_STATE_SENDING == g_fhdp_para.msg_state)    
    {        
        GUI_Msg_Upload(ON);

        fhd_msg_record(FHD_MSG_SEND);

        GUI_Send_Msg_Proc();
    }    
    else if(MSG_STATE_RECEIVED == g_fhdp_para.msg_state)
    {
        if(RECV_RES_TIMEOUT != g_fhdp_para.recv_result)
        {
            GUI_Msg_Download(ON);

            fhd_msg_record(FHD_MSG_RECV);
        }

        GUI_Recv_Msg_Proc();

        g_fhdp_para.msg_state = MSG_STATE_NONE;
        g_fhdp_para.recv_result = RECV_RES_IDLE;

        Msg_Pool_Proc();             
    }
}
#endif

SEND_PARA_PKG g_send_para_pkg;      //参数包

GUI_PARA g_gui_para;

//MBOX_SEND_INFO g_mail_box_info; //邮箱发送的消息



    



















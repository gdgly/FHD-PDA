#ifndef _WIDGETEDIT_
#define _WIDGETEDIT_

typedef WM_HWIN (*pGetHandler)(void);

//#define GET_WIDGET_HANDLE(a,b)     return WM_GetDialogItem(a,b)

#define LST_ANIP_SWITCH          0  /*�ε籣������*/

#define EDT_VOLTAGE_REFER        0  /*�ο���ѹ*/
#define EDT_ANIP_TIME            1  /*�ε籣��ʱ��*/
#define EDT_DEV_ADDR             2  /*�豸��ַ*/


#define EDT_YEAR                 3
#define EDT_MONTH                4
#define EDT_DAY                  5
#define EDT_HOUR                 6
#define EDT_MINITE               7
#define EDT_SECOND               8

#define EDT_SRC_TIME             9
#define EDT_SHUTDOWN             10

//#define CPS_EDT_NUMBER           4   //ͨ�Ų�������ҳ��edit����
#define PRW_EDT_NUMBER           3   //��������ҳ
#define SYS_EDT_NUMBER           4   //ϵͳ״̬ҳ
#define SST_EDT_NUMBER           2


WM_HWIN CPS_GetCheck(void);

WM_HWIN CPS_GetVtgChk(void);

WM_HWIN CPS_GetVtgRead(void);

void PRW_FocusSel(void);
void SSD_FocusSel(void);
void SST_FocusSel(void);
void SST_ColorChange(void);























#endif /*_WIDGETEDIT_*/































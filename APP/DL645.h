#ifndef __DL645_H__
#define __DL645_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define DL645_2007	                            2
#define DL645_1997	                            1
#define	DL645_no	                            0

#define CCTT_CONTROL_CODE_MASK               0x1F

#define FRM_CTRW_97_READ_SLVS_DATA           0x01 //�����������������
#define FRM_CTRW_97_WRITE_SLVS_DATA          0x04 //������������д����

#define FRM_CTRW_07_READ_SLVS_DATA           0x11 //�����������������
#define FRM_CTRW_07_EXT_READ_SLVS_DATA       0x12 //����
#define FRM_CTRW_07_WRITE_SLVS_DATA          0x14 //������������д����

#define FRM_CTRW_07_BROAD_READ_ADDR          0x13 //�㲥����ַ

#define DL645_97_DATA_ITEM_LEN                  2
#define DL645_07_DATA_ITEM_LEN                  4

#define DL645_ADDR_LEN				            6

#define DL645_REPLY_STAT_MASK                  0x40

#define DL645_FIX_LEN                          12

#define DL645_Data_MAX                         230 //����


#define DL645_FRAME_ERROR      200
#define DL645_FRAME_OK         0

typedef struct
{
	u8	Status;//֡״̬: 0�Ƿ���1�Ϸ���2��ַ���ϡ�3��ַ������
	u8	protocol;//Э��
	u8 	C;//645������
	u8	ID_Leng;//��ʾ�볤��
}DL645_Frame_Stat_C;

#pragma pack(1)

typedef struct
{
    u8 Start;
    u8 Addr[6];
    u8 Start1;
    u8 C;
    u8 L;
    u8 Data[DL645_Data_MAX];
} DL645_Frame_C;
#pragma pack()
void Create_DL645_Frame(u8 *Addr, u8 C, u8 len, DL645_Frame_C *DL645_Frame);
u32 Analysis_DL645_Frame(u8 *Addr,u8 *buff, DL645_Frame_Stat_C * pFreame_st);

#ifdef __cplusplus
}
#endif

#endif

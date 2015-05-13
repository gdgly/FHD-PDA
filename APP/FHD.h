#ifndef __FHD_H__
#define __FHD_H__


typedef unsigned char		uint8;              /* defined for unsigned 8-bits integer variable */
typedef signed   char		int8;				/* defined for signed 8-bits integer variable */
typedef unsigned short		uint16;				/* defined for unsigned 16-bits integer variable */
typedef signed   short		int16;				/* defined for signed 16-bits integer variable */
typedef unsigned int		uint32;				/* defined for unsigned 32-bits integer variable */
typedef signed   int		int32;				/* defined for signed 32-bits integer variable */
typedef unsigned long long	uint64;				/* defined for unsigned 64-bits integer variable */
typedef signed   long long	int64;				/* defined for signed 64-bits integer variable */
typedef float				fp32;				/* single precision floating point variable (32bits) */
typedef double				fp64;				/* double precision floating point variable (64bits) */

/* FUNC */
#define MODBUS_CTRL_OUTPUT          0x05		//���ü̵������
#define MODBUS_READ_REG			    0x03		//��ȡ���������Ĵ���
#define MODBUS_WRITE_REG		    0x10		//д�뵥�����߶���Ĵ���
#define MODBUS_EXT_READ_REG		    0x46		//��ȡ���ܼĴ���
#define MODBUS_EXT_WRITE_REG	    0x47		//д�����ܼĴ���

/* REG */
#define MODBUS_CTRL_START_ADDR      10000		//ң�ؼĴ�����ַ
#define MODBUS_CTRL_END_ADDR	    20000		//ң�ؼĴ�����ַ
#define MODBUS_FAST_START_ADDR	    30000		//���ټĴ�����ַ
#define MODBUS_FAST_END_ADDR	    40000		//���ټĴ�����ַ
#define MODBUS_GRP_START_ADDR	    40000		//ң��Ͷ�ֵ�Ĵ�����ַ
#define MODBUS_GRP_END_ADDR	        60000		//ң��Ͷ�ֵ�Ĵ�����ַ
#define MODBUS_TIME_START_ADDR      60000		//ʱ��Ĵ�����ַ
#define MODBUS_TIME_END_ADDR	    60500		//ʱ��Ĵ�����ַ
#define MODBUS_EXT_GRP_START_ADDR   63000		//���ܶ�ֵ�Ĵ�����ַ
#define MODBUS_EXT_GRP_END_ADDR	    65000		//���ܶ�ֵ�Ĵ�����ַ
#define MODBUS_CONF_START_ADDR      61000		//���üĴ�����ַ
#define MODBUS_CONF_END_ADDR	    65500		//���üĴ�����ַ
#define MODBUS_CONF0_ADDR		    0			//���üĴ�����ַ
#define MODBUS_CONF1_ADDR		    500		    //��ֵ�Ĵ�����ַ
#define MODBUS_CONF2_ADDR		    1000	    //��ֵ�Ĵ�����ַ
#define MODBUS_CONF3_ADDR		    2000	    //��ֵ�Ĵ�����ַ
#define MODBUS_CONF4_ADDR		    3000		//��ֵ�Ĵ�����ַ
#define MODBUS_CONF_SIZE		    4000

/* ϵͳ״̬ */
#define SYS_ERROR_EEPROM_RW         0x0001
#define RECORD_DROP_TIME_FLAG       0x0002      //�Ƿ��Ѿ����������һ�ε����ʱ��
#define SYS_DROP_ACTION             0x0004      //�Ƿ��е��ݼ̵���Ͷ�ж���
#define SYS_POWER_CHECK_FLAG        0x0008      //�Ƿ�������������
#define SYS_RELAY_ON_FLAG           0x0010      //�Ӵ����Ƿ��ڱպ�״̬
#define SYS_JDQ_ON_FLAG             0x0020      //�̵����Ƿ��ڱպ�״̬

#define FHD_FRAME_OK                TRUE
#define FHD_RRAME_ERROR             FALSE

uint16 mb_swap(uint16 usData);
uint32 mb_swap_32(uint8 *usData);
uint16 FHD_MakeFrame(uint8 ctrl, uint16 reg_addr, uint8 *pdata, uint16 len, uint8 *buf);
uint8 FHD_CheckFrame(uint8 *buf, uint16 len);
uint8 FHD_GetDevAddr(void);
uint8 *FHD_GetUserData(uint8 *buf);
uint8 FHD_GetUserDataLen(uint8 *buf);


#endif

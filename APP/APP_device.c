#include "includes.h"


ROM_PARA g_rom_para = {
    TAG_WORD_TABLE_MAP, //��ǩ
    0xffffbb00, //������־�����ܸ���
    0, //CRC              
    0x20150520, //�汾����    

    SYS_BEEP_ON, //����������
    60, //�Զ�˯��ʱ��
    180, //�Զ��ػ�ʱ��
};

SYS_CTRL g_sys_ctrl;

/**
  * @brief  STM32 soft reset.
  * @param  None
  * @retval None
  */
void DEV_SoftReset(void)
{
  __set_FAULTMASK(SET);
    
  NVIC_SystemReset();
}



unsigned int calccrc(unsigned char crcbuf,unsigned int crc)
{
    unsigned char i, chk;
    
    crc=crc ^ crcbuf;
    
    for(i=0;i<8;i++)
    {        
        chk=crc&1;
        crc=crc>>1;
        crc=crc&0x7fff;
        if (chk==1)
            crc=crc^0xa001;
        crc=crc&0xffff;
    }
    
    return crc;
}


void  APP_memcpy( unsigned char         *pdest,
                        const  unsigned char  *psrc,
                        unsigned int csize)
{
    if(csize > (RAM_COPY_MAX_SIZE_K * 1024))
    {
        return;
    }

    while(csize--)
    {
        *pdest++ = *psrc++;
    }
}

//buf��Ҫ����������׵�ַ��len����Ҫ����ĳ���

unsigned int Get_checksum(unsigned char *buf, unsigned short len)
{
    unsigned char hi,lo; 
    unsigned int i; 
    unsigned int crc; 
    crc=0xFFFF; 
    for (i=0;i<len;i++) 
    { 
        crc=calccrc(*buf,crc); 
        buf++; 
    } 
    hi=crc%256; 
    lo=crc/256; 
    crc=(hi<<8)|lo; 
    return crc; 
}

void DEV_Power_Off()
{
    if(g_sys_ctrl.usb_state)
    {
        return;
    }
    
    SYS_PWR_OFF();
    LCD_BL_OFF();
    LED_KEY_OFF();
    LED_SLEEP_OFF();
}

//ȷ���洢�ռ��ǿյ�
unsigned int DEV_verify_blank(unsigned int * buf_4byte, unsigned int len)
{
    unsigned int res = DEV_FALSE;

    if(len == 0)
        return res;
        
    while(len--)
    {
        if((*buf_4byte++) != 0XFFFFFFFF)
            return DEV_FALSE;
    }
    
    return DEV_TRUE;
}





//������
void DEV_Parameters_Read(void)
{

    unsigned int * pPara;
    int n = ROM_PARA_WRITE_TIMES;
    unsigned int crcr;
    //����ָ��ָ�����һ֡����
    //Ȼ��ʼ�ж��ǲ��Ǵ�������

    while(n)
    {
        n--;
        pPara = (unsigned int *)((unsigned int)ROM_ADDR_SYS_PARA + (n*ROM_ADDR_PARA_SIZE));
        if(pPara[0] == TAG_WORD_TABLE_MAP)
        {
            crcr = Get_checksum((unsigned char *)&pPara[3], (ROM_ADDR_PARA_SIZE - 12));
            //У��
            if(crcr == pPara[2])
            {
                //�汾���
                if(pPara[3] >= g_rom_para.versionDate)
                {
                    APP_memcpy((unsigned char *)&g_rom_para, (unsigned char *)pPara, ROM_ADDR_PARA_SIZE);
                    g_sys_ctrl.paraAddr = (unsigned int)pPara;
     
                }
                else
                {
                    //recover the original data from old database;

                }
                return;
            }
        }
    }

    
    DEV_Parameters_Write();
    
    
}



unsigned int DEV_Parameters_Write(void)
{
    unsigned int * pPara;
    int n = 0;
    unsigned int  i, addr, *s_addr;

    HAL_FLASH_Unlock();

    g_rom_para.crc = Get_checksum((unsigned char *)&g_rom_para.versionDate, ROM_ADDR_PARA_SIZE - 12);
    
    while(n < (ROM_PARA_WRITE_TIMES))
    {        
        pPara = (unsigned int *)((unsigned int)ROM_ADDR_SYS_PARA + n * ROM_ADDR_PARA_SIZE);
        if(DEV_verify_blank(pPara, ROM_ADDR_PARA_SIZE/4) == DEV_TRUE)
        {
            g_sys_ctrl.paraAddr= (unsigned int)pPara;  //added on 2014.12.30
            addr = (unsigned int)pPara;
            s_addr = (unsigned int *)&g_rom_para;
            for(i = 0; i < ROM_ADDR_PARA_SIZE/4; i++)
            {
                //Flash002_WritePage(addr, s_addr);
                HAL_FLASH_Program(TYPEPROGRAM_WORD,addr, *s_addr);
                addr += 4;
                s_addr ++;
            }

            HAL_FLASH_Lock();
            return DEV_OK;
        }
        n++;
    }
    
    FLASH_Erase_Sector(ROM_PARA_ERASE_SECTOR, VOLTAGE_RANGE_3);

    pPara = (unsigned int *)(ROM_ADDR_SYS_PARA);
    
    addr = (unsigned int)pPara;
    s_addr = (unsigned int *)&g_rom_para;
   
    for(i = 0; i < ROM_ADDR_PARA_SIZE/4; i++)
    {            
        HAL_FLASH_Program(TYPEPROGRAM_WORD, addr, *s_addr);
        addr += 4;
        s_addr ++;
    }

    HAL_FLASH_Lock();
    
    return DEV_OK;    
}

const u8 c_test_addr[6] = {0x76, 0x05, 0x31, 0x01, 0x0, 0x18};
const u8 c_default_dataflag[4] = {0x00, 0x00, 0x01, 0x0};

const u8 c_97_dataflag[4] = {0x90,0x10, 0x00, 0x00};

void DEV_Init(void)
{  
    DEV_Parameters_Read();  
    
    g_sys_ctrl.shutdown_timeout = 0;

    g_sys_ctrl.sleep_timeout = 0;

    g_sys_ctrl.sysPowerState = SYS_POWER_WAKEUP;

    g_sys_ctrl.dev_addr = 1;

    g_sys_ctrl.new_dev_addr = g_sys_ctrl.dev_addr;

    g_sys_ctrl.sd_format_flag = FALSE;
}


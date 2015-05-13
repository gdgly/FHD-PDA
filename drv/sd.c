#include "includes.h"

					   					   
u8 SD_Type = 0; //SD�������� 

void SPI1_SetSpeed(u8 speed)
{
    __HAL_SPI_DISABLE(&hspi1);
    
	SPI1->CR1 &= 0xFFC7; 
	SPI1->CR1 |= speed;
    
    if(SPI_CR1_SPE != (hspi1.Instance->CR1 & SPI_CR1_SPE))
    {
        __HAL_SPI_ENABLE(&hspi1);
    }
}

//SD����ʼ����ʱ��,��Ҫ����
void SD_SPI_SpeedLow(void)
{
 	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_256);//���õ�����ģʽ	
}

//SD������������ʱ��,���Ը�����
void SD_SPI_SpeedHigh(void)
{
 	SPI1_SetSpeed(SPI_BAUDRATEPRESCALER_32);//���õ�����ģʽ	
}

//ȡ��ѡ��,�ͷ�SPI����
void SD_DisSelect(void)
{
	SD_CS=1;
 	SD_ReadWriteByte(0xff);//�ṩ�����8��ʱ��
}

//ѡ��sd��,���ҵȴ���׼��OK
//����ֵ:0,�ɹ�;1,ʧ��;
u8 SD_Select(void)
{
	SD_CS=0;
	if(SD_WaitReady()==0)return 0;//�ȴ��ɹ�
	SD_DisSelect();
    
	return 1;//�ȴ�ʧ��
}

//�ȴ���׼����
//����ֵ:0,׼������;����,�������
u8 SD_WaitReady(void)
{
	u32 t=0;

    
	do
	{
		if(SD_ReadWriteByte(0XFF)==0XFF)return 0;//OK
		t++;		  	
	}while(t<0XFFFFFF);//�ȴ� 
	
	return 1;
}

//�ȴ�SD����Ӧ
//Response:Ҫ�õ��Ļ�Ӧֵ
//����ֵ:0,�ɹ��õ��˸û�Ӧֵ
//����,�õ���Ӧֵʧ��
u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFFF;//�ȴ�����	 

    
	while ((SD_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//�ȴ��õ�׼ȷ�Ļ�Ӧ  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//�õ���Ӧʧ��   
	else return MSD_RESPONSE_NO_ERROR;//��ȷ��Ӧ
}

//��sd����ȡһ�����ݰ�������
//buf:���ݻ�����
//len:Ҫ��ȡ�����ݳ���.
//����ֵ:0,�ɹ�;����,ʧ��;	
u8 SD_RecvData(u8*buf,u16 len)
{			  	  
	if(SD_GetResponse(0xFE))return 1;//�ȴ�SD������������ʼ����0xFE
    while(len--)//��ʼ��������
    {
        *buf=SD_ReadWriteByte(0xFF);
        buf++;
    }
    //������2��αCRC��dummy CRC��
    SD_ReadWriteByte(0xFF);
    SD_ReadWriteByte(0xFF);	
    
    return 0;//��ȡ�ɹ�
}

//��sd��д��һ�����ݰ������� 512�ֽ�
//buf:���ݻ�����
//cmd:ָ��
//����ֵ:0,�ɹ�;����,ʧ��;	
u8 SD_SendBlock(u8*buf,u8 cmd)
{	
	u16 t;		  	  

    
	if(SD_WaitReady())return 1;//�ȴ�׼��ʧЧ
	SD_ReadWriteByte(cmd);
	if(cmd!=0XFD)//���ǽ���ָ��
	{
		for(t=0;t<512;t++)SD_ReadWriteByte(buf[t]);//����ٶ�,���ٺ�������ʱ��
	    SD_ReadWriteByte(0xFF);//����crc
	    SD_ReadWriteByte(0xFF);
		t=SD_ReadWriteByte(0xFF);//������Ӧ
		if((t&0x1F)!=0x05)return 2;//��Ӧ����									  					    
	}						
    
    return 0;//д��ɹ�
}

//��SD������һ������
//����: u8 cmd   ���� 
//      u32 arg  �������
//      u8 crc   crcУ��ֵ	   
//����ֵ:SD�����ص���Ӧ															  
u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)
{
    u8 r1;	
	u8 Retry=0; 

    
	SD_DisSelect();//ȡ���ϴ�Ƭѡ
	if(SD_Select())return 0XFF;//ƬѡʧЧ 
	//����
    SD_ReadWriteByte(cmd | 0x40);//�ֱ�д������
    SD_ReadWriteByte(arg >> 24);
    SD_ReadWriteByte(arg >> 16);
    SD_ReadWriteByte(arg >> 8);
    SD_ReadWriteByte(arg);	  
    SD_ReadWriteByte(crc); 
	if(cmd==CMD12)SD_ReadWriteByte(0xff);//Skip a stuff byte when stop reading
    //�ȴ���Ӧ����ʱ�˳�
	Retry=0X1F;
	do
	{
		r1=SD_ReadWriteByte(0xFF);
	}while((r1&0X80) && Retry--);	 
	//����״ֵ̬
	
    return r1;
}		  

//��ȡSD����CID��Ϣ��������������Ϣ
//����: u8 *cid_data(���CID���ڴ棬����16Byte��	  
//����ֵ:0��NO_ERR
//		 1������														   
u8 SD_GetCID(u8 *cid_data)
{
    u8 r1;	   

    
    //��CMD10�����CID
    r1=SD_SendCmd(CMD10,0,0x01);
    if(r1==0x00)
	{
		r1=SD_RecvData(cid_data,16);//����16���ֽڵ�����	 
    }
	SD_DisSelect();//ȡ��Ƭѡ
	if(r1)return 1;
	else return 0;
}			

//��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
//����:u8 *cid_data(���CID���ڴ棬����16Byte��	    
//����ֵ:0��NO_ERR
//		 1������														   
u8 SD_GetCSD(u8 *csd_data)
{
    u8 r1;	 

    
    r1=SD_SendCmd(CMD9,0,0x01);//��CMD9�����CSD
    if(r1==0)
	{
    	r1=SD_RecvData(csd_data, 16);//����16���ֽڵ����� 
    }
	SD_DisSelect();//ȡ��Ƭѡ
	if(r1)return 1;
	else return 0;
}  

//��ȡSD����������������������   
//����ֵ:0�� ȡ�������� 
//����:SD��������(������/512�ֽ�)
//ÿ�������ֽ�����Ϊ512����Ϊ�������512�����ʼ������ͨ��.														  
u32 SD_GetSectorCount(void)
{
    u8 csd[16];
    u32 Capacity;  
    u8 n;
	u16 csize;  	

    
	//ȡCSD��Ϣ������ڼ����������0
    if(SD_GetCSD(csd)!=0) return 0;	    
    //���ΪSDHC�����������淽ʽ����
    if((csd[0]&0xC0)==0x40)	 //V2.00�Ŀ�
    {	
		csize = csd[9] + ((u16)csd[8] << 8) + 1;
		Capacity = (u32)csize << 10;//�õ�������	 		   
    }else//V1.XX�Ŀ�
    {	
		n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
		csize = (csd[8] >> 6) + ((u16)csd[7] << 2) + ((u16)(csd[6] & 3) << 10) + 1;
		Capacity= (u32)csize << (n - 9);//�õ�������   
    }
    
    return Capacity;
}

//��ʼ��SD��
u8 SD_Init(void)
{
    u8 r1;      // ���SD���ķ���ֵ
    u16 retry;  // �������г�ʱ����
    u8 buf[4];  
	u16 i;


    // ---------------------------------------------------
    
	SPI1_Init();

    if(SPI_CR1_SPE != (hspi1.Instance->CR1 & SPI_CR1_SPE))
    {
        __HAL_SPI_ENABLE(&hspi1);
    }

    // ---------------------------------------------------
    
 	SD_SPI_SpeedLow();	//���õ�����ģʽ 
 	for(i=0;i<10;i++)SD_ReadWriteByte(0XFF);//��������74������
	retry=20;
	do
	{
		r1=SD_SendCmd(CMD0,0,0x95);//����IDLE״̬
	}while((r1!=0X01) && retry--);
 	SD_Type=0;//Ĭ���޿�
	if(r1==0X01)
	{
		if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
		{
			for(i=0;i<4;i++)buf[i]=SD_ReadWriteByte(0XFF);	//Get trailing return value of R7 resp
			if(buf[2]==0X01&&buf[3]==0XAA)//���Ƿ�֧��2.7~3.6V
			{
				retry=0XFFFE;
				do
				{
					SD_SendCmd(CMD55,0,0X01);	//����CMD55
					r1=SD_SendCmd(CMD41,0x40000000,0X01);//����CMD41
				}while(r1&&retry--);
				if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//����SD2.0���汾��ʼ
				{
					for(i=0;i<4;i++)buf[i]=SD_ReadWriteByte(0XFF);//�õ�OCRֵ
					if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //���CCS
					else SD_Type=SD_TYPE_V2;   
				}
			}
		}else//SD V1.x/ MMC	V3
		{
			SD_SendCmd(CMD55,0,0X01);		//����CMD55
			r1=SD_SendCmd(CMD41,0,0X01);	//����CMD41
			if(r1<=1)
			{		
				SD_Type=SD_TYPE_V1;
				retry=0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{
					SD_SendCmd(CMD55,0,0X01);	//����CMD55
					r1=SD_SendCmd(CMD41,0,0X01);//����CMD41
				}while(r1&&retry--);
			}else
			{
				SD_Type=SD_TYPE_MMC;//MMC V3
				retry=0XFFFE;
				do //�ȴ��˳�IDLEģʽ
				{											    
					r1=SD_SendCmd(CMD1,0,0X01);//����CMD1
				}while(r1&&retry--);  
			}
			if(retry==0||SD_SendCmd(CMD16, SD_SECTOR_SIZE, 0X01)!=0)SD_Type=SD_TYPE_ERR;//����Ŀ�
		}
	}
	SD_DisSelect();//ȡ��Ƭѡ
	SD_SPI_SpeedHigh();//����
	if(SD_Type)return 0;
	else if(r1)return r1; 	  
    
	return 0xaa;//��������
}

//��SD��
//buf:���ݻ�����
//sector:����
//cnt:������
//����ֵ:0,ok;����,ʧ��.
u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;

    
	if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;//ת��Ϊ�ֽڵ�ַ
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD17,sector,0X01);//������
		if(r1==0)//ָ��ͳɹ�
		{
			r1=SD_RecvData(buf,512);//����512���ֽ�	   
		}
	}else
	{
		r1=SD_SendCmd(CMD18,sector,0X01);//����������
		do
		{
			r1=SD_RecvData(buf,512);//����512���ֽ�	 
			buf+=512;  
		}while(--cnt && r1==0); 	
		SD_SendCmd(CMD12,0,0X01);	//����ֹͣ����
	}   
	SD_DisSelect();//ȡ��Ƭѡ
	
	return r1;
}

//дSD��
//buf:���ݻ�����
//sector:��ʼ����
//cnt:������
//����ֵ:0,ok;����,ʧ��.
u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt)
{
	u8 r1;

    
	if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//ת��Ϊ�ֽڵ�ַ
	if(cnt==1)
	{
		r1=SD_SendCmd(CMD24,sector,0X01);//������
		if(r1==0)//ָ��ͳɹ�
		{
			r1=SD_SendBlock(buf,0xFE);//д512���ֽ�	   
		}
	}else
	{
		if(SD_Type!=SD_TYPE_MMC)
		{
			SD_SendCmd(CMD55,0,0X01);	
			SD_SendCmd(CMD23,cnt,0X01);//����ָ��	
		}
 		r1=SD_SendCmd(CMD25,sector,0X01);//����������
		if(r1==0)
		{
			do
			{
				r1=SD_SendBlock(buf,0xFC);//����512���ֽ�	 
				buf+=512;  
			}while(--cnt && r1==0);
			r1=SD_SendBlock(0,0xFD);//����512���ֽ� 
		}
	}   
	SD_DisSelect();//ȡ��Ƭѡ
	
	return r1;
}	   

// ---------------------------------------------------

uint8_t SD_ReadWriteByte(uint8_t Data)
{
    uint8_t temp = 0;


    while(!__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_TXE));
    hspi1.Instance->DR = Data;
            
    while(!__HAL_SPI_GET_FLAG(&hspi1, SPI_FLAG_RXNE));
    temp = hspi1.Instance->DR;

    return (temp);
}

// ---------------------------------------------------

/* FatFs low level driver @���� Email: 591881218@qq.com */
u8 SD_ReadBlock(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumberOfBlocks)
{
    return (SD_ReadDisk(pBuffer, ReadAddr, NumberOfBlocks));
}

u8 SD_WriteBlock(uint8_t *pBuffer, uint32_t WriteAddr, uint32_t NumberOfBlocks)
{
    return (SD_WriteDisk(pBuffer, WriteAddr, NumberOfBlocks));
}

// ---------------------------------------------------

u8 get_sd_info(void)
{
    FATFS fs, *p_fs;
    FRESULT res;
    u32 free_clusters, free_sectors, total_sectors;


    g_sys_ctrl.sd_total_capacity = 0; //����������λKB
    g_sys_ctrl.sd_free_capacity = 0; //ʣ����������λKB

    res = f_mount(SD_DRV, &fs);

    if(FR_OK != res)
    {
        return (FALSE);
    }

    p_fs = &fs;
    
    res = f_getfree("0:/", &free_clusters, &p_fs);

    if(FR_OK != res)
    {
        f_mount(SD_DRV, NULL);
        
        return (FALSE);
    }    

    total_sectors = (p_fs->n_fatent - 2) * p_fs->csize; //��������
    free_sectors = free_clusters * p_fs->csize; //����������

#if (512 != _MAX_SS)
    total_sectors *= p_fs->ssize / 512;
    free_sectors *= p_fs->ssize / 512;
#endif	    

    g_sys_ctrl.sd_total_capacity = total_sectors >> 1; //����������λKB
    g_sys_ctrl.sd_free_capacity = free_sectors >> 1; //ʣ����������λKB

    f_mount(SD_DRV, NULL);

    return (TRUE);
}

char SD_FileName[MAX_SD_FILE_NUM][MAX_SD_FILE_NAME_LEN] = {0};
unsigned long SD_FileSize[MAX_SD_FILE_NUM] = {0}; 
FIL_TIMESTAMP SD_FileTimestamp[MAX_SD_FILE_NUM] = {0};

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void scan_files(char *path)
{	
	FILINFO finfo;
	DIR dirs;
	int i = 0;
	char *fname;
    
#if _USE_LFN
#define _DF1S	0x81
    static char lfn[_MAX_LFN * (_DF1S ? 2 : 1) + 1];
    finfo.lfname = lfn;
    finfo.lfsize = sizeof(lfn);
#endif


	if(FR_OK == f_opendir(&dirs, path)) 
	{
        memset(SD_FileName, 0, sizeof(SD_FileName));
        
		while(FR_OK == f_readdir(&dirs, &finfo))
		{
#if _USE_LFN
            fname = *finfo.lfname ? finfo.lfname : finfo.fname;
#else
            fname = finfo.fname;
#endif		

            if((!fname[0]) || (i >= MAX_SD_FILE_NUM))
            {
            	break;
            }
            
            if(finfo.fattrib & AM_ARC)
            {
            	strcpy(SD_FileName[i], fname);
                
            	i++;
            }
		}
	}
}

/******************************
//
//    ��ȡ�ļ���Ŀ
//
*******************************/
u32 Get_Files_Number(void)
{
    u32 sd_file_num;
    FATFS fs;
    if(FR_OK == f_mount(SD_DRV, &fs))
    {
        scan_files("/");

        sd_file_num = 0;
        while(SD_FileName[sd_file_num][0])
        {
            sd_file_num++;
        }
    }
    f_mount(SD_DRV,NULL);
    return sd_file_num;

}

// ---------------------------------------------------

#define TEST_SECTOR_COUNT                      2

u8 g_test_buf[SD_SECTOR_SIZE * TEST_SECTOR_COUNT];

void FatFs_Test(void)
{
    u16 i;
    u32 sd_sector_size, sd_sector_count, sd_capacity;
    FATFS fs;
    FIL fp;
    UINT br;
    DIR dj;
    FRESULT res;
    

    g_rtc_time[YEAR_POS] = Hex2BcdChar(15);
    g_rtc_time[MONTH_POS] = Hex2BcdChar(1);
    g_rtc_time[DATE_POS] = Hex2BcdChar(19);
    g_rtc_time[DAY_POS] = Hex2BcdChar(1);
    g_rtc_time[HOUR_POS] = Hex2BcdChar(15);
    g_rtc_time[MIN_POS] = Hex2BcdChar(51);
    g_rtc_time[SEC_POS] = Hex2BcdChar(15);                
    RTC_WriteTime(g_rtc_time);

    if(FALSE == get_sd_info())
    {
        DEBUG_PRINT(("Get SD info error!\n"));
    }

    res = f_mount(SD_DRV, &fs);

    if(FR_OK == res)
    {
        //res = f_mkfs(SD_DRV, SD_FORMAT, _MAX_SS);
        
        if(FR_OK == res)
        {
            //DEBUG_PRINT(("SD format ok!\n"));
        }
        else
        {
            //DEBUG_PRINT(("SD format error!\n"));
        }
        
        //res = f_mkdir("SD������");

        if(FR_OK == res)
        {
        }
        else
        {
        }
        
        //res = f_chdir("SD������");

        if(FR_OK == res)
        {
        }
        else
        {
        }
        
        res = f_open(&fp, "SD.txt", FA_READ | FA_WRITE | FA_OPEN_ALWAYS);

        if(FR_OK == res)
        {
        }
        else
        {
        }

        res = f_read(&fp, g_test_buf, SD_SECTOR_SIZE, &br);

        if((FR_OK == res) && (0 != br))
        {
            
        }
        else
        {
            for(i = 0; i < (SD_SECTOR_SIZE * TEST_SECTOR_COUNT); i++)
            {
                g_test_buf[i] = '\0';
            }

            sd_sector_size = SD_SECTOR_SIZE;
            sd_sector_count = SD_GetSectorCount();
            sd_capacity = sd_sector_size * sd_sector_count;
            
            sprintf(g_test_buf, "SD����Ϣ(FatFs) \n\nSD Sector Size: %lu\nSD Sector Count: %lu\nSD Capacity: %lu\n",
                    sd_sector_size,
                    sd_sector_count,
                    sd_capacity);
            
            f_puts(g_test_buf, &fp); 
        }

        f_close(&fp);
    }

    f_mount(SD_DRV, NULL);  
}

// ---------------------------------------------------

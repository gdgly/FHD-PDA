#ifndef __FM_H__
#define __FM_H__

#ifdef __cplusplus
 extern "C" {
#endif


#define FDISK      0
#define SFD        1

#define SD_DRV     0
#define SD_FORMAT  FDISK

#define MAX_SD_FILE_NUM  50
#define MAX_SD_FILE_NAME_LEN  16

typedef struct file_timestamp
{
    u32 date;
    u32 time;
} FIL_TIMESTAMP;

extern char SD_FileName[MAX_SD_FILE_NUM][MAX_SD_FILE_NAME_LEN];
extern unsigned long SD_FileSize[MAX_SD_FILE_NUM];
extern FIL_TIMESTAMP SD_FileTimestamp[MAX_SD_FILE_NUM];

u8 get_sd_info(void);
void scan_files(char *path);
void FatFs_Test(void);
void FM_Format_Disk(void);
u32 get_file_num(void);
u8 plc_listen_record(void);
u8 fhd_msg_record(u8 msg_type);


#ifdef __cplusplus
}
#endif

#endif

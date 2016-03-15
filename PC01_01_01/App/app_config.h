#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "typedef.h"
/*config*/
#define CHANNEL_NUMBER          1

/*define*/
#define SET_NOREFUND			(0x00)
#define SET_REFUND				(0x01)

#define SET_FUN_TIME			(0x00)
#define SET_FUN_POWER			(0x01)


typedef struct
{
    UINT8 Addr;
} SYSTEM_STRUCT;

extern data SYSTEM_STRUCT s_System;

#define SYSTEM_SETTING_SECTOR               0

extern code UINT8 SoftWareVersion;
extern code UINT8 HardWareVersion;

void app_configInit(void);
BOOL app_configRead(UINT8 Sector);
void app_configWrite(UINT8 Sector);
void app_confingHandler1s(void);
void app_configDisplay(void);

#endif


#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

#include "typedef.h"

typedef struct
{
    UINT8 Time;
    UINT8 Temp;
}SYSTEM_STRUCT;

extern SYSTEM_STRUCT s_System;

extern UINT8 u8_setTime;
extern UINT8 u8_setTemp;

extern CONST UINT8 SoftWareVersion;
extern CONST UINT8 HardWareVersion;

void app_configInit(void);
BOOL app_configRead(void);
void app_configWrite(void);

#endif

#include "app_config.h"
#include "hwa_eeprom.h"
#include "string.h"

CONST UINT8 SoftWareVersion = 0x01;
CONST UINT8 HardWareVersion = 0x01;

SYSTEM_STRUCT s_System;

UINT8 u8_setTime = 0;
UINT8 u8_setTemp = 0;

void app_configInit(void)
{
	BOOL flag = FALSE;
	hwa_eepromInit();
	if (app_configRead() == FALSE)
	{
		flag = TRUE;
	}
	if(flag
		|| s_System.Time<30
		|| s_System.Time>90
		||s_System.Temp<30
		|| s_System.Temp>48
	)
	{
		s_System.Time = 30;
		s_System.Temp = 40;
        app_configWrite();
	}
	u8_setTime = s_System.Time;
	u8_setTemp = s_System.Temp;
}

BOOL app_configRead(void)
{
	BOOL b_errorFlag = hwa_eepromReadSector((UINT8*)&s_System, 0);
	return b_errorFlag;
}

void app_configWrite(void)
{
	hwa_eepromWriteSector((UINT8*)&s_System, 0);
}

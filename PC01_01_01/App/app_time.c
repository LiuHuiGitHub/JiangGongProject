#include "app_time.h"
#include "app_config.h"
#include "drive_relay.h"
#include "drive_led.h"
#include "drive_buzzer.h"
#include "hwa_eeprom.h"
#include "string.h"

data UINT16 u16_DisplayTime[AD_CHANNEL_NUM];//剩余时间，(显示时间的10倍)

void app_timeClear(UINT8 channel)
{
    if(channel == AD_CHANNEL_NUM)
    {
        memset((UINT8*)u16_DisplayTime, 0x00, sizeof(u16_DisplayTime));
    }
    else
    {
        u16_DisplayTime[channel] = 0;
    }
    app_configWrite(TIME_SAVE_SECTOR);
}

void app_timeLoad(void)
{
    UINT8 i;
    if(hwa_eepromReadSector((UINT8*)u16_DisplayTime, TIME_SAVE_SECTOR) == FALSE)
    {
        app_timeClear(AD_CHANNEL_NUM);
    }
    else
    {
        for(i = 0; i < AD_CHANNEL_NUM; i++)
        {
            if(u16_DisplayTime[i] > 9999)
            {
                app_timeClear(AD_CHANNEL_NUM);
                break;
            }
        }
    }
}

void app_timeDisplay500ms(void)
{
	UINT8 channel;
    UINT16 time;
	static BOOL b_lightFlashCount = FALSE;
    
    b_lightFlashCount = !b_lightFlashCount;

    for (channel = 0; channel < AD_CHANNEL_NUM; channel++)
    {
        time = u16_DisplayTime[channel];
        if (time == 0)                       //时间为0闪烁显示通道号
        {
            u8_ledDisBuff[0] = 19;
            u8_ledDisBuff[1] = 19;
            u8_ledDisBuff[2] = 19;
            u8_ledDisBuff[3] = 0;
            u8_ledDot = 0x00;
            drv_relayClose(channel);
        }
        else                                //时间不为零显示时间
        {
            if (b_lightFlashCount)
            {
                u8_ledDot = 0xFF;
            }
            else
            {
                u8_ledDot = 0x00;
            }
            u8_ledDisBuff[0] = time/60/10;
            u8_ledDisBuff[1] = time/60%10;
            u8_ledDisBuff[2] = time%60/10;
            u8_ledDisBuff[3] = time%60%10;
            drv_relayOpen(channel);
        }
    }
}

void app_timeAddTime(UINT8 channel, UINT16 money)
{
	u16_DisplayTime[channel] += money;    //TBD
    if(u16_DisplayTime[channel] > 99*60+59)
    {
        u16_DisplayTime[channel] = 99*60+59;
    }
    app_configWrite(TIME_SAVE_SECTOR);
}

UINT16 app_timeGetTime(UINT8 channel)
{
    return u16_DisplayTime[channel];
}
				   
void app_timePower1min(void)
{
    UINT8 i;
    static UINT8 u8_TimeCount10min = 0;
    for(i = 0; i < AD_CHANNEL_NUM; i++)
    {
        if(u16_DisplayTime[i])
        {
            u16_DisplayTime[i]--;
            if(u16_DisplayTime[i] == 0)
	        {
	            app_timeClear(i);
	        }
        }
    }
    if(++u8_TimeCount10min >= 10)
    {
        u8_TimeCount10min = 0;
        for(i = 0; i < AD_CHANNEL_NUM; i++)
        {
            if(u16_DisplayTime[i])
            {
                app_configWrite(TIME_SAVE_SECTOR);
                break;
            }
        }
    }
}


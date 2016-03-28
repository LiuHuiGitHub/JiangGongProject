#include "app_time.h"
#include "app_adc.h"
#include "app_test.h"
#include "drive_tm1640.h"
#include "sys_relay.h"
#include "hwa_eeprom.h"
#include "hwa_coin.h"
#include "hwa_uart.h"
#include "string.h"

UINT8 DisplayBuff[32];

UINT16 u16_DisplayTime[ALL_CHANNEL];//剩余时间，(显示时间的10倍)

void app_timeInit(void)
{
    drv_tm1640Init();
    app_timeLoad();
}

void app_timeClear(UINT8 channel)
{
    hwa_coinOff();                              //写E2时禁止投币
    if(channel == ALL_CHANNEL)
    {
        memset((UINT8*)u16_DisplayTime, 0x00, sizeof(u16_DisplayTime));
    }
    else
    {
        u16_DisplayTime[channel] = 0;
    }
    hwa_eepromWriteSector((UINT8*)u16_DisplayTime, TIME_SAVE_SECTOR);
}

void app_timeLoad(void)
{
    UINT8 i;
    if(hwa_eepromReadSector((UINT8*)u16_DisplayTime, TIME_SAVE_SECTOR) == FALSE)
    {
        app_timeClear(ALL_CHANNEL);
    }
    else
    {
        for(i = 0; i < ALL_CHANNEL; i++)
        {
            if(u16_DisplayTime[i] > 9999)
            {
                app_timeClear(ALL_CHANNEL);
                break;
            }
        }
    }
}

void app_timeSaveTime(void)
{
    hwa_coinOff();
    hwa_eepromWriteSector((UINT8*)u16_DisplayTime, TIME_SAVE_SECTOR);
}

UINT8 u8_lightFlashCount = 0;
void app_timeDisplay100ms(void)     //时间为0时闪烁显示通道号
{
    UINT16 time;
    UINT8 channel, space;
    u8_lightFlashCount++;
    for(channel = 0; channel < ALL_CHANNEL; channel++)
    {        
        space = 3 * channel + channel / 5 + 1;
        time = u16_DisplayTime[channel] / 10;

        /*if(OverCurrentChannelFlag[channel])     //过流保护
        {
            DisplayBuff[space] = 'F';
            DisplayBuff[space + 1] = 'u';
            DisplayBuff[space + 2] = 'L';
            sys_relayClose(channel);
        }
        else */if(FuseState[channel])     //保险丝正常
        {
            if(time == 0)                       //时间为0显示通道号
            {
                if(u8_lightFlashCount >= ALL_CHANNEL)
                {
                    u8_lightFlashCount = 0;
                }
                else if(u8_lightFlashCount >= 5)
                {
                    DisplayBuff[space] = ' ';
                }
                else
                {
                    DisplayBuff[space] = channel + '0';
                }
                DisplayBuff[space + 1] = ' ';
                DisplayBuff[space + 2] = ' ';
                sys_relayClose(channel);
            }
            else                                //时间不为零显示时间
            {
                if(OverCurrentChannelFlag[channel] && (u8_lightFlashCount==0 || u8_lightFlashCount==1 || u8_lightFlashCount==2 || u8_lightFlashCount==5 || u8_lightFlashCount==6 || u8_lightFlashCount==7))
                {
                    DisplayBuff[space] = ' ';
                    DisplayBuff[space + 1] = ' ';
                    DisplayBuff[space + 2] = ' ';
                }
                else
                {
                    DisplayBuff[space] = time / 100 + '0';
                    DisplayBuff[space + 1] = time / 10 % 10 + '0';
                    DisplayBuff[space + 2] = time % 10 + '0';
                }
                sys_relayOpen(channel);
            }
        }
        else
        {
            DisplayBuff[space] = '-';
            DisplayBuff[space + 1] = '-';
            DisplayBuff[space + 2] = '-';
            sys_relayClose(channel);
            if(u16_DisplayTime[channel])
            {
                app_timeClear(channel);
            }
        }
    }
//    drv_tm1640Display(DisplayBuff);
}

void app_timeAddMoney(UINT8 channel, UINT16 time)
{
    u16_DisplayTime[channel] += time * 10;
    if(u16_DisplayTime[channel] > 9999)
    {
        u16_DisplayTime[channel] = 9999;
    }
    app_timeSaveTime();
}

void app_timePower1min(void)
{
    UINT8 i;
    UINT8 tmp;
    for(i = 0; i < ALL_CHANNEL; i++)
    {
        if(s_SystemSetting.Function == SET_FUN_POWER)
        {
        	if(OverCurrentChannelFlag[i])							//满量程计量
        	{
            	tmp = 10 * s_SystemSetting.OverCurrentMultiple;
            }
            else if(FuseState[i] && app_adcGetValue(i) > s_SystemSetting.BaseCurrent) //保险丝正常，电流大于基本电流，以倍数累计电量
            {
                tmp = (UINT8)(app_adcGetValue(i) *10 / s_SystemSetting.BaseCurrent);
            }
            else
            {
                tmp = 10;                                      //如果保险丝断或浮充，累计时间
            }
        }
        else
        {
            tmp = 10;
        }
        if(u16_DisplayTime[i] > tmp)
        {
            u16_DisplayTime[i] -= tmp;
        }
        else if(u16_DisplayTime[i])
        {
            app_timeClear(i);
        }
    }
}

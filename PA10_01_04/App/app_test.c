#include "STC15F2K60S2.h"
#include "app_coin.h"
#include "app_test.h"
#include "app_adc.h"
#include "app_time.h"
#include "sys_delay.h"
#include "sys_relay.h"
#include "drive_tm1650.h"
#include "drive_tm1640.h"
#include "hwa_eeprom.h"
#include "hwa_key.h"
#include "hwa_coin.h"
#include "string.h"
#include "stdio.h"

code UINT8 c_softwareVersion[] = " 16 03 27-V05";

/*
typedef struct
{
    UINT16 CoinTimeOrPower;
    UINT16 CardTimeOrPower;
    UINT8 IMaxValue;				//0~9	->0~4A
    UINT8 OverCurrentMultiple;		//1~9    ->过流时，电量累加基本电流的倍数1~9倍
    UINT8 Function;					//0x00:time 0x01:power
    UINT16 BaseCurrent;				//基本电流设置，AD值小于等于该值时按该值计算，大于该值时成倍计量，
}SYSTEM_SETTINGS_STRUCT;
*/

code SYSTEM_SETTINGS_STRUCT s_FactorySetting = {180, 180, 9, 5, SET_FUN_POWER, 300};
SYSTEM_SETTINGS_STRUCT s_SystemSetting;
MONEY_SAVE_STRUCT s_moneySave;
UINT8 FuseState[10];							//保险丝状态，1:正常，0:熔断

void app_testClearMoney(void)
{
    memset(&s_moneySave, 0x00, sizeof(s_moneySave));
    hwa_eepromWriteSector((UINT8*)&s_moneySave, MONEY_SAVE_SECTOR);
}
void app_testSavePutInMoney(void)
{
	hwa_coinOff();
    hwa_eepromWriteSector((UINT8*)&s_moneySave, MONEY_SAVE_SECTOR);
}
void app_testSaveSystemSetting(void)
{
    hwa_eepromWriteSector((UINT8*)&s_SystemSetting, SYSTEM_SET_SECTOR);
}
void app_testInit(void)
{
    if( hwa_eepromReadSector((UINT8*)&s_SystemSetting, SYSTEM_SET_SECTOR) == FALSE ||
        s_SystemSetting.CoinTimeOrPower > 999 ||
        s_SystemSetting.CardTimeOrPower > 999 ||
        s_SystemSetting.IMaxValue < 1 || s_SystemSetting.IMaxValue > 9 ||
        s_SystemSetting.BaseCurrent < 100 || s_SystemSetting.BaseCurrent > 900 ||
        (s_SystemSetting.OverCurrentMultiple <1 || s_SystemSetting.OverCurrentMultiple >9) ||
        (s_SystemSetting.Function != SET_FUN_TIME && s_SystemSetting.Function != SET_FUN_POWER))
    {
        app_testRestoreFactorySetting();
    }
    if( hwa_eepromReadSector((UINT8*)&s_moneySave, MONEY_SAVE_SECTOR) == FALSE ||
        s_moneySave.CoinCount > 99999 ||
        s_moneySave.CardCount > 99999
    )
    {
        app_testClearMoney();
    }
    memset(FuseState, TRUE, sizeof(FuseState));
}

void app_testRestoreFactorySetting(void)
{
    memcpy(&s_SystemSetting, &s_FactorySetting, sizeof(s_SystemSetting));
    hwa_eepromWriteSector((UINT8*)&s_SystemSetting, SYSTEM_SET_SECTOR);
}

void SetDisplayString(UINT8 index, UINT8 *pPtr)
{
	while(*pPtr != '\0')
	{
    	DisplayBuff[index++] = *pPtr++;
    }
}
void SetDisplayNumber(UINT8 index, UINT16 num)
{
	DisplayBuff[index] = num / 100 + '0';
    DisplayBuff[index + 1] = num / 10 % 10 + '0';
    DisplayBuff[index + 2] = num % 10 + '0';
}

void app_testVersion(void)
{
    memset(DisplayBuff, ' ', sizeof(DisplayBuff));
    memcpy(&DisplayBuff[1], c_softwareVersion, 9);

    DisplayBuff[13] = 'a';
    DisplayBuff[14] = '-';
    DisplayBuff[15] = s_SystemSetting.IMaxValue + '0';

    DisplayBuff[17] = 'F';
    DisplayBuff[18] = '-';
    DisplayBuff[19] = s_SystemSetting.OverCurrentMultiple + '0';

	SetDisplayString(23, "bc-");    
    SetDisplayNumber(26, (UINT16)s_SystemSetting.BaseCurrent * 10 / 256);
    drv_tm1640Display(DisplayBuff);
    sys_delayms(2000);
}

UINT8 FuseTestChannel = 0;

BOOL app_testFuseTest(UINT8 channel)
{
    UINT8 TestChannel;
    UINT8 index;
    UINT16 value;
	BOOL flag;
	if(drv_tm1650GetKey() == KEY_TEST)
	{
		flag = TRUE;
	}
	else
	{
		flag = FALSE;
	}
    if(channel==FUSE_ALL_CHANNEL)
    {
    	sys_relayClose(ALL_RELAY);
    	sys_relayOpen(TEST_RELAY);
    	memset(DisplayBuff, ' ', sizeof(DisplayBuff));
	    for(TestChannel = 0; TestChannel < ALL_CHANNEL; TestChannel++)
	    {
            sys_relayOpen(TestChannel);
	        app_adcOnceSetChannel(TestChannel);
	        index = TestChannel * 3 + TestChannel / 5 + 1;
	        SetDisplayString(index, "---");
	        drv_tm1640Display(DisplayBuff);
	        do
	        {
	            sys_delayms(1);
	            value = app_adcGetOnceChannelValue();
	        }
	        while(value == ADC_ONCEING);
	        if(value >= FUSE_VALUE)
	        {
				if(flag)
				{
					SetDisplayNumber(index, value);
				}
				else
				{
					SetDisplayString(index, " ON");
				}
	            FuseState[TestChannel] = TRUE;
	        }
	        else
	        {
				if(flag)
				{
					SetDisplayNumber(index, value);	
				}
				else
				{							 
	        		SetDisplayString(index, "---");	
				}
	            FuseState[TestChannel] = FALSE;
	        }
	        drv_tm1640Display(DisplayBuff);
	    }
        sys_delayms(1000);
	}
	else
	{
		sys_relayClose(ALL_RELAY);
        sys_delayms(500);
    	sys_relayOpen(TEST_RELAY);
        sys_relayOpen(channel);
        sys_delayms(500);
		app_adcOnceSetChannel(channel); 
    	memset(DisplayBuff, ' ', sizeof(DisplayBuff));
        index = channel * 3 + channel / 5 + 1;
        SetDisplayString(index, "---");
        drv_tm1640Display(DisplayBuff);
		do
        {
            sys_delayms(1);
            value = app_adcGetOnceChannelValue();
        }
        while(value == ADC_ONCEING);
    	sys_relayClose(TEST_RELAY);
        if(value >= FUSE_VALUE)
        {
            FuseState[channel] = TRUE;
            return TRUE;
        }
        else
        {
            FuseState[channel] = FALSE;
            return FALSE;
        }
	}
    sys_relayClose(TEST_RELAY);
    return TRUE;
}

#define OVER_CURRENT_TIME       	200     //100ms*200 = 20s
#define OVER_CURRENT_COUNT      	4	//过流保护动作次数

UINT8 OverCurrentChannel = 0;           //过流保护循环检测通道
UINT8 OverCurrentChannelFlag[ALL_CHANNEL];       //过流保护标志
UINT8 OverCurrentCount;                 //过流保护检测累加次数
UINT8 OverCurrentTime;                  //过流保护检测累加时间

void app_testOverCurrentInit(void)
{
    memset(OverCurrentChannelFlag, FALSE, sizeof(OverCurrentChannelFlag));
}
#define AD_CHANNEL_NUM      ((UINT8)10)

#define OVER_STATE_NONE             0x00
#define OVER_STATE_SET_CHANNEL      0x01
#define OVER_STATE_READ_VALUE       0x02

UINT8 OverCurrentTestState = OVER_STATE_NONE;
UINT16 OverCurrentValue;                //过流保护阈值
UINT16 u16_lastADValue[ALL_CHANNEL];

void app_testOverCurrentProtection(void)
{
    UINT16 NowADvalue;                      //AD检测单通道AD值
    OverCurrentValue = (s_SystemSetting.IMaxValue) * 100; //0~9 -> 0~4A

    OverCurrentChannel %= AD_CHANNEL_NUM;
    
    switch(OverCurrentTestState)
    {
        case OVER_STATE_NONE:
        	if(u16_DisplayTime[OverCurrentChannel] == 0 ||		//无电量或通道过流时不再检测过流
        		OverCurrentChannelFlag[OverCurrentChannel])
		    {
				OverCurrentChannel++;
				OverCurrentTime = 0;
				OverCurrentCount = 0;
				return;
		    }
            NowADvalue = app_adcGetValue(OverCurrentChannel);
            if(NowADvalue > OverCurrentValue)
            {
                OverCurrentTestState = OVER_STATE_SET_CHANNEL;
                OverCurrentCount = 0;
                OverCurrentTime = OVER_CURRENT_TIME;
            }
            else
            {
                OverCurrentChannel++;
            }
            break;

        case OVER_STATE_SET_CHANNEL:
            app_adcOnceSetChannel(OverCurrentChannel);
            OverCurrentTestState = OVER_STATE_READ_VALUE;
            break;

        case OVER_STATE_READ_VALUE:
            NowADvalue = app_adcGetOnceChannelValue();
            if(NowADvalue != ADC_ONCEING)
            {
                if(NowADvalue > OverCurrentValue)
                {
                    OverCurrentCount++;
                    if(OverCurrentCount > OVER_CURRENT_COUNT)
                    {
                		OverCurrentChannelFlag[OverCurrentChannel] = TRUE;
//                    	app_timeClear(OverCurrentChannel);
			            OverCurrentTestState = OVER_STATE_NONE;
			            OverCurrentTime = 0;
			            OverCurrentChannel++;
                    }
                    else
                    {
                    	OverCurrentTestState = OVER_STATE_SET_CHANNEL;
                    }
                }
            }
            break;

        default:
            OverCurrentTestState = OVER_STATE_NONE;
            break;
    }

    if(OverCurrentTime)
    {
        OverCurrentTime--;
        if(OverCurrentTime == 0)
        {
            if(OverCurrentCount > OVER_CURRENT_COUNT)
            {
                OverCurrentChannelFlag[OverCurrentChannel] = TRUE;
            }
            else
            {
                OverCurrentChannelFlag[OverCurrentChannel] = FALSE;
            }
            OverCurrentTestState = OVER_STATE_NONE;
            OverCurrentChannel++;
        }
    }
}

void app_testOverCurrentRecoveryHandler1s(void)      //过流状态恢复
{
    UINT8 channel;
    for(channel=0; channel<ALL_CHANNEL; channel++)
    {
        if(OverCurrentChannelFlag[channel])
        {
            if(app_adcGetValue(channel)<OverCurrentValue)
            {
                OverCurrentChannelFlag[channel] = FALSE;
            }
        }
    }
}


BOOL FirstRun = TRUE;
BOOL timeClearFlag = FALSE;
BOOL E2ClearFlag = FALSE;
UINT16 PutInWaitTimeCnt = 0;					//投币等待时间，超时后自动退出
UINT8 SystemSettingMode = MODE_SET_NONE;
UINT16 money = 0;
UINT16 testValue[10] = {0};
void app_testSystemSetUp100ms(void)
{
    UINT8 KEY;
    UINT8 index;
    UINT16 value = 0xFFFF;
    UINT8 i;

    memset(DisplayBuff, ' ', sizeof(DisplayBuff));

    KEY = hwa_keyGet();

    if(app_coinPutIn() != PUT_NONE// ||							//有投币或刷卡
//        (BrushMoneyH || BrushMoneyL)||                          //有返款型刷卡
//    	(FirstRun&&(u16_PutInCardSave+u16_PutInCoinSave)>0)//首次开机
    )
    {
        SystemSettingMode = MODE_PUT_PIN;
		PutInWaitTimeCnt = PUT_IN_WAIT_TIME;
    }
    FirstRun = FALSE;

    switch(SystemSettingMode)
    {
        case MODE_SET_NONE:
            app_timeDisplay100ms();
            if(KEY == KEY_SET)
            {
                sys_relayClose(ALL_RELAY);                      //设置时禁止输出并保存当前时间
                app_timeSaveTime();
                SystemSettingMode = MODE_SET_SYSTEM_PAGE1;
                timeClearFlag = 0;
                E2ClearFlag = 0;
            }
            else if(KEY == KEY_TEST)                            //通道测试
            {
                sys_relayClose(ALL_RELAY);
                SystemSettingMode = MODE_SET_TEST;
                FuseTestChannel = 0xFF;
                memset(testValue, 0x00, sizeof(testValue));
                memset(DisplayBuff, '8', sizeof(DisplayBuff));
            }
            else if(KEY <10)
            {
                if(OverCurrentChannelFlag[KEY] == TRUE)
                {
                    OverCurrentChannelFlag[KEY] = FALSE;
                }
            }
            break;

        case MODE_PUT_PIN:
//            if(BrushMoneyH || BrushMoneyL)
//            {
//                money = BrushMoneyH*s_SystemSetting.CardTimeOrPower + s_SystemSetting.CardTimeOrPower*BrushMoneyL/100;
//            }
//            else
            {
                money = u16_PutInCoinSave + u16_PutInCardSave;
            }
            if(money > 999)
            {
                money = 999;
            }
            for(i = 0; i < 10; i++)
            {
                index = i * 3 + i / 5 + 1;
                if(FuseState[i])
                {
	        		SetDisplayNumber(index, money);
                }
                else
                {
	        		SetDisplayString(index, "---");
                }
            }
	        if(KEY < 10 && app_testFuseTest(KEY) != FALSE)				//有按键按下且保险丝正常
            {
	        	sys_relayOpen(KEY);
	            u16_PutInCoinSave = 0;
	            u16_PutInCardSave = 0;
	            app_testSavePutInMoney();
                app_timeAddMoney(KEY, money);
//                if(BrushMoneyH || BrushMoneyL)
//                {
//                    BrushMoneyH = 0;
//                    BrushMoneyL = 0;
//                }
//                u8_BrushChannel = KEY;
	            PutInWaitTimeCnt = 0;
	            SystemSettingMode = MODE_SET_NONE;
            }
            if(PutInWaitTimeCnt)								//投币超出3min自动退出
            {
            	PutInWaitTimeCnt--;
            	if(PutInWaitTimeCnt==0)
            	{
                    u16_PutInCoinSave = 0;
                    u16_PutInCardSave = 0;
                    app_testSavePutInMoney();
                    SystemSettingMode = MODE_SET_NONE;
            	}
            }
            break;

        case MODE_SET_TEST:                                     //电流测试模式
            if(KEY == KEY_TEST)
            {
                SystemSettingMode = MODE_SET_NONE;
                sys_relayClose(ALL_RELAY);
            }
            else if(KEY != KEY_NONE && KEY != KEY_SET)
            {
                FuseTestChannel = KEY;
                sys_relayClose(ALL_RELAY);
                app_adcOnceSetChannel(FuseTestChannel);
                sys_relayOpen(FuseTestChannel);
            }
            if(FuseTestChannel < 10)
            {
                value = app_adcGetOnceChannelValue();
                if(value != ADC_ONCEING)
                {
                    if(value > 999)
                    {
                        value = 999;
                    }
                    testValue[FuseTestChannel] = value;
                }
            }
            for(i=0; i<10; i++)
            {
				index = i * 3 + i / 5 + 1;
            	if(i==FuseTestChannel)
            	{
            		if(value == ADC_ONCEING)
            		{
	        			SetDisplayString(index, "---");
            		}
            		else
            		{
	        			SetDisplayNumber(index, value*2/5);			//显示电流 100->1.00A
            		}
            	}
            	else
            	{
	        		SetDisplayNumber(index, testValue[i]*2/5);
            	}
            }
            break;

        case MODE_SET_SYSTEM_PAGE1:                                 //系统设置第一页
            DisplayBuff[1] = 'b';                                   //channel 0     显示投币总次数
            DisplayBuff[2] = s_moneySave.CoinCount / 10000 + '0';
            DisplayBuff[3] = s_moneySave.CoinCount / 1000 % 10 + '0';
            DisplayBuff[4] = s_moneySave.CoinCount / 100 % 10 + '0'; //channel 1
            DisplayBuff[5] = s_moneySave.CoinCount / 10 % 10 + '0';
            DisplayBuff[6] = s_moneySave.CoinCount % 10 + '0';

            DisplayBuff[7] = 'c';                                   //channel 2     显示刷卡总次数
            DisplayBuff[8] = s_moneySave.CardCount / 10000 + '0';
            DisplayBuff[9] = s_moneySave.CardCount / 1000 % 10 + '0';
            DisplayBuff[10] = s_moneySave.CardCount / 100 % 10 + '0'; //channel 3
            DisplayBuff[11] = s_moneySave.CardCount / 10 % 10 + '0';
            DisplayBuff[12] = s_moneySave.CardCount % 10 + '0';

			DisplayBuff[17] = 'F';									//channel 5 	设置过流时计量加倍倍数
			DisplayBuff[18] = '-';
			DisplayBuff[19] = s_SystemSetting.OverCurrentMultiple + '0';
			if(KEY == 5)
			{
                s_SystemSetting.OverCurrentMultiple++;
                if(s_SystemSetting.OverCurrentMultiple > 9)
                {
                    s_SystemSetting.OverCurrentMultiple = 1;
                }
			}
            
//			DisplayBuff[17] = 'F';                                  //channel 5     充电电量、时间功能切换
//            DisplayBuff[18] = '-';
//            if(s_SystemSetting.Function == SET_FUN_TIME)
//            {
//                DisplayBuff[19] = '0';
//            }
//            else
//            {
//                DisplayBuff[19] = '1';
//            }
//            if(KEY == 5)
//            {
//                if(s_SystemSetting.Function == SET_FUN_POWER)
//                {
//                    s_SystemSetting.Function = SET_FUN_TIME;
//                }
//                else
//                {
//                    s_SystemSetting.Function = SET_FUN_POWER;
//                }
//                app_timeClear(ALL_CHANNEL);
//            }

            DisplayBuff[20] = 'a';                                  //channel 6     设置电流保护等级
            DisplayBuff[21] = '-';
            DisplayBuff[22] = s_SystemSetting.IMaxValue + '0';
            if(KEY == 6)
            {
                s_SystemSetting.IMaxValue++;
                if(s_SystemSetting.IMaxValue > 9)
                {
                    s_SystemSetting.IMaxValue = 1;
                }
            }
            if(KEY == 7)
            {
                app_testClearMoney();
            }

            DisplayBuff[26] = 'c';                                  //channel 8     清除充电时间
            DisplayBuff[27] = '-';
            if(timeClearFlag == 0)
            {
                DisplayBuff[28] = '1';
                if(KEY == 8)
                {
                    app_timeClear(ALL_CHANNEL);
                    timeClearFlag = 1;
                }
            }
            else
            {
                DisplayBuff[28] = '0';
            }

            DisplayBuff[29] = 'e';                                  //channel 9     恢复出厂设置
            DisplayBuff[30] = '-';
            if(E2ClearFlag == 0)
            {
                DisplayBuff[31] = '1';
                if(KEY == 9)
                {
                    E2ClearFlag = 1;
                    app_testRestoreFactorySetting();
                    SystemSettingMode = MODE_SET_NONE;
                }
            }
            else
            {
                DisplayBuff[31] = '0';
            }
            if(KEY == KEY_SET)
            {
                SystemSettingMode++;
            }
            break;

        case MODE_SET_SYSTEM_PAGE2:                                 //系统设置第二页
	        SetDisplayString(1, "b--");                             //channel 0     设置投币时间
	        SetDisplayNumber(4, s_SystemSetting.CoinTimeOrPower);	//channel 1

            if(KEY == 0)
            {
                if(s_SystemSetting.CoinTimeOrPower < 990)
                {
                    s_SystemSetting.CoinTimeOrPower += 10;
                }
            }
            if(KEY == 1)
            {
                if(s_SystemSetting.CoinTimeOrPower > 10)
                {
                    s_SystemSetting.CoinTimeOrPower -= 10;
                }
            }
			
			SetDisplayString(7, "c--");                            //channel 2 设置刷卡时间
	        SetDisplayNumber(10, s_SystemSetting.CardTimeOrPower); //channel 3

            if(KEY == 2)
            {
                if(s_SystemSetting.CardTimeOrPower < 900)
                {
                    s_SystemSetting.CardTimeOrPower += 10;
                }
            }
            if(KEY == 3)
            {
                if(s_SystemSetting.CardTimeOrPower > 10)
                {
                    s_SystemSetting.CardTimeOrPower -= 10;
                }
            }
			
			SetDisplayString(26, "bc-");
			if(KEY==8)
			{
				s_SystemSetting.BaseCurrent+=25;
				if(s_SystemSetting.BaseCurrent>900)
				{
					s_SystemSetting.BaseCurrent = 900;
				}
			}
			else if(KEY==9)
			{
				s_SystemSetting.BaseCurrent-=25;
				if(s_SystemSetting.BaseCurrent<100)
				{
					s_SystemSetting.BaseCurrent = 100;
				}
			}
			SetDisplayNumber(29, (UINT16)s_SystemSetting.BaseCurrent * 10 / 256);
			
			if(KEY == KEY_SET)
            {
                SystemSettingMode = MODE_SET_NONE;
                app_testSaveSystemSetting();
            }
            break;
    }
    drv_tm1640Display(DisplayBuff);
}


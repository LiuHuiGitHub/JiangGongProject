#include "sys.h"
#include "app_brush.h"
#include "app_test.h"
#include "app_config.h"
#include "app_time.h"
#include "hwa_eeprom.h"
#include "hwa_mifare.h"
#include "sys_eeprom.h"
#include "sys_uart.h"
#include "sys_delay.h"
#include "drive_led.h"
#include "mifare.h"
#include "drive_buzzer.h"
#include "string.h"

code UINT8 PWD_Card[] = { 0xAC, 0x1E, 0x57, 0xAF, 0x19, 0x4E };	//密码卡密码
data UINT8 LastCardId[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

void app_Show(void)
{
	UINT32 Money = s_Money.MoneySum / 100 % 1000000;
	if (b_FactorySystem == FALSE)
	{
		sys_delayms(1000);
		u8_ledDisBuff[0] = (UINT8)(Money / 100000);
		u8_ledDisBuff[1] = (UINT8)(Money / 10000 % 10);
		u8_ledDisBuff[2] = (UINT8)(Money / 1000 % 10);
		u8_ledDisBuff[3] = (UINT8)(Money / 100 % 10);
		u8_ledDisBuff[4] = (UINT8)(Money / 10 % 10);
		u8_ledDisBuff[5] = (UINT8)(Money % 10);
		u8_ledDot = 0;
		sys_delayms(1000);
		drv_ledRequestDisplayChannel0(s_System.Money, 1000, BIT0);
		drv_ledRequestDisplayChannel1(s_System.Time, 1000, 0);
	}
	sys_taskDel(app_Show);
}

void app_brushInit(void)
{
	hwa_mifareInit();
}

BOOL app_BrushGetChannelState(UINT8 channel)					//获取通道是否占用
{
	UINT8 i;
	for (i = 0; i < 5; i++)
	{
		if (s_Money.Card_ID[channel][i] != 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}
UINT8 app_BrushGetNoUseChannel(void)							//获取通道当前未使用的一个通道
{
	UINT8 channel;
	for (channel = 0; channel < CHANNEL_NUMBER; channel++)
	{
		if (app_BrushGetChannelState(channel) == FALSE)
		{
			return channel;
		}
	}
	return CHANNEL_NUMBER;
}

UINT8 app_BrushGetSurplusChannelNum(void)						//获取剩余通道数量
{
	UINT8 SurplusChannelNum = CHANNEL_NUMBER;
	UINT8 channel;
	for (channel = 0; channel < CHANNEL_NUMBER; channel++)
	{
		if (app_BrushGetChannelState(channel))
		{
			SurplusChannelNum--;
		}
	}
	return SurplusChannelNum;
}

void app_brushBrush(UINT8 channel)				//往channel加钱
{
	if (channel < CHANNEL_NUMBER)
	{
		memcpy(s_Money.Card_ID[channel], LastCardId, 5);
		app_configWrite(MONEY_SECTOR);							//累计营业额，保存卡号
		app_timeAddTime(channel);
		drv_buzzerNumber(1);
	}
	else
	{
		drv_buzzerNumber(2);
	}
	drv_ledRequestDisplayChannel0(0, 0, 0);
	drv_ledRequestDisplayChannel1(0, 0, 0);
}

BOOL app_brushGetChannelIdState(UINT8 channel)					//获取该通道是否被占用
{
	if (memcmp(s_Money.Card_ID[channel], gCard_UID, 5) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

#define NO_CHANNEL       CHANNEL_NUMBER
UINT8 ReturnCardId(void)
{
	UINT8 channel;
	for (channel = 0; channel < CHANNEL_NUMBER; channel++)
	{
		if (app_brushGetChannelIdState(channel))
		{
			return channel;
		}
	}
	return NO_CHANNEL;
}

/*搜索卡，返回卡片类型，
	0->无卡
	1->管理卡
	2->密码卡
	3->用户卡
	4->错误卡
	*/
#define NONE_CARD       0
#define MEM_CARD        1
#define USER_CARD       2
#define PWD_CARD        3

UINT8 app_brushCard(void)
{
	UINT8 Sector;
	UINT8 CardIndex;
    UINT8 i;
	for (CardIndex = MEM_CARD; CardIndex <= PWD_CARD; CardIndex++)
	{
		if (b_FactorySystem)
		{
			CardIndex = PWD_CARD;
		}
		if (CardIndex == MEM_CARD)
		{
			Load_Key(&s_System.MGM_Card);
		}
		else if (CardIndex == USER_CARD)
		{
			Load_Key(&s_System.USER_Card);
		}
		else if (CardIndex == PWD_CARD)
		{
			Load_Key(PWD_Card);
		}
		MIF_Halt();
		if (Request(RF_CMD_REQUEST_STD) != FM1702_OK)
		{
			continue;
		}
        for(i=0; i<2; i++)
        {
            if (AntiColl() == FM1702_OK && SelectCard() == FM1702_OK)
            {
                if (CardIndex == USER_CARD)     //用户卡验证钱所在扇区
                {
                    Sector = s_System.Sector;
                }
                else                            //管理和密码卡验证1扇区
                {
                    Sector = 1;
                }
                if (Authentication(gCard_UID, Sector, 0x60) == FM1702_OK)
                {
                    return CardIndex;
                }
            }
        }
	}
	return NONE_CARD;
}

void app_brushMemSetting(void)
{
	BOOL flag = FALSE;
#define U8_FIRST_BRUSH_CARD_DLY     5
	UINT8 u8_FirstBrushCardDly = 0;			//第一次刷管理卡显示信息，再次刷则更改

	app_timeClear(AD_CHANNEL_NUM);          //清除时间

	do
	{
		if (app_brushCard() == MEM_CARD && Read_Block(gBuff, 4) == FM1702_OK)
		{
			if (gBuff[0] == 0x01 && gBuff[1] == 0x0A)			//金额管理卡
			{
				if (u8_FirstBrushCardDly)
				{
					s_System.Money += 10;
					if (s_System.Money > 200)
					{
						s_System.Money = 10;
					}
					flag = TRUE;
				}
				drv_ledDisplayChannel(0, s_System.Money);
				drv_ledDisplayChannel(1, 0);
				u8_ledDot = 1 << 0;
			}
			else if (gBuff[0] == 0xFA && gBuff[1] == 0x01)		//时间管理卡
			{
				if (u8_FirstBrushCardDly)
				{
					s_System.Time += 30;
					if (s_System.Time > 600)
					{
						s_System.Time = 30;
					}
					flag = TRUE;
				}
				drv_ledDisplayChannel(0, 0);
				drv_ledDisplayChannel(1, s_System.Time);
			}
			else if (gBuff[0] == 0xFF && gBuff[1] == 0xFF)
			{
				if (b_TestMode)
				{
					b_TestMode = FALSE;
				}
				else
				{
					b_TestMode = TRUE;
				}
				drv_buzzerNumber(1);
				break;
			}
			drv_buzzerNumber(1);
			u8_FirstBrushCardDly = 1;
		}
		sys_delayms(1000);
		u8_FirstBrushCardDly++;
	} while (u8_FirstBrushCardDly < U8_FIRST_BRUSH_CARD_DLY);

	if (flag)
	{
		app_configWrite(SYSTEM_SETTING_SECTOR);
	}
}

void app_brushCycle500ms(void)
{
	UINT8 channel;
	UINT16 Money;
#define BRUSH_SEL_CHANNEL_TIME             120
	static UINT8 u8_BrushSelChannelTime = BRUSH_SEL_CHANNEL_TIME;

	switch (app_brushCard())
	{
	case MEM_CARD:
		app_brushMemSetting();
		break;

	case PWD_CARD:										//从初始卡中读取管理卡密码，并储存至E2
		if (hwa_mifareReadBlock(gBuff, 4))			//读取管理卡和用户卡密码以及扇区
		{
			memcpy(&s_System, gBuff, 16);
			s_System.Refund++;
			s_System.Refund &= 0x01;
            if (hwa_mifareReadBlock(gBuff, 5))			//读取管理卡和用户卡密码以及扇区
            {
                if(gBuff[0] == 0x01)
                {
                    s_System.RecoveryOldCard = 1;
                }
                else
                {
                    s_System.RecoveryOldCard = 0;
                }
            }
            else
            {
                break;
            }
			app_configWrite(SYSTEM_SETTING_SECTOR);
			app_timeClear(AD_CHANNEL_NUM);              //清除时间
			drv_ledRequestDisplayChannel0(s_System.Sector, 2000, 0);
			drv_ledRequestDisplayChannel1(s_System.Refund, 2000, 0);
			drv_buzzerNumber(1);
			b_FactorySystem = FALSE;
			b_TestMode = FALSE;
		}
		break;

	case USER_CARD:
		//memset(gBuff, 0x00, sizeof(gBuff));
		//pMoney->money = 20000;										//充钱
		//if (hwa_mifareWriteSector(gBuff, s_System.Sector))
		//{
		//	drv_buzzerNumber(1);
		//}
		//break;

		if (hwa_mifareReadSector(gBuff, s_System.Sector))
		{
			channel = ReturnCardId();

			if (memcmp(LastCardId, gCard_UID, 5))                           //与上次刷卡不同，清除刷卡时间
			{
				memcpy(LastCardId, gCard_UID, 5);
				u8_BrushSelChannelTime = 0;
			}
			if (channel == NO_CHANNEL									    //新卡
				|| (s_System.Refund && u8_BrushSelChannelTime)				//返款型重复刷卡
				|| (!s_System.Refund)										//非返款型重复刷卡
				)
			{
				if (channel == NO_CHANNEL && app_BrushGetSurplusChannelNum() == 0)     //通道全部占用，不允许刷卡扣款
				{
					drv_buzzerNumber(2);
					break;
				}
				if (pMoney->money >= s_System.Money)						//确保余额充足
				{
					pMoney->money -= s_System.Money;
					if (hwa_mifareWriteSector(gBuff, s_System.Sector))
					{
						s_Money.MoneySum += s_System.Money;					//累计营业额

						if (channel == NO_CHANNEL)						    //新卡，自动选择未使用的通道
						{
							app_brushBrush(app_BrushGetNoUseChannel());
						}
						else												//直接加钱
						{
							app_timeAddTime(channel);
							app_configWrite(MONEY_SECTOR);				//保存营业额
						}
						drv_buzzerNumber(1);
						drv_ledRequestDisplayChannel0(pMoney->money / 100, 2000, BIT2);		//显示余额
						drv_ledRequestDisplayChannel1(pMoney->money % 100 * 10, 2000, 0);
						if (s_System.Refund)
						{
							u8_BrushSelChannelTime = BRUSH_SEL_CHANNEL_TIME;
						}
						break;
					}
				}
				else
				{
					drv_buzzerNumber(3);
					break;
				}
			}
			else															//返款
			{
                Money = app_timeRefundMoney(channel, &pMoney->money);
				if (hwa_mifareWriteSector(gBuff, s_System.Sector))
				{
					app_timeClear(channel);
                    if (s_Money.MoneySum > Money)
					{
                        s_Money.MoneySum -= Money;           //累计营业额
					}
					app_configWrite(MONEY_SECTOR);				//保存营业额
					drv_buzzerNumber(1);
					drv_ledRequestDisplayChannel0(pMoney->money / 100, 3000, BIT2);		//显示余额
					drv_ledRequestDisplayChannel1(pMoney->money % 100 * 10, 3000, 0);
					sys_delayms(2000);
				}
				break;
			}
		}
		break;

	default:
		break;
	}


	if (u8_BrushSelChannelTime)
	{
		u8_BrushSelChannelTime--;
	}
}


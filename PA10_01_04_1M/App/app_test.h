#ifndef __APP_TEST_H__
#define __APP_TEST_H__

#include "typedef.h"

#define SYSTEM_SET_SECTOR       0
#define MONEY_SAVE_SECTOR		1

#define SET_NOREFUND			(0x00)
#define SET_REFUND				(0x01)

#define SET_FUN_TIME			(0x00)
#define SET_FUN_POWER			(0x01)

#define FUSE_ALL_CHANNEL		0xFF

#define FUSE_VALUE              ((UINT8)20)     		//保险丝检测AD阈值 实测59

#define PUT_IN_WAIT_TIME		((UINT16)1800)//100ms*10*60*3 = 3min

#define MODE_SET_NONE           0x00
#define MODE_SET_SYSTEM_PAGE1   0x01
#define MODE_SET_SYSTEM_PAGE2   0x02
#define MODE_SET_TEST           0x03
#define MODE_PUT_PIN            0x04

typedef struct
{
    UINT16 CoinTimeOrPower;
    UINT16 CardTimeOrPower;
    UINT8 IMaxValue;				//0~9	->0~4A
    UINT8 OverCurrentMultiple;		//1~9    ->过流时，电量累加基本电流的倍数1~9倍
    UINT8 Function;					//0x00:time 0x01:power
    UINT16 BaseCurrent;				//基本电流设置，AD值小于等于该值时按该值计算，大于该值时成倍计量，
}SYSTEM_SETTINGS_STRUCT;

typedef struct
{
    UINT32 CoinCount;
    UINT32 CardCount;
    UINT16 PutInCoinSave;
    UINT16 PutInCardSave;
}MONEY_SAVE_STRUCT;

extern SYSTEM_SETTINGS_STRUCT s_SystemSetting;
extern MONEY_SAVE_STRUCT s_moneySave;
extern UINT8 OverCurrentChannelFlag[10];
extern UINT8 FuseState[10];

void app_testInit(void);
void app_testVersion(void);
BOOL app_testFuseTest(UINT8 channel);
void app_testClearMoney(void);
void app_testSaveMoney(void);
void app_testSavePutInMoney(void);
void app_testSystemSetUp100ms(void);
void app_testRestoreFactorySetting(void);
void app_testOverCurrentInit(void);
void app_testOverCurrentProtection(void);
void app_testOverCurrentRecoveryHandler1s(void);

void app_testLowCurrentCloseHandler1s(void);

#endif

#include "include.h"
#include "app_config.h"

static BOOL b_keyStartOld = FALSE;
static BOOL b_keyTimeSetOld = FALSE;
static BOOL b_keyTempSetOld = FALSE;

static UINT16 u16_workTimeCounter = 0;

enum
{
	NTC_NORMAL,
	NTC_OVER_TEMP,
	NTC_OPEN_CIRCUIT,
	NTC_SHORT_CIRCUIT,
}e_ntcState = NTC_NORMAL;

enum
{
	MODE_POWER_ON,
	MODE_STANDBY,
	MODE_WORK,
	MODE_TIME_SET,
	MODE_TEMP_SET,
	MODE_AGING_TEST,
	MODE_FAULT,
}e_mode = MODE_POWER_ON, e_modeOld = MODE_STANDBY;

enum
{
	WORK_OFF,
	WORK_FULL,
	WORK_VVVF,
	WORK_VVVF_OFF,
}e_workState = WORK_OFF;

enum
{
	KEY_EVENT_NONE,
	KEY_EVENT_START,
	KEY_EVENT_TIME_SET,
	KEY_EVENT_TEMP_SET,
}e_keyEvent = KEY_EVENT_NONE;

void app_testInit(void)
{
}

void eventHandler(void)
{
	if(b_keyStartOld != b_keyStart)
	{
		b_keyStartOld = b_keyStart;
		if(b_keyStart)
		{
			e_keyEvent = KEY_EVENT_START;
		}
	}
	else if(b_keyTimeSetOld != b_keyTimeSet)
	{
		b_keyTimeSetOld = b_keyTimeSet;
		if(b_keyTimeSet)
		{
			e_keyEvent = KEY_EVENT_TIME_SET;
		}
	}
	else if(b_keyTempSetOld != b_keyTempSet)
	{
		b_keyTempSetOld = b_keyTempSet;
		if(b_keyTempSet)
		{
			e_keyEvent = KEY_EVENT_TEMP_SET;
		}
	}
}

UINT8 u8_workTime = 0;
//
#define TIME_45_MIN			(45*60*10)
#define TIME_15_MIN			(15*60*10)
#define WORK_TIME			((UINT16)u8_workTime*60*10)

//#define TIME_45_MIN			(45*10)
//#define TIME_15_MIN			(15*10)
//#define WORK_TIME			((UINT16)u8_workTime*10)

void app_testHandler100ms(void)
{
	static UINT8 u8_powerOnDelay = 5;
	static UINT8 u8_keyLongEnterCounter = 0;
	static UINT8 u8_keyEnterTimeCounter = 0;
	static UINT8 b_settingSaveFlag = FALSE;
	static UINT16 u16_agingTestTimeCounter = TIME_45_MIN;
	static BOOL b_agingTestFlag = 0;
	UINT8 temp = hwa_ntcGetTemp();
	eventHandler();

	switch(e_mode)
	{
		case MODE_POWER_ON:
			if(u8_powerOnDelay)
			{
				u8_powerOnDelay--;
				if(u8_powerOnDelay == 0)
				{
					if(b_settingSaveFlag)
					{
						b_settingSaveFlag = FALSE;
						e_mode = MODE_AGING_TEST;
					}
					else
					{
						e_mode = MODE_STANDBY;
					}
				}
				else if(b_keyTimeSet && b_keyTempSet && !b_settingSaveFlag)
				{
					u8_powerOnDelay = 30;
					u8_ledDisBuff[0] = 0;	//O
					u8_ledDisBuff[1] = 1;	//1
					b_settingSaveFlag = TRUE;
				}
			}
			break;
			
		case MODE_STANDBY:
			drv_scrRequest(SCR_OFF);
			u8_ledDisBuff[0] = 0;	//O
			u8_ledDisBuff[1] = 15;	//F
			if(e_keyEvent == KEY_EVENT_START)
			{
				drv_ledRequest(3, u8_setTemp);
				e_mode = MODE_WORK;
				u16_workTimeCounter = 0;
				u8_workTime = s_System.Time;
			}
			
			if(temp == 0)
			{
				e_mode = MODE_FAULT;
				e_ntcState = NTC_SHORT_CIRCUIT;
			}
			else if(temp == 99)
			{
				e_mode = MODE_FAULT;
				e_ntcState = NTC_OPEN_CIRCUIT;
			}
			else if(temp >= 50)
			{
				e_mode = MODE_FAULT;
				e_ntcState = NTC_OVER_TEMP;
			}
			break;
			
		case MODE_WORK:
			u8_ledDisBuff[0] = temp/10;
			u8_ledDisBuff[1] = temp%10;
			switch(e_workState)
			{
				case WORK_OFF:
					drv_scrRequest(SCR_OFF);
					if(temp < u8_setTemp)
					{
						e_workState = WORK_FULL;
					}
					break;
					
				case WORK_FULL:
					drv_scrRequest(SCR_FULL);
					if(temp >= u8_setTemp)
					{
						e_workState = WORK_VVVF;
					}
					break;
					
				case WORK_VVVF:
					drv_scrRequest(SCR_VVVF);
					if(temp <= u8_setTemp-2)
					{
						e_workState = WORK_FULL;
					}
					else if(temp >= u8_setTemp)
					{
						e_workState = WORK_VVVF_OFF;
					}
					break;
					
				case WORK_VVVF_OFF:
					drv_scrRequest(SCR_VVVF_OFF);
					if(temp < u8_setTemp)
					{
						e_workState = WORK_VVVF;
					}
					break;
					
				default:
					e_workState = WORK_OFF;
					break;
			}
			
			if(e_keyEvent == KEY_EVENT_START)
			{
				e_mode = MODE_STANDBY;
			}
			if(e_keyEvent == KEY_EVENT_TIME_SET)
			{
				drv_ledRequest(3, u8_setTime);
				e_modeOld = e_mode;
				e_mode = MODE_TIME_SET;
			}
			else if(e_keyEvent == KEY_EVENT_TEMP_SET)
			{
				drv_ledRequest(3, u8_setTemp);
				e_modeOld = e_mode;
				e_mode = MODE_TEMP_SET;
			}
			
			if(temp == 0)
			{
				e_mode = MODE_FAULT;
				e_ntcState = NTC_SHORT_CIRCUIT;
			}
			else if(temp == 99)
			{
				e_mode = MODE_FAULT;
				e_ntcState = NTC_OPEN_CIRCUIT;
			}
			else if(temp >= 50)
			{
				e_mode = MODE_FAULT;
				e_ntcState = NTC_OVER_TEMP;
			}
			
			if(++u16_workTimeCounter >= WORK_TIME)
			{
				e_mode = MODE_STANDBY;
			}
			break;
			
		case MODE_TIME_SET:
			if(e_keyEvent == KEY_EVENT_TIME_SET)
			{
				b_settingSaveFlag = TRUE;
			}
			else if(e_keyEvent == KEY_EVENT_TEMP_SET)		//时间设置时按温度保存当前时间
			{
				u8_keyLongEnterCounter = 0;
				u8_keyEnterTimeCounter = 0;
				e_mode = MODE_TEMP_SET;
				u8_workTime = u8_setTime;
				if(u8_setTime >= 30)
				{
					s_System.Time = u8_setTime;
					app_configWrite();
				}
				drv_ledRequest(3, u8_setTemp);
			}
			else if(b_keyTimeSet)
			{
				if(u8_keyLongEnterCounter < 20)
				{
					u8_keyLongEnterCounter++;
				}
				else
				{
					if(u8_keyEnterTimeCounter < 5)
					{
						u8_keyEnterTimeCounter++;
					}
					else
					{
						u8_keyEnterTimeCounter = 0;
						b_settingSaveFlag = TRUE;
					}
				}
			}
			else
			{
				u8_keyLongEnterCounter = 0;
				u8_keyEnterTimeCounter = 0;
			}
			
			if(b_settingSaveFlag == TRUE)
			{
				b_settingSaveFlag = FALSE;
				u8_setTime+=5;
				if(u8_setTime > 90)
				{
					u8_setTime = 10;
				}
				drv_ledRequest(3, u8_setTime);
			}
			else if(drv_ledGetRequest() == 0)
			{
				u8_workTime = u8_setTime;
				if(u8_setTime >= 30)
				{
					s_System.Time = u8_setTime;
					app_configWrite();
				}
				e_mode = e_modeOld;
			}
			
			if(++u16_workTimeCounter >= WORK_TIME)
			{
				e_mode = MODE_STANDBY;
			}
			break;
			
		case MODE_TEMP_SET:
			if(e_keyEvent == KEY_EVENT_TEMP_SET)
			{
				b_settingSaveFlag = TRUE;
			}
			else if(e_keyEvent == KEY_EVENT_TIME_SET)		//温度设置时按时间保存当前温度
			{
				u8_keyLongEnterCounter = 0;
				u8_keyEnterTimeCounter = 0;
				e_mode = MODE_TIME_SET;
				s_System.Temp = u8_setTemp;
				app_configWrite();
				drv_ledRequest(3, u8_setTime);
			}
			else if(b_keyTempSet)
			{
				if(u8_keyLongEnterCounter < 20)
				{
					u8_keyLongEnterCounter++;
				}
				else
				{
					if(u8_keyEnterTimeCounter < 5)
					{
						u8_keyEnterTimeCounter++;
					}
					else
					{
						u8_keyEnterTimeCounter = 0;
						b_settingSaveFlag = TRUE;
					}
				}
			}
			else
			{
				u8_keyLongEnterCounter = 0;
				u8_keyEnterTimeCounter = 0;
			}
			
			if(b_settingSaveFlag == TRUE)
			{
				b_settingSaveFlag = FALSE;
				u8_setTemp+=1;
				if(u8_setTemp > 48)
				{
					u8_setTemp = 30;
				}
				drv_ledRequest(3, u8_setTemp);
			}
			else if(drv_ledGetRequest() == 0)
			{
				s_System.Temp = u8_setTemp;
				app_configWrite();
				e_mode = e_modeOld;
			}
			
			if(++u16_workTimeCounter >= WORK_TIME)
			{
				e_mode = MODE_STANDBY;
			}
			break;
		
		case MODE_AGING_TEST:
			if(b_agingTestFlag == 0)
			{
				if(u16_agingTestTimeCounter)
				{
					u16_agingTestTimeCounter--;
					if(u16_agingTestTimeCounter == 0)
					{
						b_agingTestFlag = 1;
						u16_agingTestTimeCounter = TIME_15_MIN;
					}
				}
			}
			else
			{
				if(u16_agingTestTimeCounter)
				{
					u16_agingTestTimeCounter--;
					if(u16_agingTestTimeCounter == 0)
					{
						b_agingTestFlag = 0;
						u16_agingTestTimeCounter = TIME_45_MIN;
					}
				}
			}
			if(temp == 0)
			{
				drv_ledRequest(0xFF, 0xE1);
				drv_scrRequest(SCR_TEST_OFF);
			}
			else if(temp == 99)
			{
				drv_ledRequest(0xFF, 0xE2);
				drv_scrRequest(SCR_TEST_OFF);
			}
			else if(temp >= 50)
			{
				drv_ledRequest(0xFF, 0xE3);
				drv_scrRequest(SCR_TEST_OFF);
			}
			else
			{
				if(b_agingTestFlag == 0)
				{
					drv_scrRequest(SCR_TEST_FULL);
				}
				else
				{
					drv_scrRequest(SCR_TEST_OFF);
				}
				drv_ledRequest(0xFF, temp);
			}
			if(e_keyEvent == KEY_EVENT_START)
			{
				drv_ledRequest(0, temp);
				e_mode = MODE_STANDBY;
			}
			break;
			
		case MODE_FAULT:
			drv_scrRequest(SCR_OFF);
			switch(e_ntcState)
			{
				case NTC_NORMAL:
					e_mode = MODE_STANDBY;
					drv_ledRequest(0, 0xE3);
					break;
					
				case NTC_SHORT_CIRCUIT:
					drv_ledRequest(0xFF, 0xE1);
					if(temp > 0)
					{
						e_ntcState = NTC_NORMAL;
					}
					break;
					
				case NTC_OPEN_CIRCUIT:
					if(temp < 99)
					{
						e_ntcState = NTC_NORMAL;
					}
					drv_ledRequest(0xFF, 0xE2);
					break;
					
				case NTC_OVER_TEMP:
					drv_ledRequest(0xFF, 0xE3);
					if(temp < 49)
					{
						e_ntcState = NTC_NORMAL;
					}
					break;
				
				default:
					e_ntcState = NTC_NORMAL;
					break;
			}
			break;
			
		default:
			e_mode = MODE_STANDBY;
			break;
	}
	e_keyEvent = KEY_EVENT_NONE;
}


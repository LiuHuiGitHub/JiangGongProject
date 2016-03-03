#include "STC15F2K60S2.h"
#include "sys_relay.h"
#include "sys_uart.h"
#include "drive_tm1640.h"
#include "drive_tm1650.h"
#include "sys_delay.h"
#include "hwa_key.h"
#include "hwa_eeprom.h"
#include "hwa_coin.h"
#include "app_adc.h"
#include "app_test.h"
#include "app_time.h"
#include "stdio.h"

UINT8 taskCycleCnt10ms = 0;
UINT8 taskCycleCnt100ms = 0;
UINT8 taskCycleCnt1s = 0;
UINT8 taskCycleCnt60s = 0;
UINT8 taskCycleCnt10min = 0;

BOOL taskCycle1msFlag = FALSE;
BOOL taskCycle10msFlag = FALSE;
BOOL taskCycle100msFlag = FALSE;
BOOL taskCycle1sFlag = FALSE;
BOOL taskCycle60sFalg = FALSE;
BOOL taskCycle10minFlag = FALSE;

void sys_tim0Init(void)
{
    AUXR |= 0x80;
    TMOD = 0x00;
    TL0 = 0xCD;
    TH0 = 0xD4;
    TR0 = 0;
    ET0 = 1;
    TR0 = 1;
    EA = 1;
}
void sys_gpioInit(void)
{
    P1M1 &= ~0x3C;      //推挽输出
    P1M0 |= 0x3C;
    P2M1 &= ~0xFF;      //推挽输出
    P2M0 |= 0xFF;
    P2 = 0x00;
    P3M1 &= ~0xFC;      //推挽输出
    P3M0 |= 0xFC;
    P3 = 0x03;
//    while(1)
//    {
//	    sys_relayOpen(ALL_RELAY);
//	    sys_delayms(5000);
//	    sys_relayClose(ALL_RELAY);
//	    sys_delayms(5000);
//    }
}

void sys_taskInit(void)
{
    sys_gpioInit();
    hwa_eepromInit();
    app_testInit();
    app_timeInit();
    app_testVersion();
    app_adcInit();
    sys_tim0Init();
    app_testFuseTest(FUSE_ALL_CHANNEL);
    app_testOverCurrentInit();
}

void sys_tim0Isr(void) interrupt 1      //1ms cycle task
{
	taskCycle1msFlag = TRUE;
    app_adcHandler1ms();
	hwa_coinHandler1ms();
}

void sys_taskHandler10ms(void)
{
}

void sys_taskHandler100ms(void)
{
//    static UINT8 Buff[16];
//    sprintf(Buff, "%.6d", (UINT16)drv_tm1650GetKey());
//    drv_tm1640Display(Buff);
    app_testSystemSetUp100ms();
    app_testOverCurrentProtection();
}

void sys_taskHandler1s(void)
{
    app_testOverCurrentRecoveryHandler1s();
    app_testLowCurrentCloseHandler1s();
}
void sys_taskHandler1min(void)
{
	app_timePower1min();		//1min累计一次电量
}
void sys_taskHandler10min(void)
{
	app_timeSaveTime();			//剩余时间10分钟保存一次
}
void main(void)
{
    sys_taskInit();
    while(1)
    {
        WDT_CONTR |= 0x35;          //reset watch dog       max time 2.27s
        if(taskCycle1msFlag)
        {
        	taskCycle1msFlag = FALSE;
		    taskCycleCnt10ms++;
		    if(taskCycleCnt10ms >= 10)
		    {
		    	taskCycleCnt10ms = 0;
		    	taskCycle10msFlag = TRUE;
		    	taskCycleCnt100ms++;
		    }
			if(taskCycleCnt100ms>=10)
			{
				taskCycleCnt100ms = 0;
				taskCycle100msFlag = TRUE;
				taskCycleCnt1s++;
			}
			if(taskCycleCnt1s>=10)
			{
				taskCycleCnt1s = 0;
				taskCycle1sFlag = TRUE;
            	taskCycleCnt60s++;
			}
			if(taskCycleCnt60s>=60)
			{
				taskCycleCnt60s = 0;
				taskCycle60sFalg = TRUE;
				taskCycleCnt10min++;
			}
			if(taskCycleCnt10min>=10)
			{
				taskCycleCnt10min = 0;
				taskCycle10minFlag = TRUE;
			}
        }
        if(taskCycle10msFlag)
        {
            taskCycle10msFlag = FALSE;
            sys_taskHandler10ms();
        }
        if(taskCycle100msFlag)
        {
            taskCycle100msFlag = FALSE;
            sys_taskHandler100ms();
        }
        if(taskCycle1sFlag)
        {
            taskCycle1sFlag = FALSE;
            sys_taskHandler1s();
        }
        if(taskCycle60sFalg)
        {
        	taskCycle60sFalg = FALSE;
        	sys_taskHandler1min();
        }
        if(taskCycle10minFlag)
        {
        	taskCycle10minFlag = FALSE;
            sys_taskHandler10min();
        }
    }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
//    UINT8 s[30];
//    sprintf(s, "%s,%ld\n", file, line);
//    sys_uartSendString(s);
	while(1);
}
#endif

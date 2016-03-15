#include "sys_conf.h"
#include "stc15f2k60s2.h"
#include "drive_buzzer.h"
#include "drive_relay.h"
#include "drive_led.h"
#include "sys_uart.h"
#include "app_config.h"
#include "app_time.h"
#include "hwa_uart.h"
#include "app_test.h"
#include "stdio.h"

UINT8 taskCycleCnt500ms = 0;
UINT8 taskCycleCnt60s = 0;
BOOL taskCycle1msFlag = FALSE;
BOOL taskCycle10msFlag = FALSE;

void sys_taskInit(void)
{
    drv_buzzerInit();
	drv_relayInit();
	drv_ledInit();
    hwa_uartInit();
    app_testInit();
	app_configInit();
	app_timeLoad();
}

#define T1MS (65536-FOSC/12/1000) //12T

void sys_timeInit(void)
{
//	DISABLE_INTERRUPT();
	TMOD &= 0xF0;
	TH0 = (UINT8)(T1MS>>8);
	TL0 = (UINT8)T1MS;
	ET0 = 1;
	TR0 = 1;
//	PT1 = 1;
    EA = 1;
}

void sys_tim0Isr(void) interrupt 1      //1ms cycle task
{
    static UINT8 count = 0;
    drv_ledHandler1ms();
    app_testHandler1ms();
    hwa_uartHandler1ms();
    if(++count>=10)
    {
        drv_buzzerHandler10ms();
        taskCycle10msFlag = TRUE;
        count = 0;
    }
    taskCycle1msFlag = TRUE;
}

void main(void)
{
    sys_taskInit();
    sys_timeInit();
	drv_buzzerNumber(1);
    app_configDisplay();
    while(1)
    {
        WDT_CONTR |= 0x35;          //reset watch dog       max time 2.27s
        if(taskCycle1msFlag)
        {
        	taskCycle1msFlag = FALSE;
        }
        if(taskCycle10msFlag)
        {
            taskCycle10msFlag = FALSE;
            hwa_uartHandler10ms();
            taskCycleCnt500ms++;
			if(taskCycleCnt500ms>=50)
			{
				taskCycleCnt500ms = 0;
                app_timeDisplay500ms();
				taskCycleCnt60s++;
			}
			if(taskCycleCnt60s>=120)
			{
				taskCycleCnt60s = 0;
                app_timePower1min();
			}
        }
    }
}

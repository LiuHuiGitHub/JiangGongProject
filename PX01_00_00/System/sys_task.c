#include "include.h"

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

#define T1MS (65536-FOSC/12/1000) //12T

void sys_tim0Init(void)
{
	TMOD = 0x00;
	TH1 = (UINT8)(T1MS>>8);
	TL1 = (UINT8)T1MS;
	ET1 = 1;
	TR1 = 1;
//	PT1 = 1;
    EA = 1;
}

void sys_gpioInit(void)
{
//    P1M1 &= ~0x3C;      //推挽输出
//    P1M0 |= 0x3C;
//    P2M1 &= ~0xFF;      //推挽输出
//    P2M0 |= 0xFF;
//    P2 = 0x00;
//    P3M1 &= ~0xFC;      //推挽输出
//    P3M0 |= 0xFC;
//    P3 = 0x03;
}

void sys_taskInit(void)
{
    sys_gpioInit();
    sys_tim0Init();
    sys_uartInit();
    sys_ccapInit();
    sys_ccapEnable();
}

static UINT8 flag = 0;
void sys_tim1Isr(void) interrupt 3      //1ms cycle task
{
	taskCycle1msFlag = TRUE;
    if(++flag == 5)
    {
        P26 = 0;
    }
    else if(flag == 20)
    {
        flag = 0;
        P26 = 1;
    }
}

void sys_taskHandler10ms(void)
{
}

void sys_taskHandler100ms(void)
{
    static UINT8 Buff[16];
    sprintf(Buff, "%ld,%ld\r\n", u32_PulseCycle*625/1152, u32_PulseWidth*625/1152);
    sys_uartSendString(Buff);
}

void sys_taskHandler1s(void)
{
}
void sys_taskHandler1min(void)
{
}
void sys_taskHandler10min(void)
{
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

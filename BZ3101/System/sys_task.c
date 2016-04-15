#include "iostm8s103f3.h"
#include "include.h"

static BOOL b_taskHandler2ms = FALSE;
static BOOL b_taskCycle100ms = FALSE;

static UINT8 u8_taskCycleCount100ms = 0;
static UINT8 u8_taskCycleCount500ms = 0;

static void sys_taskHandler2ms(void);
static void sys_taskCycle100ms(void);
static void sys_taskCycle500ms(void);

void sys_taskGpioInit(void)
{
    PA_DDR |= 0x0F;
    PA_CR1 |= 0x0F;
    PA_CR2 &= (UINT8)(~0x0F);
	PA_ODR = 0x00;
	
    PC_DDR |= 0xFF;
    PC_CR1 |= 0xFF;
    PC_CR2 &= (UINT8)(~0xFF);
	PC_ODR = 0x00;
	
	PD_DDR_DDR3 = 0;
	PD_CR1_C13 = 0;
	PD_CR2_C23 = 0;
	
	PD_DDR |= 0xF6;
	PD_CR1 |= 0xF6;
	PD_CR2 &= (UINT8)(~0xF6);
	PD_ODR = 0x00;
}

void sys_taskInit(void)
{
    CLK_PCKENR1 |= 0x01;
    TIM4_PSCR = 0x07;				//TIM4 prescaler is 128, TIM4 clock is 125KHz
//    TIM4_ARR = 125;					//TIM4 Handler 1ms
	TIM4_ARR = 250;					//TIM4 Handler 2ms
    TIM4_EGR = 0x01;				//enable update
    TIM4_IER = 0x01;				//enable update interrupt
    TIM4_CR1 = 0x01;				//enable TIM4
}

#pragma vector = TIM4_UPD_OVF_IRQ
__interrupt void TIM4_UPD_OVF_IRQHandler(void)
{
    TIM4_SR &= ~0x01;
    b_taskHandler2ms = TRUE;
    sys_taskHandler2ms();
}

void sys_taskHandler2ms(void)
{
	drv_ledHandler2ms();
	//drv_scrHandler2ms();
	drv_ledBreathing();
}

int main(void)
{
    CLK_CKDIVR = 0x00;				//cpu clock is HSI 16MHz
    sys_taskGpioInit();
	drv_scrInit();
	drv_ledInit();
	sys_pwmInit();
	sys_iwdgInit();
    sys_taskInit();
	hwa_ntcInit();
    app_configInit();
	app_testInit();
	enable_interrupt();
    while(1)
    {
        sys_iwdgReset();
        if(b_taskHandler2ms)
        {
            b_taskHandler2ms = FALSE;
            if(++u8_taskCycleCount100ms >= 50)
            {
                u8_taskCycleCount100ms = 0;
                b_taskCycle100ms = TRUE;
            }
        }
        if(b_taskCycle100ms)
        {
            b_taskCycle100ms = FALSE;
            sys_taskCycle100ms();
			if(++u8_taskCycleCount500ms >= 5)
			{
				u8_taskCycleCount500ms = 0;
				sys_taskCycle500ms();
			}
        }
		sleep();
    }
}

static void sys_taskCycle100ms(void)
{
	app_testHandler100ms();
}

static void sys_taskCycle500ms(void)
{
	hwa_ntcHandler500ms();
}


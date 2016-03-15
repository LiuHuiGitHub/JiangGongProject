#include <STC15F2K60S2.H>
#include "sys_ccap.h"
#include "intrins.h"

static data UINT8 u8_OverCnt = 0;                           //�洢PCA��ʱ�������
static data UINT32 u32_TempValue;
static data UINT16 u16_NowValue;
static data UINT16 u16_LastValue = 0;
data UINT32 u32_PulseWidth = 0;
data UINT32 u32_PulseCycle = 0;                     //u32_PulseCycle*625/1152->us

void sys_ccapInit(void)
{
	P1M1 |= 0x01;       		    //��������
    P1M0 &= ~0x01;
    
    CCON = 0;                       //��ʼ��PCA���ƼĴ���
                                    //PCA��ʱ��ֹͣ
                                    //���CF��־
                                    //���ģ���жϱ�־
    CL = 0;                         //��λPCA�Ĵ���
    CH = 0;
    CCAP0L = 0;
    CCAP0H = 0;
    CMOD = 0x01;                    //����ʱPCA��������,����PCAʱ��ԴΪϵͳʱ��/12,��ʹ��PCA��ʱ����ж�
    CCAPM0 = 0x31;                  //PCAģ��0Ϊ16λ����ģʽ(������/�½��ز���,�ɲ�ߵ�ƽ���ߵ͵�ƽ���),�Ҳ��������ж�
    PPCA = 1;                       //PCA���ȼ���
//    CR = 1;                         //PCA��ʱ����ʼ����
//    EA = 1;
    sys_TrigPulseInit();
}

void sys_ccapEnable(void)
{
    CR = 1;                         //PCA��ʱ����ʼ����
}

void sys_ccapDisable(void)
{
    CR = 0;                         //PCA��ʱ����ʼ����
}

#define TRIG_OPEN_WIDTH (65536-FOSC/12/1000)
void sys_TrigPulseInit(void)
{
    TMOD = 0x00;
	TH0 = 0;
	TL0 = 0;
	TR0 = 0;
	ET0 = 1;
//    EA = 1;
    
    P17 = 0;
}
void sys_trigPulseWidth(UINT16 width)
{
    TR0 = 0;
    TL0 = (UINT8)(-width);
    TH0 = (UINT8)((-width)>>8);
    TR0 = 1;
}

void sys_trigPulse() interrupt 1
{
    TR0 = 0;
    if(P17 == 0)
    {
        TL0 = (UINT8)TRIG_OPEN_WIDTH;
        TH0 = (UINT8)(TRIG_OPEN_WIDTH>>8);
        TR0 = 1;
    }
    P17 = !P17;
}

void PCA_isr() interrupt 7
{
    if (CF)
    {
        CF = 0;
        u8_OverCnt++;               //PCA��ʱ�������+1
    }
    if (CCF0)
    {
        CCF0 = 0;
        
        ((UINT8*)&u16_NowValue)[1] = CCAP0L;
        ((UINT8*)&u16_NowValue)[0] = CCAP0H;
        
        ((UINT8*)&u32_TempValue)[3] = 0;
        ((UINT8*)&u32_TempValue)[2] = 0;
        ((UINT8*)&u32_TempValue)[1] = u8_OverCnt;
        ((UINT8*)&u32_TempValue)[0] = 0;
        
        u8_OverCnt = 0;
        
        u32_TempValue += u16_NowValue;
        u32_TempValue -= u16_LastValue;
        
        if(P11 == 0)
        {
            u32_PulseWidth = u32_TempValue;
        }
        else
        {
            u32_PulseCycle = u32_TempValue + u32_PulseWidth;
            sys_trigPulseWidth(u32_PulseWidth);
        }

        u16_LastValue = u16_NowValue;
    }
}


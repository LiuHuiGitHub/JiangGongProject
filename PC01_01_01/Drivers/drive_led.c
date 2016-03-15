#include <STC15F2K60S2.H>
#include "drive_led.h"

sbit LED_CS0 = P3 ^ 4;
sbit LED_CS1 = P3 ^ 3;
sbit LED_CS2 = P3 ^ 2;
sbit LED_CS3 = P3 ^ 7;

/*数码管管脚定义
P20~7
a f b g c e h d
*/

/*Seg Class*/
#define SEG_CLASS       0   //0->共阳 1->共阴

/*Seg Port*/
#define SEG_PORT        P2

/*Seg Pin*/
#define SEG_PORT_A      BIT0
#define SEG_PORT_B      BIT2
#define SEG_PORT_C      BIT4
#define SEG_PORT_D      BIT7
#define SEG_PORT_E      BIT5
#define SEG_PORT_F      BIT1
#define SEG_PORT_G      BIT3
#define SEG_PORT_H      BIT6

/*Seg Coding*/
#define SEG_0           (SEG_PORT_A|SEG_PORT_B|SEG_PORT_C|SEG_PORT_D|SEG_PORT_E|SEG_PORT_F)
#define SEG_1           (SEG_PORT_B|SEG_PORT_C)
#define SEG_2           (SEG_PORT_A|SEG_PORT_B|SEG_PORT_D|SEG_PORT_E|SEG_PORT_G)
#define SEG_3           (SEG_PORT_A|SEG_PORT_B|SEG_PORT_C|SEG_PORT_D|SEG_PORT_G)
#define SEG_4           (SEG_PORT_B|SEG_PORT_C|SEG_PORT_F|SEG_PORT_G)
#define SEG_5           (SEG_PORT_A|SEG_PORT_C|SEG_PORT_D|SEG_PORT_F|SEG_PORT_G)
#define SEG_6           (SEG_PORT_A|SEG_PORT_C|SEG_PORT_D|SEG_PORT_E|SEG_PORT_F|SEG_PORT_G)
#define SEG_7           (SEG_PORT_A|SEG_PORT_B|SEG_PORT_C)
#define SEG_8           (SEG_PORT_A|SEG_PORT_B|SEG_PORT_C|SEG_PORT_D|SEG_PORT_E|SEG_PORT_F|SEG_PORT_G)
#define SEG_9           (SEG_PORT_A|SEG_PORT_B|SEG_PORT_C|SEG_PORT_D|SEG_PORT_F|SEG_PORT_G)
#define SEG_A           (SEG_PORT_A|SEG_PORT_B|SEG_PORT_C|SEG_PORT_E|SEG_PORT_F|SEG_PORT_G)
#define SEG_B           (SEG_PORT_C|SEG_PORT_D|SEG_PORT_E|SEG_PORT_F|SEG_PORT_G)
#define SEG_C           (SEG_PORT_A|SEG_PORT_D|SEG_PORT_E|SEG_PORT_F)
#define SEG_D           (SEG_PORT_B|SEG_PORT_C|SEG_PORT_D|SEG_PORT_E|SEG_PORT_G)
#define SEG_E           (SEG_PORT_A|SEG_PORT_D|SEG_PORT_E|SEG_PORT_F|SEG_PORT_G)
#define SEG_F           (SEG_PORT_A|SEG_PORT_E|SEG_PORT_F|SEG_PORT_G)
#define SEG_P           (SEG_PORT_A|SEG_PORT_B|SEG_PORT_E|SEG_PORT_F|SEG_PORT_G)
#define SEG_U           (SEG_PORT_B|SEG_PORT_C|SEG_PORT_D|SEG_PORT_E|SEG_PORT_F)
#define SEG_LINE        (SEG_PORT_G)
#define SEG_NULL        (~(SEG_8|SEG_PORT_H))

/**/
#define SEG_ON          (SEG_CLASS==0)
#define SEG_OFF         (SEG_CLASS==1)

//与显示相关变量
code UINT8 ledCoding[] = {  
                            ~SEG_0,~SEG_1,~SEG_2,~SEG_3,~SEG_4,
                            ~SEG_5,~SEG_6,~SEG_7,~SEG_8,~SEG_9,
                            ~SEG_A,~SEG_B,~SEG_C,~SEG_D,~SEG_E,
                            ~SEG_F,~SEG_LINE,~SEG_P,~SEG_U,~SEG_NULL
                        };

data UINT8 u8_ledDisBuff[4] = {0,19,19,19};
data UINT8 u8_ledDot = 0x00;
static data UINT8 u8_ledIndex = 0;//从左到右为0,1,2

void drv_ledInit(void)
{
	P3M1 &= ~0x9C;      //推挽输出
	P3M0 |= 0x9C;
	P2M1 &= ~0xFF;      //推挽输出
	P2M0 |= 0xFF;
	LED_CS0 = SEG_OFF;
	LED_CS1 = SEG_OFF;
	LED_CS2 = SEG_OFF;
	LED_CS3 = SEG_OFF;
}

/* LED动态显示 */
void drv_ledHandler1ms(void)
{
	LED_CS0 = SEG_OFF;
	LED_CS1 = SEG_OFF;
	LED_CS2 = SEG_OFF;
	LED_CS3 = SEG_OFF;
	u8_ledIndex++;
	u8_ledIndex %= 4;
	SEG_PORT = ledCoding[u8_ledDisBuff[u8_ledIndex]];
	if (u8_ledDot & (1 << u8_ledIndex))//show dot
	{
        if(SEG_CLASS)
        {
            SEG_PORT |= SEG_PORT_H;
        }
        else
        {
            SEG_PORT &= ~SEG_PORT_H;
        }
	}
	if (u8_ledIndex == 0)
	{
		LED_CS0 = SEG_ON;
	}
	else if (u8_ledIndex == 1)
	{
		LED_CS1 = SEG_ON;
	}
	else if (u8_ledIndex == 2)
	{
		LED_CS2 = SEG_ON;
	}
	else if (u8_ledIndex == 3)
	{
		LED_CS3 = SEG_ON;
	}
}

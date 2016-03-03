#include "reg52.h"
#include "intrins.h"        //包含_nop_()指令头文件
#include "drive_tm1640.h"
#include "sys_delay.h"

/********************定义控制端口**********************/
sbit DIN1640_1 = P2 ^ 6;               //定义DIN数据端口
sbit CLK1640_1 = P2 ^ 7;               //定义CLK数据端口


#define LED1        0
#define LED2        1
#define LED3        2
#define LED4        3
#define LED5        4
#define LED6        5
#define LED7        6
#define LED8        7
#define LED9        8
#define LED10       9
#define LED11       10
#define LED12       11
#define LED13       12
#define LED14       13
#define LED15       14
#define LED16       15
code unsigned char TM1640_Place[TM1640_NUM][16] =       //位选地址对应数码管
{
    {LED1, LED14, LED15, LED16, LED11, LED12, LED13, LED8, LED9, LED10, LED5, LED4, LED3, LED2, LED7, LED6},
    {LED15, LED16, LED14, LED11, LED12, LED13, LED1, LED8, LED9, LED10, LED5, LED4, LED3, LED2, LED7, LED6},
};
code unsigned char TM1640_Section[TM1640_NUM][22] =     //段选
{
    //'0'   '1'   '2'   '3'   '4'   '5'   '6'   '7'   '8'   '9'   ' '   '-'   'F'   'O'   'N'  '\0'   'c'   'e'   'a'   'u'   'b'  'L'
    {0x7e, 0x42, 0xba, 0xda, 0xc6, 0xdc, 0xfc, 0x4a, 0xfe, 0xde, 0x00, 0x80, 0xac, 0x7e, 0x6e, 0x00, 0x3c, 0xbc, 0xee, 0x76, 0xf4, 0x34},
    {0xfa, 0x28, 0xb6, 0xbc, 0x6c, 0xdc, 0xde, 0xa8, 0xfe, 0xfc, 0x00, 0x04, 0xc6, 0xfa, 0xea, 0x00, 0xd2, 0xd6, 0xee, 0x7a, 0x5e, 0x52},
};
code unsigned char SegCoding[] = "0123456789 -FON\0ceaubL";    //编码
#define SEG_CODING_LEN       (sizeof(SegCoding))

/********************Start函数*************************/
void I2CStart()
{
    DIN1640_1 = 1;
    CLK1640_1 = 1;
    _nop_();
    _nop_();
    DIN1640_1 = 1;
    _nop_();
    _nop_();
    DIN1640_1 = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    CLK1640_1 = 0;
    _nop_();
    _nop_();
}

void I2CStart2()
{
    CLK1640_1 = 1;
    DIN1640_1 = 1;
    _nop_();
    _nop_();
    CLK1640_1 = 1;
    _nop_();
    _nop_();
    CLK1640_1 = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    DIN1640_1 = 0;
    _nop_();
    _nop_();
}

/********************Stop函数*************************/
void I2CStop()
{
    CLK1640_1 = 1;
    _nop_();
    _nop_();
    DIN1640_1 = 0;
    _nop_();
    _nop_();
    DIN1640_1 = 1;
    _nop_();
    _nop_();
    CLK1640_1 = 0;
    DIN1640_1 = 0;
}

void I2CStop2()
{
    DIN1640_1 = 1;
    _nop_();
    _nop_();
    CLK1640_1 = 0;
    _nop_();
    _nop_();
    CLK1640_1 = 1;
    _nop_();
    _nop_();
    DIN1640_1 = 0;
    CLK1640_1 = 0;
}

/***************发送8bit数据，从低位开始**************/
void I2CWritebyte(unsigned char oneByte)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        CLK1640_1 = 0;
        if(oneByte & 0x01)
            DIN1640_1 = 1;
        else
            DIN1640_1 = 0;
        _nop_();
        _nop_();
        _nop_();
        CLK1640_1 = 1;
        oneByte = oneByte >> 1;
    }
    CLK1640_1 = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    DIN1640_1 = 0;
    _nop_();
    _nop_();
}

void I2CWritebyte2(unsigned char oneByte)
{
    unsigned char i;
    for(i = 0; i < 8; i++)
    {
        DIN1640_1 = 0;
        if(oneByte & 0x01)
            CLK1640_1 = 1;
        else
            CLK1640_1 = 0;
        _nop_();
        _nop_();
        _nop_();
        DIN1640_1 = 1;
        oneByte = oneByte >> 1;
    }
    DIN1640_1 = 0;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    CLK1640_1 = 0;
    _nop_();
    _nop_();
}

/*************************************0~4通道显示函数*****************************************/
void drv_tm1640Update1(unsigned char *pData)
{
    unsigned char addr, index;

    I2CStart();
    I2CWritebyte(0x40); //数据命令设置：普通模式，地址自动加一
    I2CStop();

    I2CStart();
    I2CWritebyte(0xc0); //地址命令设置：初始地址00H
    for(addr = 0; addr < 16; addr++) //发送16位显示数据
    {
        for(index = 0; index < SEG_CODING_LEN; index++)
        {
            if(SegCoding[index] == pData[TM1640_Place[0][addr]])    //找到地址i上的数据在编码所在的位置index
            {
                break;
            }
        }
        if(index == SEG_CODING_LEN)
        {
            index = 10;     //未找到默认显示空' '
        }
        I2CWritebyte(TM1640_Section[0][index]);
    }
    I2CStop();

    I2CStart();
    I2CWritebyte(0x89); //显示控制：显示开，脉冲宽度设为11/16
    I2CStop();
}

/*************************************5~9??????*****************************************/
void drv_tm1640Update2(unsigned char *pData)
{
    unsigned char addr, index;

    I2CStart2();
    I2CWritebyte2(0x40);
    I2CStop2();

    I2CStart2();
    I2CWritebyte2(0xc0);
    for(addr = 0; addr < 16; addr++) //发送16位显示数据
    {
        for(index = 0; index < SEG_CODING_LEN; index++)
        {
            if(SegCoding[index] == pData[TM1640_Place[1][addr]])    //找到地址i上的数据在编码所在的位置index
            {
                break;
            }
        }
        if(index == SEG_CODING_LEN)
        {
            index = 10;     //未找到默认显示空' '
        }
        I2CWritebyte2(TM1640_Section[1][index]);
    }
    I2CStop2();

    I2CStart2();
    I2CWritebyte2(0x89);
    I2CStop2();
}

void drv_tm1640Init(void)
{
    drv_tm1640Update1("                ");
    drv_tm1640Update2("                ");
}

void drv_tm1640Display(UINT8 *pData)
{
    drv_tm1640Update1(pData);
    drv_tm1640Update2(pData + 16);
}

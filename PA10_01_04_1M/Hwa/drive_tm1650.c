#include "STC15F2K60S2.h"
#include "sys_delay.h"		//包含_nop_()指令头文件
#include "drive_tm1650.h"

//#define sys_delayus(10) _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();  //宏定义

/********************定义控制端口**********************/
sbit SCL=P5^5;                       //时钟线
sbit SDA=P5^4;                       //数据线

/******************** START信号 ***********************/
void TM1650_START()
{
    SCL=1;
    SDA=1;
    sys_delayus(10);
    SDA=0;
    sys_delayus(10);
    SCL=0;
}
/******************** STOP信号 ************************/
void TM1650_STOP()
{
    SDA=0;
    sys_delayus(10);
    SCL=1;
    sys_delayus(10);
    SDA=1;
    sys_delayus(10);
    SCL=0;
    SDA=0;
}
/****************写1个字节给TM1650********************/
void write_8bit( unsigned char dat)
{
    unsigned char i;
    SCL=0;
    for(i=0;i<8;i++)
        {
        if(dat&0x80)
        {
            SDA=1;
            sys_delayus(10);
            SCL=1;
            sys_delayus(10);
            SCL=0;   
        }
        else
        {
            SDA=0;
            sys_delayus(10);
            SCL=1;
            sys_delayus(10);
            SCL=0;
        }   
            dat<<=1;     
        }
        SDA=1;          //ACK信号
        sys_delayus(10);
        SCL=1;
        sys_delayus(10);
        SCL=0;
        sys_delayus(10);  
}

/**********************读8bit**************************/
unsigned char read_8bit()
{
    unsigned char dat,i;
    SDA=1;
    dat=0;
    for(i=0;i<8;i++)
    {
    SCL=1;                        //时钟上沿
    sys_delayus(10);
    dat<<=1;
    if(SDA)
     dat++;
    SCL=0;
    sys_delayus(10);
    }
    SDA=0;              //ACK信号
    sys_delayus(10);
    SCL=1;
    sys_delayus(10);
    SCL=0;
    sys_delayus(10);
    
    return dat ;

} 
/*******************读按键命令************************/
unsigned char TM1650_read(void)
{
    unsigned char key;
    TM1650_START();
    write_8bit(0x49);     //读按键指令   
    key=read_8bit();
    TM1650_STOP();
    return key;
}
code unsigned char keyCoding[] = {86,94,102,110,118,78,79,119,111,103,95,87};
code unsigned char keyNum = sizeof(keyCoding);

unsigned char drv_tm1650GetKey(void)
{
    unsigned char tmp, i;;
    tmp = TM1650_read();
    for(i=0; i<keyNum; i++)
    {
        if(tmp==keyCoding[i])
        {
            return i;
        }
    }
    return KEY_NONE;
}
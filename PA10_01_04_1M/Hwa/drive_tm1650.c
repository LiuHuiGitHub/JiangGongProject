#include "STC15F2K60S2.h"
#include "sys_delay.h"		//包含_nop_()指令头文件
#include "drive_tm1650.h"

sbit SCL = P3 ^ 1;
sbit SDA = P3 ^ 0;

#define CON_ADDR    0x81

UINT16 drv_touchRead(void)
{
    UINT8 bitnum, addr = CON_ADDR;
    BOOL bit_temp;
    UINT16 key = 0;

    SDA = 0;
    //send  addr
    sys_delayus(5);
    for(bitnum = 0; bitnum < 8; bitnum++)
    {
        SCL = 0;
        if(addr & 0x80)
            SDA = 1;
        else
            SDA = 0;
        addr = addr << 1;
        sys_delayus(5);
        SCL = 1;
        sys_delayus(5);
    }

    SDA = 1;		//release sda

    //read ack
    SCL = 0;
    sys_delayus(5);
    SCL = 1;
    sys_delayus(5);
    bit_temp = SDA;     //ACK
    if(bit_temp)
        return 0xFFFF;	//error occurs
    //read key
    for(bitnum = 0; bitnum < 16; bitnum++)
    {
        SCL = 0;
        sys_delayus(5);
        SCL = 1;
        sys_delayus(5);
        bit_temp = SDA;
        if(bit_temp)
        {
            key = key << 1;
            key = key | 0x01;
        }
        else
        {
            key = key << 1;
        }
    }
    SCL = 0;
    sys_delayus(5);
    SCL = 1;
    sys_delayus(5);
    bit_temp = SDA;     //NACK
    if(!bit_temp)
        return 0xFFFF;	//error occurs
    SCL = 0;
    sys_delayus(5);
    SDA = 0;
    sys_delayus(5);
    SCL = 1;
    sys_delayus(5);
    SDA = 1;
    
    key ^= 0xFFFF;

    return key;
}

/*
64      256     32768
32      512     16384
16      1024    8192
128     2048    4096
*/
                                /*0  1  2  3  4  5  6  7  8  9  10  11*/
code unsigned int keyCoding[] = {2048,16,1024,8192,32,512,16384,64,256,32768,128,4096};
code unsigned char keyNum = sizeof(keyCoding)/sizeof(*keyCoding);

unsigned char drv_tm1650GetKey(void)
{
    unsigned int tmp;
    unsigned char i;
    tmp = drv_touchRead();
    for(i=0; i<keyNum; i++)
    {
        if(tmp==keyCoding[i])
        {
            return i;
        }
    }
    return KEY_NONE;
}
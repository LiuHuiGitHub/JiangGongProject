#include <STC15F2K60S2.H>
#include "hwa_uart.h"
#include "sys_uart.h"
#include "string.h"
#include "drive_buzzer.h"
#include "drive_relay.h"
#include "drive_led.h"
#include "sys_delay.h"
#include "app_time.h"
#include "app_config.h"
#include "app_test.h"


#define TX_BUFF_LEN     RX_BUFF_LEN
BOOL comRxFalg = FALSE;
BOOL comTxFlag = FALSE;
UINT8 comRxLen = 0;
//UINT8 comRxBuff[RX_BUFF_LEN] = {0xAA,0x7,0x20,0x01,0x00,0x30,0x00,0x00,0x16,0xCC};    //添加时间
//UINT8 comRxBuff[RX_BUFF_LEN] = {0xAA,0x7,0x20,0x01,0x00,0x00,0x00,0x00,0x26,0xCC};    //清除时间
UINT8 comRxBuff[RX_BUFF_LEN] = {0xAA,0x2,0x23,0x21,0xCC};      //读版本
//UINT8 comRxBuff[RX_BUFF_LEN] = {0xAA,0x7,0x20,0x01,0x30,0x00,0x00,0x00,0x16,0xCC};
UINT8 comTxBuff[TX_BUFF_LEN];

#define MAX_DATA_LEN		10

typedef struct
{
    UINT8 head;
    UINT8 len;
    UINT8 cmd;
    UINT8 dat[MAX_DATA_LEN];
    UINT8 check;
    UINT8 end;
}COM_STD_STRUCT;

typedef struct
{
    UINT8 head;
    UINT8 cmd;
    UINT8 value;
    UINT8 end;
    COM_STD_STRUCT std_dat;
    UINT8 expand;
}COM_TX_STRUCT;

COM_STD_STRUCT s_comRxBuff;
COM_TX_STRUCT s_comTxBuff;
code COM_TX_STRUCT c_s_comTxBuff = {
									0xBB,0x00,0x00,0xCC,
									0xAA,0x00,0x00,
									0x00,0x00,0x00,0x00,0x00,
									0x00,0x00,0x00,0x00,0x00,
									0x00,0xCC,0x00
									};

void hwa_uartInit(void)
{
    sys_uartInit();
}

void hwa_uartHandler1ms(void)
{
	UINT8 i;
    UINT8* p = comRxBuff;
	if(sys_uartOverTime1ms()!=FALSE)
	{
		comRxLen = sys_uartReadData(comRxBuff);
        s_comRxBuff.head = *p++;
        s_comRxBuff.len = *p++;
        s_comRxBuff.cmd = *p++;
        for(i=0; i<s_comRxBuff.len-2; i++)
        {
            if(i < MAX_DATA_LEN)
            {
                s_comRxBuff.dat[i] = *p++;
            }
        }
        for(; i<MAX_DATA_LEN; i++)
        {
            s_comRxBuff.dat[i] = 0x00;
        }
        s_comRxBuff.check = comRxBuff[s_comRxBuff.len+1];
        s_comRxBuff.end = comRxBuff[s_comRxBuff.len+2];
        comRxFalg = TRUE;
	}
}

#define CE_OK                   0
#define CE_CHECK_ERROR          1
#define CE_BAD_COMMAND          2
#define CE_BAD_PARAM            3
#define CE_BAD_PADATA           4

BOOL b_ResetFlag = FALSE;

UINT8 comfun_0x21(void)       //Reset
{
    b_ResetFlag = TRUE;
    return CE_OK;
}

UINT8 comfun_0x23(void)   	//Read Version
{
	s_comTxBuff.expand = TRUE;
    s_comTxBuff.std_dat.dat[0] = HardWareVersion;
    s_comTxBuff.std_dat.dat[1] = SoftWareVersion;
	s_comTxBuff.std_dat.len = 4;
	drv_buzzerNumber(1);
    return CE_OK;
}

UINT8 comfun_0x10(void)
{
    if(s_comRxBuff.dat[0] == s_System.Addr)
    {
        drv_relayOpen(0);
        sys_delayms(200);
    	s_comTxBuff.expand = TRUE;
        if(app_testGetFuseState())
        {
            s_comTxBuff.std_dat.dat[0] = 0;
            drv_buzzerNumber(1);
        }
        else
        {
            s_comTxBuff.std_dat.dat[0] = 1;
            drv_buzzerNumber(2);
        }
        s_comTxBuff.std_dat.dat[1] = 0;
    	s_comTxBuff.std_dat.len = 4;
        return CE_OK;
    }
    else
    {
        return CE_BAD_PADATA;
    }
}

UINT8 comfun_0x20(void)
{
    UINT8 time;
    if(s_comRxBuff.dat[0] == s_System.Addr)
    {
        if(s_comRxBuff.dat[1] == 0
            && s_comRxBuff.dat[2] == 0
            && s_comRxBuff.dat[3] == 0
            && s_comRxBuff.dat[4] == 0
        )
        {
            if(app_timeGetTime(0) != 0)
            {
                app_timeClear(0);
            }
            s_comTxBuff.std_dat.dat[0] = 0;
            drv_buzzerNumber(1);
        }
        else
        {
            if(s_comRxBuff.dat[1] >= 99
                || s_comRxBuff.dat[2] >= 60
            )
            {
                return CE_BAD_PADATA;
            }
            
            drv_relayOpen(0);
            sys_delayms(200);
            if(app_testGetFuseState())
            {
                s_comTxBuff.std_dat.dat[0] = 0;
                time = (UINT16)s_comRxBuff.dat[1]*60+s_comRxBuff.dat[2];
                app_timeAddTime(0, time);
                drv_buzzerNumber(1);
            }
            else
            {
                s_comTxBuff.std_dat.dat[0] = 1;
                drv_buzzerNumber(2);
            }
        }
    	s_comTxBuff.expand = TRUE;
        s_comTxBuff.std_dat.dat[1] = 0;
    	s_comTxBuff.std_dat.len = 4;
        return CE_OK;
    }
    else
    {
        return CE_BAD_PADATA;
    }
}

UINT8 comfun_0x00(void)
{
    s_System.Addr = s_comRxBuff.dat[0];
    app_configWrite(SYSTEM_SETTING_SECTOR);
	drv_buzzerNumber(1);
    return CE_OK;
}

UINT8 comCheck(UINT8* dat, UINT8 len)
{
	UINT8 i, check = 0;
	for(i=0; i<=len; i++)
	{
		check ^= dat[i];
	}
	return check;
}

void hwa_uartHandler10ms(void)
{
	UINT8 i;
    UINT8 error = CE_OK;
    if(comRxFalg)
    {
        comRxFalg = FALSE;
        if(s_comRxBuff.head != 0xAA)
        {
            return;
        }
        comTxFlag = TRUE;
        memcpy((UINT8*)&s_comTxBuff, (UINT8*)&c_s_comTxBuff, sizeof(s_comTxBuff));
		if(s_comRxBuff.len > MAX_DATA_LEN
			|| s_comRxBuff.len < 2)
		{
            error = CE_BAD_PARAM;
		}
        if(s_comRxBuff.head == 0xAA && s_comRxBuff.end == 0xCC)
        {
        	if(comCheck(&s_comRxBuff.len, s_comRxBuff.len) == s_comRxBuff.check)
        	{
        		s_comTxBuff.cmd = s_comRxBuff.cmd;
                switch(s_comRxBuff.cmd)
                {
                    case 0x21:      //Reset
                        error = comfun_0x21();
                        break;
                    case 0x23:      //Read Version
                        error = comfun_0x23();
                        break;
                    case 0x10:      //Read State
                        error = comfun_0x10();
                        break;
                    case 0x20:      //Cmd
                        error = comfun_0x20();
                        break;
                    case 0x00:
                        error = comfun_0x00();
                        break;
                    default:        //Cmd Error
                        error = CE_BAD_COMMAND;
                        break;
                }
        	}
            else
            {
                error = CE_CHECK_ERROR;
            }
        }
        else
        {
            error = CE_BAD_PARAM;
        }
        s_comTxBuff.value = error;
    }
    
    if(comTxFlag)
    {
        comTxFlag = FALSE;
        comTxBuff[0] = s_comTxBuff.head;
        comTxBuff[1] = s_comTxBuff.cmd;
        comTxBuff[2] = s_comTxBuff.value;
        comTxBuff[3] = s_comTxBuff.end;
        if(s_comTxBuff.expand)
        {
        	s_comTxBuff.std_dat.cmd = s_comTxBuff.cmd;
	        comTxBuff[4] = s_comTxBuff.std_dat.head;
	        comTxBuff[5] = s_comTxBuff.std_dat.len;
	        comTxBuff[6] = s_comTxBuff.std_dat.cmd;
	        for(i=0; i<s_comTxBuff.std_dat.len-2; i++)
	        {
	        	comTxBuff[7+i] = s_comTxBuff.std_dat.dat[i];
	        }
	        i += 7;
        	s_comTxBuff.std_dat.check = comCheck((UINT8*)&s_comTxBuff.std_dat.len, s_comTxBuff.std_dat.len);
	        comTxBuff[i++] = s_comTxBuff.std_dat.check;
	        comTxBuff[i++] = s_comTxBuff.std_dat.end;
	        
            sys_uartSendData(comTxBuff, 4+2+s_comTxBuff.std_dat.len+1);
        }
        else
        {
            sys_uartSendData(comTxBuff, 4);
        }
    }
    
    if(b_ResetFlag)
    {
    	while(1);
    }
}


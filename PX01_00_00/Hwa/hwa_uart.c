#include "hwa_uart.h"
#include "sys_uart.h"
#include "string.h"
#include "app_time.h"
#include "app_test.h"


UINT8 u8ErrorCnt = 0;

#define MSG_LEN		((UINT8)8)
#define MSG_HEAD	(0xA5)

typedef enum
{
    /*Req Cmd*/
    MASTER_READ_STATE       = 0x00,
    MASTER_BRUSH_CHANNEL	= 0x01,
    MASTER_REFUND_MONEY		= 0x02,
    MASTER_PAUSE_BRUSH		= 0x03,
    MASTER_UPDATE_CHANNEL	= 0x04,
    
    /*Resp Cmd*/
    SLAVE_NONE       		= 0x80,
    SLAVE_BRUSH_MONEY		= 0x81,
    SLAVE_BRUSH_COMPLETE	= 0x82,
    SLAVE_REFUND_CHANNEL	= 0x83,
    SLAVE_REFUND_COMPLETE	= 0x84,

    /*Rx State*/
	RX_NONE         		= 0xF0,
	RX_RIGHT        		= 0xF1,
	RX_LEN_ERROR    		= 0xF2,
	RX_HEAD_ERROR   		= 0xF3,
	RX_CRC_ERROR    		= 0xF4,
    RX_CMD_ERROR    		= 0xF5,
}STATE_ENUM;

#define NONE_REFUND_CHANNEL      0xFF

UINT8 u8_BrushChannel = NONE_BRUSH_CHANNEL;
UINT8 u8_RefundChannel = NONE_REFUND_CHANNEL;
UINT8 BrushMoneyH = 0;
UINT8 BrushMoneyL = 0;
UINT16 u16_Channel = 0;
BOOL b_PauseBrush = FALSE;

typedef struct
{
	UINT8 head;
	STATE_ENUM cmd;
	UINT8 dat[4];
	UINT8 crc[2];
}MSG_STRUCT;

MSG_STRUCT Rx;
STATE_ENUM state = RX_NONE;

STATE_ENUM e_MasterState = MASTER_READ_STATE;


UINT16 CRC16_RTU( UINT8 *dat, UINT8 len)
{
    UINT16 crc = 0xFFFF;
    UINT8 i;

    while(len--)
    {
        crc = crc ^*dat++;
        for ( i = 0; i < 8; i++)
        {
            if( ( crc & 0x0001) > 0)
            {
                crc = crc >> 1;
                crc = crc ^ 0xa001;
            }
            else
                crc = crc >> 1;
        }
    }
    return ( crc);
}

void hwa_uartInit(void)
{
    sys_uartInit();
}

void hwa_uartHandler1ms(void)
{
	UINT8 len;
	if(sys_uartOverTime1ms()!=FALSE)
	{
		len = sys_uartReadData((UINT8*)&Rx);
		if(len < MSG_LEN)
		{
			state = RX_LEN_ERROR;
		}
		else if(Rx.head != MSG_HEAD)
		{
			state = RX_HEAD_ERROR;
		}
		else if(CRC16_RTU((UINT8*)&Rx, MSG_LEN-2)!=(Rx.crc[0]|Rx.crc[1]<<8))
		{
			state = RX_CRC_ERROR;
		}
		else
		{
			state = RX_RIGHT;
		}
	}
}

BOOL hwa_uartReadRxState(void)
{
	if(state != RX_NONE)
	{
		if(state != RX_RIGHT)
		{
			u8ErrorCnt += 2;
			if(u8ErrorCnt>=10)
			{
				u8ErrorCnt = 10;
			}
		    state = RX_NONE;
		}
		else
		{
			if(u8ErrorCnt)
			{
				u8ErrorCnt--;
			}
		    state = RX_NONE;
		    if(Rx.cmd<RX_NONE)
		    {
            	return TRUE;
            }
		}
	}
    return FALSE;
}

static UINT8 u8_count = 0;
void hwa_uartHandler100ms(void)
{
	UINT16 crc;
    BOOL Rx_CplFlag;					//接收消息帧完成标志
	MSG_STRUCT Tx = {MSG_HEAD,MASTER_READ_STATE,0x00,0x00,0x00,0x00,0x00,0x00};
    Rx_CplFlag = hwa_uartReadRxState();
    switch(e_MasterState)
    {
        case MASTER_READ_STATE:								//读取刷卡版当前状态
            if(Rx_CplFlag)
            {
                if(Rx.cmd == SLAVE_NONE)					//无刷卡无返款
                {
                    if(++u8_count >= 10)
                    {
                        u8_count = 0;
	                    Tx.cmd = MASTER_UPDATE_CHANNEL;
	                    Tx.dat[0] = (UINT8)u16_Channel;
	                    Tx.dat[1] = (UINT8)(u16_Channel>>8);
                    }
                    else
                    {
                    	Tx.cmd = MASTER_READ_STATE;
                    }
                }
                else
                {
                	u8_count = 0;
	                if(Rx.cmd == SLAVE_BRUSH_MONEY)
	                {
	                    BrushMoneyH = Rx.dat[0];			//记录刷卡金额
	                    BrushMoneyL = Rx.dat[1];
	                    e_MasterState = MASTER_BRUSH_CHANNEL;
	                    return;
	                }
	                else if(Rx.cmd == SLAVE_REFUND_CHANNEL)
	                {
	                	u8_RefundChannel = Rx.dat[0];		//记录返款通道
	                    e_MasterState = MASTER_REFUND_MONEY;
	                    return;
	                }
	                else
	                {
	                    e_MasterState = MASTER_READ_STATE;
	                    Tx.cmd = MASTER_READ_STATE;
	                }
                }
            }
            else
            {
            	if(b_PauseBrush)
	        	{
        			Tx.cmd = MASTER_PAUSE_BRUSH;
	        	}
	        	else
	        	{
                	Tx.cmd = MASTER_READ_STATE;
                }
            }
            break;
            
        case MASTER_REFUND_MONEY:							//刷卡版请求返款，主机发送返款金额
        	if(Rx_CplFlag)
            {
                if(Rx.cmd == SLAVE_REFUND_COMPLETE)			//刷卡版已返款完毕
                {
                	app_timeClear(u8_RefundChannel);
                	u8_RefundChannel = NONE_REFUND_CHANNEL;
                	e_MasterState = MASTER_READ_STATE;
                	return;
                }
                else if(Rx.cmd == SLAVE_NONE)			    //刷卡版返款失败
                {
                	e_MasterState = MASTER_READ_STATE;
                	return;
                }
            }
            else
            {
                Tx.cmd = MASTER_REFUND_MONEY;
                Tx.dat[0] = (UINT8)(u16_DisplayTime[u8_RefundChannel]/s_SystemSetting.CardTimeOrPower);
                Tx.dat[1] = (UINT8)((u16_DisplayTime[u8_RefundChannel]%s_SystemSetting.CardTimeOrPower)*100/s_SystemSetting.CardTimeOrPower);
            }
            break;
            
        case MASTER_BRUSH_CHANNEL:							//刷卡版请求通道，主机发送刷卡通道
            
            if(Rx_CplFlag)
            {
				if(Rx.cmd == SLAVE_BRUSH_COMPLETE)
                {
                    e_MasterState = MASTER_READ_STATE;
                    u8_BrushChannel = NONE_BRUSH_CHANNEL;
                    return;
                }
            }
            else if(u8_BrushChannel<10)						//已选择通道
            {
                Tx.cmd = MASTER_BRUSH_CHANNEL;
                Tx.dat[0] = u8_BrushChannel;
            }
            else
            {
            	return;
            }
            break;
            
        default:
            e_MasterState = MASTER_READ_STATE;
            return;
    }
	crc = CRC16_RTU((UINT8*)&Tx, MSG_LEN-2);
	Tx.crc[0] = crc;
	Tx.crc[1] = crc>>8;
	sys_uartSendData((UINT8*)&Tx, MSG_LEN);
}


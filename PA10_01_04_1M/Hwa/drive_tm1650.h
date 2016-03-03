#ifndef __DRIVE_TM1650_H__
#define __DRIVE_TM1650_H__


#define KEY_NONE        0xFF

unsigned int drv_touchRead(void);
unsigned char drv_tm1650GetKey(void);

#endif
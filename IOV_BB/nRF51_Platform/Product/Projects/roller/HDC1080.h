#ifndef _HDC1080_H_
#define _HDC1080_H_


////////////////////////////////////////////////////
//temper=[(d15:d0)/2^16]*165c-40c
//relative=[(d15:d0)/2^16]*100%
////////////////////////////////////////////////////
#define HDC1080_Device_Adderss      0x80
#define HDC1080_Read_Temperature    0x00
#define HDC1080_Read_Humidity       0x01
#define HDC1080_Read_Config         0x02

#define HDC1080_DEVICE_ID           0xff
#define HDC1080_ID                  0x1000
//////////////////////////////////////////////////////////////
#define HDC1080_Rst                 15 //0x8000 soft reset
#define HDC1080_Enht                13//0x2000  enable heat
#define HDC1080_Mode                12//0x1000  work mode-0:read seperately£¬1:read continuiously
#define HDC1080_Hres                8//0x0000 14 11 7bit temp

void HDC1080_Init(void);
int16_t HDC1080_Read_Temper(void);
uint16_t HDC1080_Read_Humidi(void);
uint16_t HDC1080_Read_Conf(void);
uint16_t HDC1080_Read_ManufacturerID(void);
uint16_t HDC1080_Read_DeviceID(void);

#endif


#ifndef _HDC1080_H_
#define _HDC1080_H_

#include <stdint.h>
////////////////////////////////////////////////////

////////////////////////////////////////////////////
#define HDC1080_Device_Adderss      0x40

/******
* register Map
*/
#define HDC1080_Temperature         0x00
#define HDC1080_Humidity            0x01
#define HDC1080_Config              0x02
#define HDC1080_SerialID_1          0xFB
#define HDC1080_SerialID_2          0xFC
#define HDC1080_SerialID_3          0xFD
#define HDC1080_ManufacturerID      0xFE
#define HDC1080_DEVICE_ID           0xFF



//////////////////////////////////////////////////////////////
typedef struct
{

    uint16_t res1               :8 ;  //reserved, must be 0
    uint16_t Hres               :2 ;  // humidity resolution 10 -- 8bit ; 01 -- 11bit ; 00 -- 14bit
    uint16_t Tres               :1 ;  // Temperature resolution 1 -- 11bit; 0 -- 14bit
    uint16_t BTST               :1 ;  // Battery Status 1 -- < 2.8V ; 0 -- >2.8V
    uint16_t MODE               :1 ;  // MODE  0 -- Temperature or Humidity ; 1 -- Temperature and Humidity
    uint16_t HEAT               :1 ;  // Heater 0 -- disable ; 1 -- enable
    uint16_t res2               :1 ;  //reserved, must be 0
    uint16_t RST                :1 ;  //Software Reset. 0 -- normal operation. this bit self clear; 1 -- software reset
} hdc1080_config_reg_t;


void HDC1080_Soft_Reset(void);
int16_t HDC1080_Read_Temperature(void);
uint16_t HDC1080_Read_Humidity(void);
void HDC1080_Read_Temperature_and_Humidity(uint16_t *temperature, uint16_t *humidity);

uint16_t HDC1080_Read_DeviceID(void);
uint16_t HDC1080_Read_ManufacturerID(void);
uint16_t HDC1080_Read_Config(void);
uint16_t HDC1080_Read_MODE(void);
void HDC1080_Set_MODE(uint8_t mode);

#endif


#include <stdint.h>
#include "juma_sdk_api.h"
#include "HDC1080.h"

/*******************************************************************************
  * @brief  HDC1080_Write_Buffer
  * @param  uint8_t addr is point register
  * @param  uint8_t *buffer is the need to write data point
  * @param  uint8_t len is the write data length
  * @retval void
 *******************************************************************************/
void HDC1080_Write_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
    I2C_bus_write(HDC1080_Device_Adderss, addr, buffer, len);
}
/*******************************************************************************
  * @brief  HDC1080_Read_Buffer between triger and read there is no wait.
  * @param  uint8_t addr is point register
  * @param  uint8_t *buffer is the need to read data point
  * @param  uint8_t len is the read data length
  * @retval void
 *******************************************************************************/
void HDC1080_Read_Buffer(uint8_t addr, uint8_t *buffer, uint8_t len)
{
    I2C_bus_read(HDC1080_Device_Adderss, addr, buffer, len);
}
/*******************************************************************************
  * @brief  HDC1080_Soft_Reset 
  * @param  void
  * @retval void
 *******************************************************************************/
void HDC1080_Soft_Reset(void)
{
    uint8_t temp[2];
    temp[0] = 0x80; 
    temp[1] = 0x00;
    HDC1080_Write_Buffer(HDC1080_Read_Config, temp, 2);
    nrf_delay_ms(20);//there should be waiting for more than 15 ms.
}
/*******************************************************************************
  * @brief  HDC1080_Setting 
  * @param  void
  * @retval void
 *******************************************************************************/
void HDC1080_Setting(void)
{
    uint16_t tempcom = 0;
    uint8_t temp[2];
	
    tempcom |= 1<<HDC1080_Mode ;
    // 14bit
    temp[0] = (uint8_t)(tempcom >> 8); 
    temp[1] = (uint8_t)tempcom;
    HDC1080_Write_Buffer(0x02, temp, 2);
}
/*******************************************************************************
  * @brief  HDC1080_Init 
  * @param  void
  * @retval void
 *******************************************************************************/
void HDC1080_Init(void)
{
    HDC1080_Soft_Reset();
    HDC1080_Setting();
}
/*******************************************************************************
  * @brief  HDC1080_Read_Temper 
  * @param  void
  * @retval int16_t tempr value
 *******************************************************************************/
int16_t HDC1080_Read_Temper(void)
{
    uint8_t buffer[2];

    HDC1080_Read_Buffer(0x00, buffer, 2);
    //return ((buffer[0]<<8)|buffer[1]);
    return (int16_t)(((((buffer[0]<<8)|buffer[1])/65536.0)*165-40)*100);
}
/*******************************************************************************
  * @brief  HDC1080_Read_Humidity 
  * @param  void
  * @retval int16_t Humidity value
 *******************************************************************************/
uint16_t HDC1080_Read_Humidi(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(HDC1080_Read_Humidity, buffer, 2);

    return (buffer[0]<<8)|buffer[1];
    //return (int16_t)((((buffer[0]<<8)|buffer[1])/65536.0)*10000);
}
/*******************************************************************************
  * @brief  HDC1080_Read_Configuration 
  * @param  void
  * @retval Config value
 *******************************************************************************/
uint16_t HDC1080_Read_Conf(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(HDC1080_Read_Config, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}
/*******************************************************************************
  * @brief  HDC1080_Read_ManufacturerID 
  * @param  void
  * @retval ManufacturerID 
 *******************************************************************************/
uint16_t HDC1080_Read_ManufacturerID(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(0xfe, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}
/*******************************************************************************
  * @brief  HDC1080_Read_DeviceID
  * @param  void
  * @retval DeviceID
 *******************************************************************************/
uint16_t HDC1080_Read_DeviceID(void)
{
    uint8_t buffer[2];
	
    HDC1080_Read_Buffer(0xff, buffer, 2);

    return ((buffer[0]<<8)|buffer[1]);
}

#include <stdint.h>
#include "juma_sdk_api.h"
#include "HDC1080.h"


/*******************************************************************************
  * @brief  HDC1080_Write_Frame
  * @param  uint8_t addr is point register
  * @param  uint8_t data is the content need to write
  * @retval void
 *******************************************************************************/
void HDC1080_Write_Frame(uint8_t addr, uint16_t data)
{
    uint8_t wbytes[3];
    wbytes[0] = addr;
    wbytes[1] = (uint8_t)((data & 0xff00) >> 8);
    wbytes[2] = (uint8_t)(data & 0xff);
    nrf_i2c_twi_tx(HDC1080_Device_Adderss, wbytes, 3, 0);
}
/*******************************************************************************
  * @brief  HDC1080_Read_Frame
  * @param  uint8_t addr is point register
  * @param  uint8_t data is the place need read to
  * @retval void
 *******************************************************************************/
void HDC1080_Read_Frame(uint8_t addr, uint16_t *data)
{
    uint8_t rbytes[2];
    nrf_i2c_twi_tx(HDC1080_Device_Adderss, &addr, 1, 1);
    nrf_i2c_twi_rx(HDC1080_Device_Adderss, rbytes,2);
    *data = (((uint16_t)rbytes[0]) << 8) + rbytes[1];
    
}
/*******************************************************************************
  * @brief  HDC1080_Read_Frame_Conversation
  * @param  uint8_t addr is point register
  * @param  uint8_t data is the place need read to
  * @retval void
 *******************************************************************************/
void HDC1080_Read_Frame_Conversation(uint8_t addr, uint16_t *data)
{
    uint8_t rbytes[2];
    nrf_i2c_twi_tx(HDC1080_Device_Adderss, &addr, 1, 1);
    nrf_delay_ms(7); // conversation time Temperature typical max 6.35ms , Humidity typical max 6.50ms
    nrf_i2c_twi_rx(HDC1080_Device_Adderss, rbytes,2);
    *data = (((uint16_t)rbytes[0]) << 8) + rbytes[1];
    
}
/*******************************************************************************
  * @brief  HDC1080_Read_Temperature_and_Humidity  Where Settting Mode to 1
  * @param  uint8_t temperature is the temperature from hdc1080
  * @param  uint8_t humidity is the humidity from hdc1080
  * @retval void
 *******************************************************************************/
void HDC1080_Read_Temperature_and_Humidity(uint16_t *temperature, uint16_t *humidity)
{
    uint8_t addr = HDC1080_Temperature;
    uint8_t rbytes[4];
    uint16_t data;
    nrf_i2c_twi_tx(HDC1080_Device_Adderss, &addr, 1, 1);
    nrf_delay_ms(20); // conversation time Temperature typical max 6.35ms , Humidity typical max 6.50ms
    nrf_i2c_twi_rx(HDC1080_Device_Adderss, rbytes, 4);
    data  = (((uint16_t)rbytes[0]) << 8) + rbytes[1];
    *temperature = (int16_t)((data/65536.0)*165-40);
    data  = (((uint16_t)rbytes[2]) << 8) + rbytes[3];
    *humidity = (int16_t)((data/65536.0)*100);    
    
}



/*******************************************************************************
  * @brief  HDC1080_Soft_Reset 
  * @param  void
  * @retval void
 *******************************************************************************/
void HDC1080_Soft_Reset(void)
{
    uint16_t data;
    ((hdc1080_config_reg_t *)&data)->RST = 1;

    HDC1080_Write_Frame(HDC1080_Config,data);
    nrf_delay_ms(20);//there should be waiting for more than 15 ms.
}


/*******************************************************************************
  * @brief  HDC1080_Read_Temperature 
  * @param  void
  * @retval int16_t tempr value
 *******************************************************************************/
int16_t HDC1080_Read_Temperature(void)
{
    uint16_t data;
    
    HDC1080_Read_Frame_Conversation(HDC1080_Temperature, &data);
    return (int16_t)((data/65536.0)*165-40);
}
/*******************************************************************************
  * @brief  HDC1080_Read_Humidity 
  * @param  void
  * @retval int16_t Humidity value
 *******************************************************************************/
uint16_t HDC1080_Read_Humidity(void)
{
    uint16_t data;
    
    HDC1080_Read_Frame_Conversation(HDC1080_Humidity, &data);
    return (int16_t)((data/65536.0)*100);
}
/*******************************************************************************
  * @brief  HDC1080_Read_Configuration 
  * @param  void
  * @retval Config value
 *******************************************************************************/
uint16_t HDC1080_Read_Config(void)
{
    uint16_t data;
    
    HDC1080_Read_Frame(HDC1080_Config, &data);
    return data;
}
/*******************************************************************************
  * @brief  HDC1080_Read_MODE 
  * @param  void
  * @retval MODE value
 *******************************************************************************/
uint16_t HDC1080_Read_MODE(void)
{
    uint16_t data;
    
    HDC1080_Read_Frame(HDC1080_Config, &data);
    return ((hdc1080_config_reg_t *)&data)->MODE;
}
/*******************************************************************************
  * @brief  HDC1080_Set_MODE 
  * @param  void
  * @retval MODE value
 *******************************************************************************/
void HDC1080_Set_MODE(uint8_t mode)
{
    uint16_t data;
    
    HDC1080_Read_Frame(HDC1080_Config, &data);
    ((hdc1080_config_reg_t *)&data)->MODE = mode?1:0; 
    
    HDC1080_Write_Frame(HDC1080_Config,data);
}
/*******************************************************************************
  * @brief  HDC1080_Read_ManufacturerID 
  * @param  void
  * @retval ManufacturerID 
 *******************************************************************************/
uint16_t HDC1080_Read_ManufacturerID(void)
{
    uint16_t data;
    
    HDC1080_Read_Frame(HDC1080_ManufacturerID, &data);
    return data;
}
/*******************************************************************************
  * @brief  HDC1080_Read_DeviceID
  * @param  void
  * @retval DeviceID
 *******************************************************************************/
uint16_t HDC1080_Read_DeviceID(void)
{
    uint16_t data  = 0;   

    HDC1080_Read_Frame(HDC1080_DEVICE_ID, &data);
    return data;
}


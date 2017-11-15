#include <stdint.h>
#include "juma_sdk_api.h"
#include "MC24AA02.h"




/*******************************************************************************
  * @brief  MC24AA02_Random_Read
  * @param  uint8_t addr is point register
  * @retval uint8_t data is the place need read to
 *******************************************************************************/
uint8_t MC24AA02_Random_Read(uint8_t addr)
{
    uint8_t buf[1];
    buf[0] = addr;
    nrf_i2c_twi_tx(MC24AA02_Device_Adderss, buf, 1, 1);
    buf[0] = 0;
    nrf_i2c_twi_rx(MC24AA02_Device_Adderss, buf,1);
    return buf[0];
}
/*******************************************************************************
  * @brief  MC24AA02_Write_Byre
  * @param  uint8_t addr is point register
  * @param  uint8_t data is the content need to write
  * @retval void
 *******************************************************************************/
void MC24AA02_Write_Byre(uint8_t addr, uint8_t data)
{
    uint8_t wbytes[2];
    wbytes[0] = addr;
    wbytes[1] = data;
    nrf_i2c_twi_tx(MC24AA02_Device_Adderss, wbytes, 2, 0);
}
/*******************************************************************************
  * @brief  MC24AA02_Read_Sequential
  * @param  uint8_t addr is point register
  * @param  uint8_t data is the place need read to
  * @param  uint8_t len is the place length need read 
  * @retval void
 *******************************************************************************/
void MC24AA02_Read_Sequential(uint8_t addr, uint8_t *data, uint8_t len)
{
    nrf_i2c_twi_tx(MC24AA02_Device_Adderss, &addr, 1, 0);
    nrf_i2c_twi_rx(MC24AA02_Device_Adderss, data, len);   
}

/*******************************************************************************
  * @brief  MC24AA02_Read_CurSequential
  * @param  uint8_t data is the place need read to
  * @param  uint8_t len is the place length need read 
  * @retval void
 *******************************************************************************/
void MC24AA02_Read_CurSequential(uint8_t *data, uint8_t len)
{
    nrf_i2c_twi_rx(MC24AA02_Device_Adderss, data, len);   
}

/*******************************************************************************
  * @brief  MC24AA02_Write_Page
  * @param  uint8_t data is the content need to write
  * @retval void
 *******************************************************************************/
void MC24AA02_Write_Page(MC24AA02_Write_Page_t data)
{
    nrf_i2c_twi_tx(MC24AA02_Device_Adderss, (uint8_t *)&data, sizeof(data), 0);
}

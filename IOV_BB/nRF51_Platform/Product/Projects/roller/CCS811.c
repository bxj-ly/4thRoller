#include <stdint.h>
#include "juma_sdk_api.h"
#include "CCS811.h"



/*******************************************************************************
  * @brief  CCS811_Wake_Up
 *******************************************************************************/
void CCS811_Wake_Up(void)
{
    gpio_setup(CCS811_NWAKE_PIN, GPIO_OUTPUT);
    gpio_write(CCS811_NWAKE_PIN, 0);
}
/*******************************************************************************
  * @brief  CCS811_Read_Byte
  * @param  uint8_t addr is point register
  * @retval uint8_t data is the place need read to
 *******************************************************************************/
uint8_t CCS811_Read_Byte(uint8_t addr)
{
    uint8_t buf[1];
    buf[0] = addr;
    nrf_i2c_twi_tx(CCS811_Device_Adderss, buf, 1, 1);
    buf[0] = 0;
    nrf_i2c_twi_rx(CCS811_Device_Adderss, buf,1);
    return buf[0];
}
/*******************************************************************************
  * @brief  CCS811_Write_Byre
  * @param  uint8_t addr is point register
  * @param  uint8_t data is the content need to write
  * @retval void
 *******************************************************************************/
void CCS811_Write_Byre(uint8_t addr, uint8_t data)
{
    uint8_t wbytes[2];
    wbytes[0] = addr;
    wbytes[1] = data;
    nrf_i2c_twi_tx(CCS811_Device_Adderss, wbytes, 2, 0);
}



#include "stdio.h"
#include "nrf_drv_twi.h"
#include "app_util_platform.h"

#include "twi_master.h"
//#include "twi_master_config.h"

 /* TWI instance. */
 nrf_drv_twi_t app_twi_instance = NRF_DRV_TWI_INSTANCE(0);
 
void nrf_i2c_bus_init(void){
    uint32_t err_code;
    const nrf_drv_twi_config_t twi_config = {
        .scl                = 7,
        .sda                = 8,
        .frequency          = NRF_TWI_FREQ_400K, //was 100K
        .interrupt_priority = APP_IRQ_PRIORITY_HIGH
    };

    err_code = nrf_drv_twi_init(&app_twi_instance, &twi_config, NULL, NULL); //todo add event handler
    APP_ERROR_CHECK(err_code);
    nrf_drv_twi_enable(&app_twi_instance);    
}

void i2c_bus_init(void)
{
    nrf_i2c_bus_init();
}

int8_t nrf_i2c_twi_tx(uint8_t addr, uint8_t *  p_data,uint8_t len, uint8_t no_stop)
{
    if( no_stop != 0)
        return nrf_drv_twi_tx(&app_twi_instance, addr, p_data, len, true);
    else
        return nrf_drv_twi_tx(&app_twi_instance, addr, p_data, len, false);
}

int8_t nrf_i2c_twi_rx(uint8_t addr, uint8_t *  p_data,uint8_t len)
{
    return nrf_drv_twi_rx(&app_twi_instance, addr, p_data, len);
}

 /*	\Brief: The function is used as I2C bus write
 *	\Return : Status of the I2C write
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register, where data is to be written
 *	\param reg_data : It is a value held in the array,
 *		which is written in the register
 *	\param cnt : The no of bytes of data to be written
 */
int8_t  I2C_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
     ret_code_t err_code = nrf_drv_twi_tx(&app_twi_instance, dev_addr, reg_data, cnt, false);
     APP_ERROR_CHECK(err_code);
     if(err_code == NRF_SUCCESS)
     {
         return 0;
     }
     return -1;
}
int8_t  I2C_bus_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t value)
{
	uint8_t writeData[2];
	writeData[0] = reg_addr;
	writeData[1] = value;
	ret_code_t err_code = nrf_drv_twi_tx(&app_twi_instance, dev_addr, writeData, 2, false);
    APP_ERROR_CHECK(err_code);

    if(err_code == NRF_SUCCESS)
    {
        return 0;
    }
	return -1;
}

 /*	\Brief: The function is used as I2C bus read
 *	\Return : Status of the I2C read
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register, where data is going to be read
 *	\param reg_data : This is the data read from the sensor, which is held in an array
 *	\param cnt : The no of data to be read
 */
int8_t  I2C_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
     /* initialize data to zero so we don't return random values. */
     for (int i = 0; i < cnt; i++) 
     {
         reg_data[i] = 0;
     }
     
     /* Write: register address we want to start reading from */
     if (NRF_SUCCESS == nrf_drv_twi_tx(&app_twi_instance, dev_addr, &reg_addr, 1, TWI_ISSUE_STOP))
     {
         /*  Read: the number of bytes requested. */
         if (NRF_SUCCESS == nrf_drv_twi_rx(&app_twi_instance, dev_addr, reg_data, cnt))
         {           
             /*  Read succeeded. */
             return 0;
         }
     }
     
     /* read or write failed. */
     return -1;

}

uint8_t  I2C_bus_read_byte(uint8_t dev_addr, uint8_t reg_addr)
{
	uint8_t data;
	data = reg_addr;
	ret_code_t err_code = nrf_drv_twi_tx(&app_twi_instance, dev_addr, &data, 1, TWI_ISSUE_STOP);
	APP_ERROR_CHECK(err_code);
	err_code = nrf_drv_twi_rx(&app_twi_instance, dev_addr, &data, 1);
	APP_ERROR_CHECK(err_code);
	return data;
}

#if 0
 /*	\Brief: The function is used as I2C bus write
 *	\Return : Status of the I2C write
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register, where data is to be written
 *	\param reg_data : It is a value held in the array,
 *		which is written in the register
 *	\param cnt : The no of bytes of data to be written
 */
int8_t  I2C_bus_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
    /* ????? */
    if (twi_master_transfer(dev_addr, reg_data, cnt, TWI_DONT_ISSUE_STOP))
    {
        return 0;
    }

    /* read or write failed. */
    return -1;
}

int8_t  I2C_bus_write_byte(uint8_t dev_addr, uint8_t reg_addr, uint8_t value)
{
    uint8_t data[2];
    data[0] = reg_addr;
    data[1] = value;

    /*  Write: register protocol */
    if (twi_master_transfer(dev_addr, data, 2, TWI_ISSUE_STOP))
    {
        return 0;    
    }

    /* read or write failed. */
    return -1;
}


 /*	\Brief: The function is used as I2C bus read
 *	\Return : Status of the I2C read
 *	\param dev_addr : The device address of the sensor
 *	\param reg_addr : Address of the first register, where data is going to be read
 *	\param reg_data : This is the data read from the sensor, which is held in an array
 *	\param cnt : The no of data to be read
 */
int8_t  I2C_bus_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
    /* initialize data to zero so we don't return random values. */
    for (int i = 0; i < cnt; i++) 
    {
        reg_data[i] = 0;
    }

    /* Write: register address we want to start reading from */
    if (twi_master_transfer(dev_addr, &reg_addr, 1, TWI_DONT_ISSUE_STOP))
    {
        /*  Read: the number of bytes requested. */
        if (twi_master_transfer(dev_addr | TWI_READ_BIT, reg_data, cnt, TWI_ISSUE_STOP))
        {           
            /*  Read succeeded. */
            return 0;
        }
    }

    /* read or write failed. */
    return -1;

}

uint8_t  I2C_bus_read_byte(uint8_t dev_addr, uint8_t reg_addr)
{
    uint8_t data = 0;

    if (I2C_bus_read(dev_addr, reg_addr, &data, 1)) 
    {
        return data;
    }
    return 0;
}
#endif

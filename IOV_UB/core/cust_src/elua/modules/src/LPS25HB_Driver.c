/**
  ******************************************************************************
  * @file    LPS25HB_Driver.c
  * @author  HESA Application Team
  * @version 1.0.0
  * @date    07/04/2014
  * @brief   LPS25HB driver file
  
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * THIS SOURCE CODE IS PROTECTED BY A LICENSE.
  * FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
  * IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  
  ******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "LPS25HB_Driver.h"
#ifdef  USE_FULL_ASSERT_LPS25HB
#include <stdio.h>
#endif

/** @addtogroup Environmental_Sensor
  * @{
  */

/** @defgroup LPS25HB_DRIVER
* @brief LPS25HB DRIVER
  * @{
  */


/** @defgroup LPS25HB_Public_Functions
* @{
*/

/* Exported Functions -------------------------------------------------------------*/
/** @defgroup LPS25HB_Exported_Functions
  * @{
  */
	
	/**
* @brief  Init the HAL layer.
  * @param  None
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB_HalInit(void)
{
  PlatformI2CParam i2cParam;
  
  i2cParam.speed = PLATFORM_I2C_SPEED_SLOW;
  i2cParam.slaveAddr = LPS25HB_ADDRESS;
  
  platform_i2c_setup(2, &i2cParam);
}
  
/**
* @brief  DeInit the HAL layer.
  * @param  None
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
void LPS25HB_HalDeInit(void)
{
}
  
/**
* @brief  Read LPS25HB Registers
* @param  uint32_t RegAddr:       address of the first register to read
* @param  uint32_t NumByteToRead:   number of registers to read
* @param  uint8_t *Data:          pointer to the destination data buffer
* @retval LPS25HB_ERROR or LPS25HB_OK 
*/
// the user must redefine the proper LPS25HB_ReadReg
LPS25HB_Error_et LPS25HB_ReadReg(const uint8_t RegAddr, uint32_t NumByteToRead, uint8_t* Data)
{
  platform_i2c_recv_data(2, LPS25HB_ADDRESS, &RegAddr, Data, NumByteToRead); 
}

/**
* @brief  Write LPS25HB Registers
* @param  uint32_t RegAddr:      address of the register to wr te
* @param  uint8_t *Data:         pointer to the source data buffer
* @param  uint32_t NumByteToWrite:           Number of bytes to write
* @retval LPS25HB_ERROR or LPS25HB_OK 
*/
// the user must redefine the proper LPS25HB_WriteReg
LPS25HB_Error_et LPS25HB_WriteReg(const uint8_t RegAddr, uint32_t NumByteToWrite, uint8_t* Data)  
{
  platform_i2c_send_data(2, LPS25HB_ADDRESS, &RegAddr, Data, NumByteToWrite);
}

/**
  * @brief  Read identification code by WHO_AM_I register
  * @param  Buffer to empty by Device identification Value.
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_DeviceID(uint8_t* deviceid)
{  
  if(LPS25HB_ReadReg(LPS25HB_WHO_AM_I_REG, 1, deviceid))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}


/**
  * @brief  Get the LPS25HB driver version.
  * @param  None
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_DriverVersion(LPS25HB_DriverVersion_st *Version)
{  
  Version->Major = LPS25HB_DriverVersion_Major;
  Version->Minor = LPS25HB_DriverVersion_Minor;
  Version->Point = LPS25HB_DriverVersion_Point;
  
  return LPS25HB_OK;
}

/**
  * @brief  Set LPS25HB Pressure and Temperature Resolution Mode
  * @param  Pressure Resolution
	* @param  Temperature Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_Avg(LPS25HB_Avgp_et avgp,LPS25HB_Avgt_et avgt )
{  
  uint8_t tmp;
	
  LPS25HB_assert_param(IS_LPS25HB_AVGT(avgt));
  LPS25HB_assert_param(IS_LPS25HB_AVGP(avgp));
  
  if(LPS25HB_ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~(LPS25HB_AVGT_MASK | LPS25HB_AVGP_MASK);
  tmp |= (uint8_t)avgp;
  tmp |= (uint8_t)avgt;
  
  if(LPS25HB_WriteReg(LPS25HB_RES_CONF_REG, 1, &tmp) )
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

 
/**
  * @brief  Exit the shutdown mode for LPS25HB.
  * @param  None
  * @retval None
*/

LPS25HB_Error_et LPS25HB_Activate(void)
{  
  if(LPS25HB_Set_PowerDownMode(LPS25HB_SET))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief  Enter the shutdown mode for LPS25HB.
  * @param  None
  * @retval None
*/
LPS25HB_Error_et LPS25HB_DeActivate(void)
{  
  if(LPS25HB_Set_PowerDownMode(LPS25HB_RESET))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief  Set LPS25HB Pressure Resolution Mode
  * @param  Pressure Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_AvgP(LPS25HB_Avgp_et avgp)
{  
  uint8_t tmp;  
  
  LPS25HB_assert_param(IS_LPS25HB_AVGP(avgp));
	
  if(LPS25HB_ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_AVGP_MASK;
  tmp |= (uint8_t)avgp;
  
  if(LPS25HB_WriteReg(LPS25HB_RES_CONF_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief  Set LPS25HB Temperature Resolution Mode
  * @param  Temperature Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_AvgT(LPS25HB_Avgt_et avgt)
{  
  uint8_t tmp;
  
  LPS25HB_assert_param(IS_LPS25HB_AVGT(avgt));
  
  if(LPS25HB_ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_AVGT_MASK;
  tmp |= (uint8_t)avgt;
  
  if(LPS25HB_WriteReg(LPS25HB_RES_CONF_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief  Get LPS25HB Pressure Resolution Mode
  * @param  Pressure Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_AvgP(LPS25HB_Avgp_et* avgp)
{
  uint8_t tmp;
  
  if(LPS25HB_ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
  *avgp = (LPS25HB_Avgp_et)(tmp & LPS25HB_AVGP_MASK);
  
  return LPS25HB_OK;
}


/**
  * @brief  Get LPS25HB Temperature Resolution Mode
  * @param  Temperature Resolution
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_AvgT(LPS25HB_Avgt_et* avgt)
{  
  uint8_t tmp;
 
  if(LPS25HB_ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp) )
    return LPS25HB_ERROR;
  
  *avgt = (LPS25HB_Avgt_et)(tmp & LPS25HB_AVGT_MASK);
	
  return LPS25HB_OK;
}


/**
  * @brief  Set LPS25HB Output Data Rate
  * @param  Output Data Rate
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_Odr(LPS25HB_Odr_et odr)
{  
  uint8_t tmp;
  
  LPS25HB_assert_param(IS_LPS25HB_ODR(odr));
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_ODR_MASK;
  tmp |= (uint8_t)odr;
		  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
		  
  return LPS25HB_OK;
}


/**
  * @brief  Get LPS25HB Output Data Rate
  * @param  Buffer to empty with Output Data Rate
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_Odr(LPS25HB_Odr_et* odr)
{
  uint8_t tmp;
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  *odr = (LPS25HB_Odr_et)(tmp & LPS25HB_ODR_MASK);
	   
  return LPS25HB_OK;
}

/**
  * @brief  SET/RESET LPS25HB Power Down Mode bit
  * @param  LPS25HB_SET (Active Mode)/LPS25HB_RESET (Power Down Mode)
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_PowerDownMode(LPS25HB_BitStatus_et pd)
{
  uint8_t tmp;
  
  LPS25HB_assert_param(IS_LPS25HB_BitStatus(pd));
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
	 
  tmp &= ~LPS25HB_PD_MASK;
  tmp |= ((uint8_t)pd)<<LPS25HB_PD_BIT;
  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
	
  return LPS25HB_OK;
}

/**
  * @brief  Set Block Data Mode
  * @detail It is recommended to set BDU bit to ?1?.
  * @detail This feature avoids reading LSB and MSB related to different samples.
  * @param 	LPS25HB_BDU_CONTINUOS_UPDATE, LPS25HB_BDU_NO_UPDATE
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */

LPS25HB_Error_et LPS25HB_Set_Bdu(LPS25HB_Bdu_et bdu)
{  
  uint8_t tmp;
   
  LPS25HB_assert_param(IS_LPS25HB_BDUMode(bdu));
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_BDU_MASK;
   tmp |= ((uint8_t)bdu);
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief  Get Block Data Mode
  * @param 	Buffer to empty whit thw bdu mode read from sensor
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_Bdu(LPS25HB_Bdu_et* bdu)
{  
  uint8_t tmp;
	
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  *bdu = (LPS25HB_Bdu_et)(tmp & LPS25HB_BDU_MASK);
	   
  return LPS25HB_OK;
}

/**
  * @brief  Enable/Disable Interrupt Circuit.
  * @param  LPS25HB_ENABLE/LPS25HB_DISABLE
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_InterruptCircuitEnable(LPS25HB_State_et diff_en) 
{  
  uint8_t tmp;
  
  LPS25HB_assert_param(IS_LPS25HB_State(diff_en));
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_DIFF_EN_MASK;
    tmp |= ((uint8_t)diff_en)<<LPS25HB_DIFF_EN_BIT;
  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
* @brief  Get the Interrupt Circuit bit.
* @param Buffer to empty whit thw diff_en mode read from sensor
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_InterruptCircuitEnable(LPS25HB_State_et* diff_en)
{  
  uint8_t tmp;
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  *diff_en= (LPS25HB_State_et)((tmp & LPS25HB_DIFF_EN_MASK)>>LPS25HB_DIFF_EN_BIT);
  
  return LPS25HB_OK;
}


/**
* @brief  Set ResetAutoZero Function bit
* @details REF_P reg (@0x08..0A) set pressure reference to default tmp RPDS reg (0x39/3A).
* @param  None
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_ResetAZ(void)
{  
  uint8_t tmp;
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
	/* Set the RESET_AZ bit*/
	/* RESET_AZ is self cleared*/
  tmp |= LPS25HB_RESET_AZ_MASK;
		
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
	  
  return LPS25HB_OK;
}

/**
* @brief  Set SPI mode: 3 Wire Interface or 4 Wire Interface
* @param   LPS25HB_SPI_3_WIRE, LPS25HB_SPI_4_WIRE
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_SpiInterface(LPS25HB_SPIMode_et spimode)
{  
  uint8_t tmp;
   
  LPS25HB_assert_param(IS_LPS25HB_SPIMode(spimode));
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_SIM_MASK;
  tmp |= (uint8_t)spimode;
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}


/**
* @brief  Get SPI mode: 3 Wire Interface or 4 Wire Interface
* @param  Buffer to empty with spi mode read from Sensor
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_SpiInterface(LPS25HB_SPIMode_et* spimode)
{  
  uint8_t tmp;
 
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
    return LPS25HB_ERROR;

  *spimode = (LPS25HB_SPIMode_et)(tmp & LPS25HB_SIM_MASK);
	  
  return LPS25HB_OK;
}

/**
  * @brief  Enable/Disable I2C Mode 
  * @param State: Enable (reset bit)/ Disable (set bit)
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_I2C(LPS25HB_State_et statei2c)
{  
  uint8_t tmp;
	
  LPS25HB_assert_param(IS_LPS25HB_State(statei2c));
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;

  tmp &= ~LPS25HB_I2C_MASK;
  if(statei2c == LPS25HB_DISABLE) {
    tmp |= LPS25HB_I2C_MASK;
  }
  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
	
}

/**
* @brief  Set the one-shot bit in order to start acquisition when the ONE SHOT mode
          has been selected by the ODR configuration.
* @detail Once the measurement is done, ONE_SHOT bit will self-clear.
* @param  None
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_StartOneShotMeasurement(void)
{
  uint8_t tmp;
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
    
  /* Set the one shot bit */
	/* Once the measurement is done, one shot bit will self-clear*/
  tmp |= LPS25HB_ONE_SHOT_MASK;
  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
	
}

/**
* @brief  Set AutoZero Function bit
* @detail When set to ?1?, the actual pressure output is copied in the REF_P reg (@0x08..0A) 
* @param  LPS25HB_SET/LPS25HB_RESET
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_AutoZeroFunction(LPS25HB_BitStatus_et autozero) 
{  
  uint8_t tmp;
  
  LPS25HB_assert_param(IS_LPS25HB_BitStatus(autozero));
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &=  ~LPS25HB_AUTO_ZERO_MASK;
  tmp |= ((uint8_t)autozero)<<LPS25HB_AUTO_ZERO_BIT;
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief   Software Reset. Self-clearing upon completion
  * @param   None
  * @retval Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_SwReset(void) 
{  
  uint8_t tmp;
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp |= (0x01<<LPS25HB_SW_RESET_BIT);
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
* @brief  Reboot Memory Content
* @param 	 None
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_MemoryBoot(void) 
{  
  uint8_t tmp;
 
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp |= (0x01<<LPS25HB_BOOT_BIT);
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
* @brief   Software Reset ann Reboot Memory Content. 
* @detail  The device is reset to the power on configuration if the SWRESET bit is set to ?1? 
           and BOOT is set to ?1?; Self-clearing upon completion.
* @param  None
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_SwResetAndMemoryBoot(void) 
{
  uint8_t tmp;
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp |= ((0x01<<LPS25HB_SW_RESET_BIT) | (0x01<<LPS25HB_BOOT_BIT));
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief   Enable/Disable FIFO Mode 
  * @param   LPS25HB_ENABLE/LPS25HB_DISABLE
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_FifoModeUse(LPS25HB_State_et status) 
{  
  uint8_t tmp;
 
  LPS25HB_assert_param(IS_LPS25HB_State(status));
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_FIFO_EN_MASK;
  tmp |= ((uint8_t)status)<<LPS25HB_FIFO_EN_BIT;
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
* @brief    Enable/Disable FIFO Watermark Level Use 
* @param    LPS25HB_ENABLE/LPS25HB_DISABLE
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_FifoWatermarkLevelUse(LPS25HB_State_et status) 
{  
  uint8_t tmp;
   
  LPS25HB_assert_param(IS_LPS25HB_State(status));
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_WTM_EN_MASK;
  tmp |= ((uint8_t)status)<<LPS25HB_WTM_EN_BIT;
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
* @brief   Enable/Disable 1 HZ ODR Decimation
* @param   LPS25HB_ENABLE/LPS25HB_DISABLE
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_FifoMeanDecUse(LPS25HB_State_et status) 
{  
  uint8_t tmp;
   
  LPS25HB_assert_param(IS_LPS25HB_State(status));
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_FIFO_MEAN_MASK;
  tmp |= ((uint8_t)status)<<LPS25HB_FIFO_MEAN_BIT;
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;	
	
}

/**
* @brief   Enable/Disable Interrupt Active High (default tmp 0) or Low(tmp 1)
* @param   LPS25HB_ENABLE/LPS25HB_DISABLE
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_InterruptActiveLevel(LPS25HB_State_et status)
{  
  uint8_t tmp;
	
  LPS25HB_assert_param(IS_LPS25HB_State(status));
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG3, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_INT_H_L_MASK;
  tmp |= ((uint8_t)status)<<LPS25HB_INT_H_L_BIT;
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG3, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;	
}

/**
* @brief   Push-pull/open drain selection on interrupt pads. Default tmp: 0
* @param LPS25HB_PushPull/LPS25HB_OpenDrain
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_InterruptOutputType(LPS25HB_OutputType_et output)
{
  uint8_t tmp;
   
  LPS25HB_assert_param(IS_LPS25HB_OutputType(output));
   
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG3, 1, &tmp))
    return LPS25HB_ERROR;
  
  tmp &= ~LPS25HB_PP_OD_MASK;
  tmp |= (uint8_t)output;
	  
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG3, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;	
}

/**
* @brief   Set Data signal on INT1 pad control bits. Default tmp: 00
* @param   LPS25HB_DATA,LPS25HB_P_HIGH_LPS25HB_P_LOW,LPS25HB_P_LOW_HIGH
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_InterruptControlConfig(LPS25HB_OutputSignalConfig_et config)
{  
  uint8_t tmp;
	
  LPS25HB_assert_param(IS_LPS25HB_OutputSignal(config));
	
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG3, 1, &tmp))
    return LPS25HB_ERROR;

  tmp &= ~(LPS25HB_INT1_S2_MASK | LPS25HB_INT1_S1_MASK);
  tmp |= (uint8_t)config;
		
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG3, 1, &tmp))
    return LPS25HB_ERROR;

  return LPS25HB_OK;
}

/**
  * @brief   Set INT1 interrupt pins configuration
  * @param   LPS25HB_EMPTY,LPS25HB_WTM,LPS25HB_OVR,LPS25HB_DATA_READY
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_InterruptDataConfig(LPS25HB_DataSignalType_et signal)
{  
  uint8_t tmp;
	
  LPS25HB_assert_param(IS_LPS25HB_DataSignal(signal));
		
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG4, 1, &tmp))
    return LPS25HB_ERROR;
	
  tmp &= ~(LPS25HB_P1_EMPTY_MASK | LPS25HB_P1_WTM_MASK \
          | LPS25HB_P1_OVERRUN_MASK  | LPS25HB_P1_DRDY_MASK );
  tmp |= (uint8_t)signal;
				
  if(LPS25HB_WriteReg(LPS25HB_CTRL_REG4, 1, &tmp))
    
    return LPS25HB_ERROR;
 
  return LPS25HB_OK;
}

/**
  * @brief   Enable\Disable Interrupt Generation on differential pressure low and/or high event
  * @param   LPS25HB_DISABLE_INT, LPS25HB_PHE,LPS25HB_PLE,LPS25HB_PLE_PHE
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_InterruptDifferentialConfig(LPS25HB_InterruptDiffConfig_et config)
{  
  uint8_t tmp;
	
  LPS25HB_assert_param(IS_LPS25HB_InterruptDiff(config));
	
  if(LPS25HB_ReadReg(LPS25HB_INTERRUPT_CFG_REG, 1, &tmp))
    return LPS25HB_ERROR;
	
  tmp &= ~(LPS25HB_PL_E_MASK | LPS25HB_PH_E_MASK);
  tmp |= (uint8_t)config;
  if(config!=LPS25HB_DISABLE_INT){
      /* Enable DIFF_EN bit in CTRL_REG1 */
       if(LPS25HB_Set_InterruptCircuitEnable(LPS25HB_ENABLE))
          return LPS25HB_ERROR;
  }
		
  if(LPS25HB_WriteReg(LPS25HB_INTERRUPT_CFG_REG, 1, &tmp))
    return LPS25HB_ERROR;
	
  return LPS25HB_OK;
}

/**
  * @brief   Enable/Disable Latch Interrupt request into INT_SOURCE register.
  * @param   LPS25HB_ENABLE/LPS25HB_DISABLE
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_LatchInterruptRequest(LPS25HB_State_et status)
{  
  uint8_t tmp;
	
  LPS25HB_assert_param(IS_LPS25HB_State(status));
		
  if(LPS25HB_ReadReg(LPS25HB_INTERRUPT_CFG_REG, 1, &tmp))
    return LPS25HB_ERROR;
	
  tmp &= ~LPS25HB_LIR_MASK;
  tmp |= (((uint8_t)status)<<LPS25HB_LIR_BIT);
		
  if(LPS25HB_WriteReg(LPS25HB_INTERRUPT_CFG_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief   Get the Interrupt Generation on differential pressure status event.
  * @detail  The INT_SOURCE register is cleared by reading it.
  * @param  Status Event Flag: PH,PL,IA
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_InterruptDifferentialEventStatus(LPS25HB_InterruptDiffStatus_st* interruptsource)
{  
  uint8_t tmp;
	
  if(LPS25HB_ReadReg(LPS25HB_INTERRUPT_SOURCE_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
  interruptsource->PH = (uint8_t)(tmp & LPS25HB_PH_MASK); 
  interruptsource->PL = (uint8_t)((tmp & LPS25HB_PL_MASK)>>LPS25HB_PL_BIT);
  interruptsource->IA = (uint8_t)((tmp & LPS25HB_IA_MASK)>>LPS25HB_IA_BIT);

  return LPS25HB_OK;
}

/**
  * @brief   Get the status of Pressure and Temperature data 
  * @param   Data Status Flag:  TempDataAvailable, TempDataOverrun, PressDataAvailable, PressDataOverrun
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_DataStatus(LPS25HB_DataStatus_st* datastatus)
{
  uint8_t tmp;
  
  if(LPS25HB_ReadReg(LPS25HB_STATUS_REG, 1, &tmp))
    return LPS25HB_ERROR;
	
  datastatus->TempDataAvailable = (tmp & LPS25HB_TDA_MASK);
  datastatus->PressDataAvailable = (uint8_t)((tmp & LPS25HB_PDA_MASK)>>LPS25HB_PDA_BIT);  
  datastatus->TempDataOverrun = (uint8_t)((tmp & LPS25HB_TOR_MASK)>>LPS25HB_TOR_BIT);   
  datastatus->PressDataOverrun = (uint8_t)((tmp & LPS25HB_POR_MASK)>>LPS25HB_POR_BIT);  
  
  return LPS25HB_OK;
}


/**
* @brief    Get the raw pressure tmp
* @detail   The data are expressed as PRESS_OUT_H/_L/_XL in 2�s complement.
            Pout(hPA)=PRESS_OUT / 4096
* @param    The buffer to empty with the pressure raw tmp
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_RawPressure(int32_t *raw_press)
{
  uint8_t buffer[3];
  uint32_t tmp = 0;  
	
  if(LPS25HB_ReadReg(LPS25HB_PRESS_OUT_XL_REG, 3, buffer))
      return LPS25HB_ERROR;
  
  /* Build the raw data */
  for(uint8_t i=0; i<3; i++)
    tmp |= (((uint32_t)buffer[i]) << (8*i));
	
  /* convert the 2's complement 24 bit to 2's complement 32 bit */
  if(tmp & 0x00800000)
    tmp |= 0xFF000000;
  
  *raw_press = ((int32_t)tmp);
  
  return LPS25HB_OK;   
}

/**
* @brief    Get the Pressure value in hPA.
* @detail   The data are expressed as PRESS_OUT_H/_L/_XL in 2�s complement.
            Pout(hPA)=PRESS_OUT / 4096
* @param    The buffer to empty with the pressure value that must be divided by 100 to get the value in hPA
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_Pressure(int32_t* Pout)
{  
  int32_t raw_press;
 
  if(LPS25HB_Get_RawPressure(&raw_press))
      return LPS25HB_ERROR;
  
  *Pout = (raw_press*100)/4096;
  
  return LPS25HB_OK;
}

/**
* @brief    Get the Raw Temperature tmp.
* @detail   Temperature data are expressed as TEMP_OUT_H&TEMP_OUT_L as 2�s complement number.
            Tout(degC)=42.5+ (TEMP_OUT/480)
* @param    Buffer to empty with the temperature raw tmp.
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_RawTemperature(int16_t* raw_data)
{
  uint8_t buffer[2];
  uint16_t tmp;
	  
  if(LPS25HB_ReadReg(LPS25HB_TEMP_OUT_L_REG, 2, buffer))
      return LPS25HB_ERROR;
	
  /* Build the raw tmp */
  tmp = (((uint16_t)buffer[1]) << 8) + (uint16_t)buffer[0];
 
  *raw_data = ((int16_t)tmp);
  
  return LPS25HB_OK;    
}


/**
* @brief    Get the Temperature value in �C.
* @detail   Temperature data are expressed as TEMP_OUT_H&TEMP_OUT_L as 2�s complement number.
*            Tout(degC)=42.5+ (TEMP_OUT/480)
* @param    Buffer to empty with the temperature value that must be divided by 10 to get the value in �C
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_Temperature(int16_t* Tout)
{  
  int16_t raw_data;
  
  if(LPS25HB_Get_RawTemperature(&raw_data))
    return LPS25HB_ERROR;
  
  *Tout = raw_data/48 + 425;

  return LPS25HB_OK;
}

/**
* @brief    Get the threshold tmp used for pressure interrupt generation (hPA).
* @detail   THS_P=THS_P_H&THS_P_L and is expressed as unsigned number.
            P_ths(hPA)=(THS_P)/16.							
* @param    Buffer to empty with the pressure threshold in hPA
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_PressureThreshold(int16_t* P_ths)
{
  uint8_t tempReg[2];
	  
  if(LPS25HB_ReadReg(LPS25HB_THS_P_LOW_REG, 2, tempReg))
    return LPS25HB_ERROR;
 
  *P_ths= (((((uint16_t)tempReg[1])<<8) + tempReg[0])/16);
  
  return LPS25HB_OK;	
}

/**
* @brief    Set the threshold tmp used for pressure interrupt generation (hPA).
* @detail   THS_P=THS_P_H&THS_P_L and is expressed as unsigned number.
            P_ths(hPA)=(THS_P)/16.							
* @param    Pressure threshold in hPA
* @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_PressureThreshold(int16_t P_ths)
{  
  uint8_t buffer[2];
  
  buffer[0] = (uint8_t)(16 * P_ths);
  buffer[1] = (uint8_t)(((uint16_t)(16 * P_ths))>>8);   
  
  if(LPS25HB_WriteReg(LPS25HB_THS_P_LOW_REG, 2, buffer))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;	
}


/**
* @brief  Set Fifo Mode					
* @param  LPS25HB_FIFO_BYPASS_MODE, LPS25HB_FIFO_MODE, LPS25HB_FIFO_STREAM_MODE, LPS25HB_FIFO_TRIGGER_STREAMTOFIFO_MODE, 
          LPS25HB_FIFO_TRIGGER_BYPASSTOSTREAM_MODE,LPS25HB_FIFO_MEAN_MODE, LPS25HB_FIFO_TRIGGER_BYPASSTOFIFO_MODE
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_FifoMode(LPS25HB_FifoMode_et fifomode) 
{  
  uint8_t tmp;  
	
  LPS25HB_assert_param(IS_LPS25HB_FifoMode(fifomode));
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))
		return LPS25HB_ERROR;

  tmp &= ~LPS25HB_FMODE_MASK;
  tmp |= (uint8_t)fifomode;

  if(LPS25HB_WriteReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))           
		return LPS25HB_ERROR;  
	 
  return LPS25HB_OK;
}

/**
  * @brief    Get Fifo Mode					
* @param 	  buffer to empty with fifo mode tmp
  * @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_FifoMode(LPS25HB_FifoMode_et* fifomode) 
{  
  uint8_t tmp;  
  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))
      return LPS25HB_ERROR;
                    
  tmp &= ~LPS25HB_WTM_POINT_MASK;
  *fifomode = (LPS25HB_FifoMode_et)tmp;
  	 
  return LPS25HB_OK;
}

/**
  * @brief    Get the Fifo Status			
  * @param 	  Status Flag: FIFO_WTM,FIFO_EMPTY,FIFO_FULL,FIFO_LEVEL
  * @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_FifoStatus(LPS25HB_FifoStatus_st* status)
{  
  uint8_t tmp;
	
  if(LPS25HB_ReadReg(LPS25HB_STATUS_FIFO_REG, 1, &tmp))
    return LPS25HB_ERROR;
	 
  status->FIFO_WTM = (uint8_t)((tmp & LPS25HB_WTM_FIFO_MASK)>>LPS25HB_WTM_FIFO_BIT);
  status->FIFO_FULL = (uint8_t)((tmp & LPS25HB_FULL_FIFO_MASK)>>LPS25HB_FULL_FIFO_BIT);
  status->FIFO_EMPTY = (uint8_t)((tmp & LPS25HB_EMPTY_FIFO_MASK)>>LPS25HB_EMPTY_FIFO_BIT);
  status->FIFO_LEVEL = (uint8_t)(tmp & LPS25HB_DIFF_POINT_MASK);

  return LPS25HB_OK;
}

/**
  * @brief    Set Watermark Value
  * @param 	  wtmlevel = [0,31]
  * @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_FifoWatermarkLevel(uint8_t wtmlevel)
{  
  uint8_t tmp; 

  LPS25HB_assert_param(IS_LPS25HB_WtmLevel(wtmlevel));

  if(LPS25HB_ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))
			return LPS25HB_ERROR;

  tmp &= ~LPS25HB_WTM_POINT_MASK;
  tmp |= wtmlevel;

  if(LPS25HB_WriteReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))           
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}

/**
  * @brief    Get Watermark Value
* @param 	  wtmlevel tmp read from sensor
  * @retval   Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_FifoWatermarkLevel(uint8_t *wtmlevel)
{  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_FIFO_REG, 1, wtmlevel))
			return LPS25HB_ERROR;

  *wtmlevel &= LPS25HB_WTM_POINT_MASK;
  
  return LPS25HB_OK;
}

/**
  * @brief   Set the number of sample to perform moving average when FIFO_MEAN_MODE is used
  * @param 	 LPS25HB_FIFO_SAMPLE_2,LPS25HB_FIFO_SAMPLE_4,LPS25HB_FIFO_SAMPLE_8,LPS25HB_FIFO_SAMPLE_16,LPS25HB_FIFO_SAMPLE_32
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_FifoSampleSize(LPS25HB_FifoMeanModeSample_et samplesize)
{  
  uint8_t tmp; 
	
  LPS25HB_assert_param(IS_LPS25HB_FifoMeanModeSample(samplesize));

  if(LPS25HB_ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))
	return LPS25HB_ERROR;

  tmp &= ~LPS25HB_WTM_POINT_MASK;
  tmp |= (uint8_t)samplesize;


  if(LPS25HB_WriteReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))           
	return LPS25HB_ERROR;

	
  return LPS25HB_OK;	
}

/**
  * @brief   Get the number of sample to perform moving average when FIFO_MEAN_MODE is used
* @param 	 buffer to empty with sample size tmp
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_FifoSampleSize(LPS25HB_FifoMeanModeSample_et* samplesize)
{  
  uint8_t tmp; 
	
  if(LPS25HB_ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))
	return LPS25HB_ERROR;

  tmp &= LPS25HB_WTM_POINT_MASK;
  *samplesize = (LPS25HB_FifoMeanModeSample_et)tmp;
	
  return LPS25HB_OK;	
}

/**
* @brief   Get the reference pressure after soldering for computing differential pressure (hPA)
* @param   buffer to empty with the he pressure tmp (hPA)
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_PressureOffsetValue(int16_t *pressoffset)
{  
  uint8_t buffer[2];
  int16_t raw_press;  
 
  if(LPS25HB_ReadReg(LPS25HB_RPDS_L_REG, 2, buffer))
      return LPS25HB_ERROR;
	
  raw_press = (int16_t)((((uint16_t)buffer[1]) << 8) + (uint16_t)buffer[0]);
  
  *pressoffset = (raw_press*100)/4096;	
	  
  return LPS25HB_OK;	
}

/**
* @brief   Set Generic Configuration
* @param   Struct to empty with the chosen tmp
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_GenericConfig(LPS25HB_ConfigTypeDef_st* pxLPS25HBInit)
{  
      /* Step 1. Init REF_P register*/
      /* The REF_P is the Reference Pressure. Its reset tmp is 0x00*/
      /* The REF_P will be set to the defualt RPDS (0x39h) tmp  if Reset_AZ is enabled.*/
      /* The REF_P will be set the actual pressure output if AutoZero is enabled*/

      if((pxLPS25HBInit->Reset_AZ)==LPS25HB_ENABLE){
              if(LPS25HB_ResetAZ())
                      return LPS25HB_ERROR;
      }
      else if((pxLPS25HBInit->AutoZero)==LPS25HB_ENABLE){
            if(LPS25HB_Set_AutoZeroFunction(LPS25HB_SET))
                      return LPS25HB_ERROR;
      }
      
      /* Step 2. Init the Pressure and Temperature Resolution*/
      if(LPS25HB_Set_Avg(pxLPS25HBInit->PressResolution,pxLPS25HBInit->TempResolution))
              return LPS25HB_ERROR;
              
      /* Step 3. Init the Output Data Rate*/
      if(LPS25HB_Set_Odr(pxLPS25HBInit->OutputDataRate))
              return LPS25HB_ERROR;

      /*Step 4. BDU bit is used to inhibit the output registers update between the reading of upper and
              lower register parts. In default mode (BDU = ?0?), the lower and upper register parts are
              updated continuously. If it is not sure to read faster than output data rate, it is recommended
              to set BDU bit to ?1?. In this way, after the reading of the lower (upper) register part, the
              content of that output registers is not updated until the upper (lower) part is read too.
              This feature avoids reading LSB and MSB related to different samples.*/
      
      if(LPS25HB_Set_Bdu(pxLPS25HBInit->BDU))
              return LPS25HB_ERROR;
      
      /*Step 5. SIM bit selects the SPI serial interface mode.*/
      /* This feature has effect only if SPI interface is used*/
      
      if(LPS25HB_Set_SpiInterface(pxLPS25HBInit->Sim))
              return LPS25HB_ERROR;

       return LPS25HB_OK;
}


/**
* @brief   Get Generic Configuration
* @param   Struct to empty with the  tmp read from sensor
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_GenericConfig(LPS25HB_ConfigTypeDef_st* pxLPS25HBInit)
{  
  uint8_t tmp;
	
  if(LPS25HB_ReadReg(LPS25HB_RES_CONF_REG, 1, &tmp))
    return LPS25HB_ERROR;
  
    pxLPS25HBInit->PressResolution=(LPS25HB_Avgp_et)(tmp&LPS25HB_AVGP_MASK);
    pxLPS25HBInit->TempResolution=(LPS25HB_Avgt_et)(tmp& LPS25HB_AVGT_MASK);
	
    if(LPS25HB_ReadReg(LPS25HB_CTRL_REG1, 1, &tmp))
      return LPS25HB_ERROR;

 
  pxLPS25HBInit->OutputDataRate= (LPS25HB_Odr_et)(tmp & LPS25HB_ODR_MASK);
  
  pxLPS25HBInit->BDU=(LPS25HB_Bdu_et)(tmp & LPS25HB_BDU_MASK);
  pxLPS25HBInit->Sim=(LPS25HB_SPIMode_et)(tmp& LPS25HB_SIM_MASK);
	
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
    return LPS25HB_ERROR;
	 
  pxLPS25HBInit->AutoZero=(LPS25HB_State_et)((tmp&LPS25HB_RESET_AZ_MASK)>>LPS25HB_AUTO_ZERO_BIT);

	
  return LPS25HB_OK;
	
}

/**
* @brief   Set Interrupt Configuration
* @param    Struct to empty with the chosen tmp
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Set_InterruptConfig(LPS25HB_InterruptTypeDef_st* pLPS25HBInt)
{  
    if(LPS25HB_Set_InterruptActiveLevel(pLPS25HBInt->INT_H_L))
            return LPS25HB_ERROR;
    
    if(LPS25HB_Set_InterruptOutputType(pLPS25HBInt->PP_OD))
            return LPS25HB_ERROR;

    if(LPS25HB_Set_InterruptControlConfig(pLPS25HBInt->OutputSignal_INT1))
                    return LPS25HB_ERROR;
    
    if(pLPS25HBInt->OutputSignal_INT1==LPS25HB_DATA){
            
            if(LPS25HB_Set_InterruptDataConfig(pLPS25HBInt->DataInterrupt_INT1))
                    return LPS25HB_ERROR;
    }
                    
    if(LPS25HB_LatchInterruptRequest(pLPS25HBInt->LatchIRQ))
                    return LPS25HB_ERROR;
    
    if(LPS25HB_Set_PressureThreshold(pLPS25HBInt->fP_threshold))
                    return LPS25HB_ERROR;
    
    /*DIFF_EN bit is used to enable the circuitry for the computing of differential pressure output.*/
    /*It is suggested to turn on the circuitry only after the configuration of REF_P_x and THS_P_x.*/
    
    if(LPS25HB_Set_InterruptDifferentialConfig(pLPS25HBInt->PressureInterrupt))
                    return LPS25HB_ERROR;
    
    return LPS25HB_OK;
}

/**
* @brief   Get Interrupt Configuration
* @param   Struct to empty with the tmp read from sensor
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_InterruptConfig(LPS25HB_InterruptTypeDef_st* pLPS25HBInt)
{  
  uint8_t tmp;
	  
  if(LPS25HB_ReadReg(LPS25HB_CTRL_REG3, 1, &tmp))
    return LPS25HB_ERROR;
  
  pLPS25HBInt->INT_H_L=(LPS25HB_State_et)((tmp&LPS25HB_INT_H_L_MASK)>>LPS25HB_INT_H_L_BIT);

  pLPS25HBInt->PP_OD =(LPS25HB_OutputType_et)(tmp&LPS25HB_PP_OD_MASK);
  pLPS25HBInt->OutputSignal_INT1=(LPS25HB_OutputSignalConfig_et)((tmp&0x03));
		
  if(pLPS25HBInt->OutputSignal_INT1==LPS25HB_DATA){
    if(LPS25HB_ReadReg(LPS25HB_CTRL_REG4, 1, &tmp))
			return LPS25HB_ERROR;
	
    pLPS25HBInt->DataInterrupt_INT1=(LPS25HB_DataSignalType_et)(tmp &=0x0F);
  }
  if(LPS25HB_ReadReg(LPS25HB_INTERRUPT_CFG_REG, 1, &tmp))
    return LPS25HB_ERROR;
	
  pLPS25HBInt->LatchIRQ=(LPS25HB_State_et)((tmp &LPS25HB_LIR_MASK)>>LPS25HB_LIR_BIT);
  pLPS25HBInt->PressureInterrupt=(LPS25HB_InterruptDiffConfig_et)(tmp &LPS25HB_PE_MASK);
  
  if(LPS25HB_Get_PressureThreshold(&pLPS25HBInt->fP_threshold))
    return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}
/**
  * @brief   Set Fifo Configuration
* @param 	 Struct to empty with the chosen tmp 
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Set_FifoConfig(LPS25HB_FIFOTypeDef_st* pLPS25HBFIFO)
{
  if(pLPS25HBFIFO->FIFO_MODE == LPS25HB_FIFO_BYPASS_MODE) { 			
		/* FIFO Disable-> FIFO_EN bit=0 in CTRL_REG2*/
		if(LPS25HB_Set_FifoModeUse(LPS25HB_DISABLE))
			return LPS25HB_ERROR;
  } 
  else {
		/* FIFO Enable-> FIFO_EN bit=1 in CTRL_REG2*/
		if(LPS25HB_Set_FifoModeUse(LPS25HB_ENABLE))
			return LPS25HB_ERROR;

			if(pLPS25HBFIFO->FIFO_MODE==LPS25HB_FIFO_MEAN_MODE){
                                    if(LPS25HB_Set_FifoSampleSize(pLPS25HBFIFO->MEAN_MODE_SAMPLE))
                                            return LPS25HB_ERROR;
                                    if(pLPS25HBFIFO->FIFO_MEAN_DEC)
                                            if(LPS25HB_Set_FifoMeanDecUse(LPS25HB_ENABLE))
                                              return LPS25HB_ERROR;
			}
			else{
                                  if (pLPS25HBFIFO->WTM_INT){
                                          if(LPS25HB_Set_FifoWatermarkLevelUse(LPS25HB_ENABLE))
                                                  return LPS25HB_ERROR;
                                          if(LPS25HB_Set_FifoWatermarkLevel(pLPS25HBFIFO->WTM_LEVEL))
                                                  return LPS25HB_ERROR;	
                                  }
			}
	}
  
  if(LPS25HB_Set_FifoMode(pLPS25HBFIFO->FIFO_MODE))
    return LPS25HB_ERROR;
 
  return LPS25HB_OK;

}

/**
  * @brief   Get Fifo Configuration
* @param 	 Struct to empty with the  tmp read from sensor
  * @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
  */
LPS25HB_Error_et LPS25HB_Get_FifoConfig(LPS25HB_FIFOTypeDef_st* pLPS25HBFIFO)
{  
  uint8_t tmp;
	
  if(LPS25HB_ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))
    return LPS25HB_ERROR;

  pLPS25HBFIFO->FIFO_MODE=(LPS25HB_FifoMode_et)(tmp&LPS25HB_FMODE_MASK);
 
  if(pLPS25HBFIFO->FIFO_MODE==LPS25HB_FIFO_MEAN_MODE){

    pLPS25HBFIFO->MEAN_MODE_SAMPLE=(LPS25HB_FifoMeanModeSample_et)(tmp&LPS25HB_WTM_POINT_MASK);
    if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
      return LPS25HB_ERROR;
    pLPS25HBFIFO->FIFO_MEAN_DEC=(LPS25HB_State_et)((tmp&LPS25HB_FIFO_MEAN_MASK)>>LPS25HB_FIFO_MEAN_BIT);
  
  }
  else{
	if(pLPS25HBFIFO->FIFO_MODE != LPS25HB_FIFO_BYPASS_MODE) { 
          if(LPS25HB_ReadReg(LPS25HB_CTRL_REG2, 1, &tmp))
            return LPS25HB_ERROR;
          pLPS25HBFIFO->WTM_INT=(LPS25HB_State_et)((tmp&LPS25HB_WTM_EN_MASK)>>LPS25HB_WTM_EN_BIT);
          if (pLPS25HBFIFO->WTM_INT){
            if(LPS25HB_ReadReg(LPS25HB_CTRL_FIFO_REG, 1, &tmp))
              return LPS25HB_ERROR;
            pLPS25HBFIFO->WTM_LEVEL=(uint8_t)(tmp&LPS25HB_WTM_POINT_MASK);
          }
        }
  }
 
  return LPS25HB_OK;
}


/**
* @brief   Get the Reference Pressure tmp that is sum to the sensor output pressure
* @param   Buffer to empty with reference pressure tmp
* @retval  Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_ReferencePressure(int32_t* RefP)
{  
  uint8_t buffer[3];
  uint32_t tempVal=0;
  int32_t raw_press;  

  if(LPS25HB_ReadReg(LPS25HB_REF_P_XL_REG, 3, buffer))
      return LPS25HB_ERROR;
  
  /* Build the raw data */
  for(uint8_t i=0; i<3; i++)
      tempVal |= (((uint32_t)buffer[i]) << (8*i));
  
  /* convert the 2's complement 24 bit to 2's complement 32 bit */
  if(tempVal & 0x00800000)
    tempVal |= 0xFF000000;
  
  raw_press =((int32_t)tempVal);
  *RefP = (raw_press*100)/4096;
	
  return LPS25HB_OK;
}


/**
* @brief  Check if the single measurement has completed.
* @param  tmp is set to 1, when the measure is completed
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/  
LPS25HB_Error_et LPS25HB_IsMeasurementCompleted(uint8_t* Is_Measurement_Completed)
{  
  uint8_t tmp;
  LPS25HB_DataStatus_st datastatus;
	  
  if(LPS25HB_ReadReg(LPS25HB_STATUS_REG, 1, &tmp))
    return LPS25HB_ERROR;
	
  datastatus.TempDataAvailable=(uint8_t)(tmp&0x01);
  datastatus.PressDataAvailable= (uint8_t)((tmp&0x02)>>LPS25HB_PDA_BIT);  
  
  *Is_Measurement_Completed=(uint8_t)((datastatus.PressDataAvailable) & (datastatus.TempDataAvailable));
  
  return LPS25HB_OK;
 }
 
/**
* @brief  Get the values of the last single measurement.
* @param  Pressure and temperature tmp
* @retval Status [LPS25HB_ERROR, LPS25HB_OK]
*/
LPS25HB_Error_et LPS25HB_Get_Measurement(LPS25HB_MeasureTypeDef_st *Measurement_Value)
{  
  int16_t Tout;
  int32_t Pout;
  
  if(LPS25HB_Get_Temperature(&Tout))
    return LPS25HB_ERROR;
  
  Measurement_Value->Tout=Tout;
  
  if(LPS25HB_Get_Pressure(&Pout))
    return LPS25HB_ERROR;
  
  Measurement_Value->Pout=Pout;
  
  return LPS25HB_OK;
 
 }
 
 
/**
* @brief  De initialization function for LPS25HB.
*         This function put the LPS25HB in power down, make a memory boot and clear the data output flags.
* @param  None.
* @retval Error code [LPS25HB_OK, LPS25HB_ERROR].
*/
LPS25HB_Error_et LPS25HB_DeInit(void)
{    
  LPS25HB_MeasureTypeDef_st Measurement_Value;    

  /* LPS25HB in power down */
  if(LPS25HB_Set_PowerDownMode(LPS25HB_RESET))
     return LPS25HB_ERROR;
  
  /* Make LPS25HB Reset and Reboot */ 
  if(LPS25HB_SwResetAndMemoryBoot())
     return LPS25HB_ERROR;
 
  /* Dump of data output */
  if(LPS25HB_Get_Measurement(& Measurement_Value))
     return LPS25HB_ERROR;
  
  return LPS25HB_OK;
}
  

 
#ifdef  USE_FULL_ASSERT_LPS25HB
/**
* @brief  Reports the name of the source file and the source line number
*         where the assert_param error has occurred.
* @param file: pointer to the source file name
* @param line: assert_param error line source number
* @retval : None
*/
void LPS25HB_assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number */
  printf("Wrong parameters tmp: file %s on line %d\r\n", file, line);
  
  /* Infinite loop */
  while (1)
  {
  }
}
#endif
 
 /**
* @}
*/

/**
  * @}
  */

/**
  * @}
*/

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/

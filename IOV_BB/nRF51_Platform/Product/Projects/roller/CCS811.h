#ifndef _CCS811_H_
#define _CCS811_H_

#include <stdint.h>
////////////////////////////////////////////////////

////////////////////////////////////////////////////
#define CCS811_Device_Adderss      0x5a

/******
* App Register Map
*/
#define CCS811_STATUS              0x00 
#define CCS811_MEAS_MODE           0x01
#define CCS811_ALG_RESULT_DATA     0x02
#define CCS811_RAW_DATA            0x03
#define CCS811_ENV_DATA            0x05
#define CCS811_THRESHOLDS          0x10
#define CCS811_BASELINE            0x11
#define CCS811_HW_ID               0x20
#define CCS811_HW_Version          0x21
#define CCS811_FW_Boot_Version     0x23
#define CCS811_FW_App_Version      0x24
#define CCS811_ERROR_ID            0xE0
#define CCS811_SW_RESET            0xFF

/*******
* Bootloader Register Map
*/
#define APP_ERASE                  0xF1 
#define APP_DATA                   0xF2
#define APP_VERIFY                 0xF3
#define APP_START                  0xF4

/*******
* Pin define
*/
#define CCS811_NWAKE_PIN 25
#define CCS811_NRST_PIN  24
#define CCS811_NINT_PIN  23

void CCS811_Wake_Up(void);
uint8_t CCS811_Read_Byte(uint8_t addr);
void CCS811_Write_Byre(uint8_t addr, uint8_t data);

#endif


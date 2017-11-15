#ifndef _MC24AA02_H_
#define _MC24AA02_H_

#include <stdint.h>
////////////////////////////////////////////////////

////////////////////////////////////////////////////
#define MC24AA02_Device_Adderss      0x50

#define MC24AAXXXE48_EUI48_Addr 	 0xFA

#define MC24AAXXXE64_EUI64_Addr 	 0xF8

typedef struct {
  uint8_t addr;
  uint8_t data[8];
}MC24AA02_Write_Page_t;

uint8_t MC24AA02_Random_Read(uint8_t addr);
void MC24AA02_Write_Byre(uint8_t addr, uint8_t data);
void MC24AA02_Read_Sequential(uint8_t addr, uint8_t *data, uint8_t len);
void MC24AA02_Read_CurSequential(uint8_t *data, uint8_t len);
void MC24AA02_Write_Page(MC24AA02_Write_Page_t data);

#endif


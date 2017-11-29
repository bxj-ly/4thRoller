/*
  Copyright 2014-2015 juma.io

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at
       http://www.apache.org/licenses/LICENSE-2.0
   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "juma_sdk_api.h"
#include "HDC1080.h"
#include "CCS811.h"
#include "MC24AA02.h"


#define BEEPER_CTL 9
#define DEBUG_LED 10
#define SERIAL_RX 28
#define SERIAL_TX 29
#define GPRS_PWRKEY 13

uint8_t is_led_on = 0;

static void bb_send_data(char *buffer)
{
  serial_send((uint8_t *)buffer, strlen(buffer)); 
  if(0 != ble_device_is_connected()){
    ble_device_send(1, strlen(buffer),(uint8_t *)buffer);
  } 
}
void led_show(void)
{
    gpio_setup(DEBUG_LED, GPIO_OUTPUT);
    if(is_led_on){
        is_led_on = 0;
        gpio_write(DEBUG_LED, is_led_on);
    }
    else {
        is_led_on = 1;
        gpio_write(DEBUG_LED, is_led_on);
    }
}

#define USE_HDC1080_MODE_1
void on_ready()
{
    char chrStr[4];
#if 0    
    uint16_t regval = 0;
#endif
    led_show();
  
    CCS811_Wake_Up();
    serial_setup(SERIAL_RX, SERIAL_TX, UART_BAUDRATE_Baud115200);    
    
#if 0
    
    regval = HDC1080_Read_DeviceID();
    sprintf(chrStr, "HDC1080_Read_DeviceID = 0x%x\r\n ", regval);
    serial_send((uint8_t *)chrStr, strlen(chrStr));
    regval = HDC1080_Read_ManufacturerID();
    sprintf(chrStr, "HDC1080_Read_ManufacturerID = 0x%x\r\n ", regval); 
    serial_send((uint8_t *)chrStr, strlen(chrStr));    
#endif
    HDC1080_Soft_Reset(); 

#ifdef USE_HDC1080_MODE_1
    HDC1080_Set_MODE(1);
#endif /*USE_HDC1080_MODE_1*/
#if 0
    regval = HDC1080_Read_MODE();
    sprintf(chrStr, "HDC1080_Read_MODE = 0x%x\r\n\r\n\r\n ", regval);   
    serial_send((uint8_t *)chrStr, strlen(chrStr));

    regval = CCS811_Read_Byte(CCS811_HW_ID);
    sprintf(chrStr, "CCS811_HW_ID = 0x%x\r\n ", regval);  
    serial_send((uint8_t *)chrStr, strlen(chrStr));
    
    uint8_t eui64[8];
    uint8_t i = 0;
    int len = 0;
    len += sprintf(chrStr + len, "MC24AAXXXE64_EUI64 = ");
    for(i=0; i < 8; i ++){
        eui64[i] = 0;
    }    
    MC24AA02_Read_Sequential(MC24AAXXXE64_EUI64_Addr, eui64, 8);
    for(i=0; i < 8; i ++){
 //       eui64[i] = MC24AA02_Random_Read(MC24AAXXXE64_EUI64_Addr+i);
        len += sprintf(chrStr + len, "0x%2x ", eui64[i]);
    }
    len += sprintf(chrStr + len, "\r\n");
    serial_send((uint8_t *)chrStr, strlen(chrStr));
    
    MC24AA02_Write_Page_t mc2d;
    mc2d.addr = 0x00;
    for(i=0; i < 8; i ++){
        mc2d.data[i] = 0x41 + i;
    }    
    MC24AA02_Write_Page(mc2d);
#endif  
    timer_init(9, TIMER_PERIODIC);
    timer_start(32000);//2000ms is the max value
    ble_device_set_name("ROLLER");
    ble_device_start_advertising();	 

    gpio_setup(GPRS_PWRKEY, GPIO_OUTPUT);
    gpio_write(GPRS_PWRKEY, 1);
    
    //play_sound(BEEPER_CTL);   
}
char buffer[64];
int splen = 0;
uint8_t mc24buf[9];
uint8_t tcount = 0;
uint8_t timer_cnt = 0;
void timer_on_fired(void)
{	
    led_show();

    if(timer_cnt < 10) 
    {
      timer_cnt++;
      return;
    } 
    else
    {
      timer_cnt = 0;
    }
    // there is a dirty byte on first send.
    sprintf(buffer, "ignore me");    
    serial_send((uint8_t *)buffer, strlen(buffer));    
#ifdef USE_HDC1080_MODE_1
    uint16_t temp,humi;
    HDC1080_Read_Temperature_and_Humidity(&temp, &humi);
    sprintf(buffer, "HDC1080.T: %d\r\n", temp);
    bb_send_data(buffer);
    sprintf(buffer, "HDC1080.H: %d\r\n", humi);
    bb_send_data(buffer);
#else /*USE_HDC1080_MODE_1*/
    uint16_t data = 0;
    data = HDC1080_Read_Temperature();
    sprintf(buffer, "HDC1080.T: %d", data);  
    bb_send_data(buffer);     
    
    data = HDC1080_Read_Humidity();
    sprintf(buffer, "HDC1080.H: %d", data);  
    bb_send_data(buffer);     
#endif     /*USE_HDC1080_MODE_1*/
    uint8_t ccs = 0;
    
    ccs = CCS811_Read_Byte(CCS811_STATUS);
    sprintf(buffer, "CCS.S: 0x%x\r\n", ccs);  
    bb_send_data(buffer);    
  
    for(tcount=0;tcount<9;tcount++){
        mc24buf[tcount] = 0;
    }
    splen = 0;
    splen += sprintf(buffer + splen, "EEPROM.R:");
    MC24AA02_Read_Sequential(0x00, mc24buf , 8);

    splen += sprintf(buffer + splen, "%s\r\n",mc24buf);
    bb_send_data(buffer);
}


typedef enum __SERIAL_FSM__{
SERIAL_FSM_WAIT_HEADER_0 = 0,
SERIAL_FSM_WAIT_HEADER_1,
SERIAL_FSM_WAIT_DATA_LEN,
SERIAL_FSM_WAIT_DATA
}serial_fsm_e;

static serial_fsm_e serial_fsm = SERIAL_FSM_WAIT_HEADER_0;
static char serial_buffer[128];
static uint8_t serial_buffer_index = 0;
static uint8_t serial_data_len = 0;

void receive_serial_data(uint8_t data)
{
  if(serial_data_len > 0)
  {
    serial_buffer[serial_buffer_index++] = data;
    if(0 == --serial_data_len)
    {
      ble_device_send(1, serial_buffer_index,(uint8_t *)serial_buffer);
      serial_buffer_index = 0;
      serial_fsm = SERIAL_FSM_WAIT_HEADER_0;
    }

    if('\n' == data)
    {
      serial_buffer_index = 0;
      serial_fsm = SERIAL_FSM_WAIT_HEADER_0;
    }
  }
  
}

void serial_on_data(uint8_t data)
{
  switch(serial_fsm)
  {
  case SERIAL_FSM_WAIT_HEADER_0:
    if(0xFF == data) serial_fsm = SERIAL_FSM_WAIT_HEADER_1;
    break;
    
  case SERIAL_FSM_WAIT_HEADER_1:
    if(0xAA == data) serial_fsm = SERIAL_FSM_WAIT_DATA_LEN;
    else serial_fsm = SERIAL_FSM_WAIT_HEADER_0;
    break;    

  case SERIAL_FSM_WAIT_DATA_LEN:
    serial_buffer_index = 0;
    serial_data_len = data;
    serial_fsm = SERIAL_FSM_WAIT_DATA;
    break;  

  case SERIAL_FSM_WAIT_DATA:
    receive_serial_data(data);
    break; 

  default:
    serial_fsm = SERIAL_FSM_WAIT_HEADER_0;
  }

}

void ble_device_on_connect(void)
{

}

void ble_device_on_disconnect(uint8_t reason)
{
    ble_device_start_advertising();
}

void ble_device_on_message(uint8_t type, uint16_t length, uint8_t* value)
{
    ble_device_send(type, length, value);
}




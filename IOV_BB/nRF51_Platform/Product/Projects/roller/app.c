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


#define BEEPER_CTL 9
#define DEBUG_LED 10
#define SERIAL_RX 29
#define SERIAL_TX 28

uint8_t is_led_on = 0;
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
    char chrStr[64];
    uint16_t regval = 0;
  
    CCS811_Wake_Up();
    led_show();
//    SEGGER_RTT_WriteString(0, "on_ready\n");	
    serial_setup(SERIAL_RX, SERIAL_TX, UART_BAUDRATE_Baud115200);    
    sprintf(chrStr, "on_ready\n ");    
    serial_send((uint8_t *)chrStr, strlen(chrStr)); 
    

    
    regval = HDC1080_Read_DeviceID();
    sprintf(chrStr, "HDC1080_Read_DeviceID = 0x%x\n ", regval);
    serial_send((uint8_t *)chrStr, strlen(chrStr));
    regval = HDC1080_Read_ManufacturerID();
    sprintf(chrStr, "HDC1080_Read_ManufacturerID = 0x%x\n ", regval); 
    serial_send((uint8_t *)chrStr, strlen(chrStr));    

    HDC1080_Soft_Reset(); 

#ifdef USE_HDC1080_MODE_1
    HDC1080_Set_MODE(1);
#endif /*USE_HDC1080_MODE_1*/

    regval = HDC1080_Read_MODE();
    sprintf(chrStr, "HDC1080_Read_MODE = 0x%x\n\n\n ", regval);   
    serial_send((uint8_t *)chrStr, strlen(chrStr));

    regval = CCS811_Read_Byte(CCS811_HW_ID);
    sprintf(chrStr, "CCS811_HW_ID = 0x%x\n ", regval);  
    serial_send((uint8_t *)chrStr, strlen(chrStr));
  
    
    timer_init(9, TIMER_PERIODIC);
    timer_start(32000);//2000ms is the max value
    ble_device_set_name("ROLLER_ECHO_DEMO");
    ble_device_start_advertising();	 
    
    play_sound(BEEPER_CTL);
}
char buffer[64];

void timer_on_fired(void)
{	
    
#ifdef USE_HDC1080_MODE_1
    uint16_t temp,humi;
    HDC1080_Read_Temperature_and_Humidity(&temp, &humi);
    sprintf(buffer, "HDC1080_Read_Temperature = %d , HDC1080_Read_Humidity = %d%%\n ", temp, humi);  
    serial_send((uint8_t *)buffer, strlen(buffer)); 
#else /*USE_HDC1080_MODE_1*/
    uint16_t data = 0;
    data = HDC1080_Read_Temperature();
    sprintf(buffer, "HDC1080_Read_Temperature = %d\n ", data);  
    serial_send((uint8_t *)buffer, strlen(buffer));     

    
    data = HDC1080_Read_Humidity();
    sprintf(buffer, "HDC1080_Read_Humidity = %d%%\n ", data);  
    serial_send((uint8_t *)buffer, strlen(buffer));      
#endif     /*USE_HDC1080_MODE_1*/
    uint8_t ccs = 0;
    
    ccs = CCS811_Read_Byte(CCS811_STATUS);
    sprintf(buffer, "CCS811_STATUS = 0x%x\n ", ccs);  
    serial_send((uint8_t *)buffer, strlen(buffer));    



//  SEGGER_RTT_WriteString(0, "timer_on_fired\n");	

  if(0 != ble_device_is_connected()){
    ble_device_send(1, strlen("hello\r\n"),(uint8_t *)"hello\r\n");
  }

  led_show();
}
void serial_on_data(uint8_t data)
{
    led_show();
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




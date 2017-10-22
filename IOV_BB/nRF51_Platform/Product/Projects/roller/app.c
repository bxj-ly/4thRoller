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

void on_ready()
{
    led_show();
    SEGGER_RTT_WriteString(0, "on_ready\n");	
    serial_setup(SERIAL_RX, SERIAL_TX, UART_BAUDRATE_Baud115200);
    timer_init(9, TIMER_PERIODIC);
    timer_start(2000);//2000ms is the max value
    ble_device_set_name("ROLLER_ECHO_DEMO");
    ble_device_start_advertising();	 
    
    play_sound(BEEPER_CTL);
}
char buffer[64];

void timer_on_fired(void)
{	
  uint16_t ManufacturerID = HDC1080_Read_ManufacturerID();

  sprintf(buffer, "ManufacturerID=%x",ManufacturerID);
  SEGGER_RTT_WriteString(0, "timer_on_fired\n");	

  if(0 != ble_device_is_connected()){
  ble_device_send(1, strlen("hello\r\n"),"hello\r\n");
  }

  serial_send( buffer, strlen(buffer) );  
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




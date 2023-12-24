#ifndef CONSTS
#define CONSTS
#include <Arduino.h>

/*
 *==========Arduino Nano pinout====== 
 *                      _______
 *                 TXD-|       |-Vin 
 *                 RXD-|       |-Gnd to driver board  
 *                 RST-|       |-RST
 *                 GND-|       |-+5V To driver board  
 *                  D2-|       |-A7
 *         To EN^,  D3-|       |-A6
 *       To Strobe, D4-|       |-A5 ,SCL (to Display) 
 *          To Clk, D5-|       |-A4 ,SDA (to Display) 
 *         To Data, D6-|       |-A3
 *                  D7-|       |-A2
 *                  D8-|       |-A1
 *                  D9-|       |-A0 
 *                 D10-|       |-Ref
 *                 D11-|       |-3.3V   
 *                 D12-|       |-D13
 *                      --USB--        
 */

#define SR_st_pin 4 // 74HC565 shift register strob pin (12) - active high (low/hig/low pulse shift Sr vector to outputs)make sure set low during "push"data in
#define SR_clk_pin 5// 74HC565 shift clock pin (11) - active high (low/high/low pulse shift data in) make sure initaly set to low 
#define SR_data_pin 6// 74HC565 Data in pin (14)  
#define SR_en_pin 3// 74HC565 out put enable pin (13) - active low 

#define INTERNAL_LED 13
#define BAUDRATE 115200
 
const int num_of_boards = 1; 
const int valves_per_board = 16;
const int num_of_valves = valves_per_board * num_of_boards;
const uint32_t max_pulse_time = 1000; // ms
const uint32_t min_pulse_time = 0;
const uint32_t max_space_time = 2000;
const uint32_t min_space_time = 10;

uint32_t pulse_time = 200;
uint32_t space_time = 1000;

bool use_serial = true ; //  print to serial (set monitor to same boudrate)
bool use_display = false; // display on SH106 (make sure Display I2C address and DATA nad Clock conection)  

#endif

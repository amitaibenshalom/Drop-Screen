
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
#define SR_clk_pin 5 // 74HC565 shift clock pin (11) - active high (low/high/low pulse shift data in) make sure initaly set to low 
#define SR_data_pin 6 // 74HC565 Data in pin (14)  
#define SR_en_pin 3 // 74HC565 out put enable pin (13) - active low 
#define encoder_pinA 11
#define encoder_pinB 12
#define encoder_sw 10

#define INTERNAL_LED 13
#define BAUDRATE 115200
 
const uint32_t max_pulse_time = 1000; // ms
const uint32_t min_pulse_time = 0;
const uint32_t max_space_time = 2000;
const uint32_t min_space_time = 0;
const byte max_cassettes = 32;
const byte boards_per_cassette = 4;
const byte valves_per_board = 16;
const uint32_t pulse_encoder_inc = 10; // how much does the encoder increases the pulse time
const uint32_t space_encoder_inc = 100; // how much does the encoder increases the pulse time

const int num_of_valves = max_cassettes*boards_per_cassette*valves_per_board; 

uint32_t pulse_time = 100;
uint32_t space_time = 500;
byte cassette_num = 0;
byte board_num = 0;
byte valve_num = 0;
byte current_setting = 0; // 0-cassette , 1-board , 2-valve
const byte max_settings = 3;
const uint32_t confirm_time = 3000;
const uint32_t valve_on_time = 4000;

bool on_button = false;
bool started = false;
uint32_t last_click = 0;
byte old_encoder_read = 0;
byte new_encoder_read = 0;
byte encoder_pos = 0;
const byte prescaler = 2;

float delay_tick = 1;

bool use_serial = true ; //  print to serial (set monitor to same boudrate)
bool use_display = false; // display on SH106 (make sure Display I2C address and DATA nad Clock conection)  

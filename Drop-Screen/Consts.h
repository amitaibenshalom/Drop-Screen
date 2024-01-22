#ifndef CONSTS_H
#define CONSTS_H
#include <Arduino.h>
#include <avr/pgmspace.h>

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
#define led_pin 2

#define image_w 64
#define image_h 20

#define INTERNAL_LED 13
#define BAUDRATE 115200

const byte max_cassettes = 32;
const byte boards_per_cassette = 4;
const byte valves_per_board = 16;
const uint16_t num_of_valves = max_cassettes*boards_per_cassette*valves_per_board; 
const uint16_t valves_per_cassette = boards_per_cassette*valves_per_board;

const uint16_t max_space_time = 2000; // time between frames of the drawing
const uint16_t min_space_time = 0;
const byte max_drawing_depth = max_cassettes; // number of rows producing the same frame together
const byte min_drawing_depth = 1;
const byte max_valve_on_time = 40; // how much time will the valve be on during one row of the frame
const byte min_valve_on_time = 5;
const uint16_t max_led_start = 1000; // how much time will the led be offset by the frame
const uint16_t min_led_start = 0;
const uint16_t max_led_on_time = 1500; // how much time will the led be on during the frame
const uint16_t min_led_on_time = 20;

const uint16_t space_time_step = 50; // how much does the encoder increases the space time variable
const byte drawing_depth_step = 1;
const byte valve_on_time_step = 1;
const uint16_t led_start_step = 10;
const uint16_t led_on_time_step = 10;

byte valve_on_time = 15;
uint16_t space_time = 1500;
uint16_t led_start = 690;
uint16_t led_on_time = 280;
byte drawing_depth = 1;

//byte flags = 0; // 8 bits, for all the flags:  
//                //    0: button_flag - true if user presses button
//                //    1: drawing_flag - true if the system is currently drawing a frame
//                //    2: valve_on_flag - true if valves are on (for valve_on_time millis)
//                //    3: space_flag - true if needed to wait in space now (for space_time millis)
//                //    4: led_start_flag - true if needed to wait in to led start (for led_start millis)
//                //    5: led_on_flag - true if needed to wait for led to be turned off (for led_on_time millis)
                
bool on_button = false;
bool drawing_flag = false;
bool valve_on_flag = false;
bool space_flag = false;
bool led_start_flag = false;
bool led_on_flag = false;

//uint32_t last_click = 0;
uint32_t last_valve_on = 0;
uint32_t last_space_time = 0;
uint32_t last_led_start = 0;
uint32_t last_led_on = 0;

byte current_setting = 0;
const byte max_settings = 5;
byte old_encoder_read = 0;
byte new_encoder_read = 0;
signed int encoder_pos = 0;
const byte prescaler = 2;

float delay_tick = 1;


// 'heart_4', 64x20px
const byte heart_bytes [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xf8, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xf8, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 
  0xff, 0xff, 0x00, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 
  0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x00, 0x07, 0xc0, 0x03, 0xff, 0xff, 
  0xff, 0xff, 0xf0, 0xff, 0xf0, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const byte drawings_num = 1; // number of drawings
char* drawings[] = {
  heart_bytes
};
byte drawing_index = 0; // which drawing currently is produced
byte row_in_drawing = 0; //which row in the drawing is currently produced
byte cassette_drawing = 0; //in which cassette is the current frame (drawing) being produced


#endif

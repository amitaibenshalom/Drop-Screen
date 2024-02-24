#ifndef CONSTS_H
#define CONSTS_H
#include <Arduino.h>
#include <avr/pgmspace.h>

#define SR_st_pin 4 // 74HC565 shift register strob pin (12) - active high (low/hig/low pulse shift Sr vector to outputs)make sure set low during "push"data in
#define SR_clk_pin 13 // 74HC565 shift clock pin (11) - active high (low/high/low pulse shift data in) make sure initaly set to low 
#define SR_data_pin 11 // 74HC565 Data in pin (14)  
#define SR_en_pin 7 // 74HC565 out put enable pin (13) - active low 
#define encoder_pinA 2
#define encoder_pinB 3
#define encoder_sw 8
#define red_led_pin 5
#define green_led_pin 6
#define blue_led_pin 9

#define image_w 64
#define image_h 20
#define cassettes_num 10
#define START_KEY 's'

#define INTERNAL_LED 13
#define BAUDRATE 115200

float view_height = 1.5; // in meters
const float g = 9.81; // m/s^2
const byte max_cassettes = 32;
const byte boards_per_cassette = 4;
const byte valves_per_board = 16;
const uint16_t num_of_valves = max_cassettes*boards_per_cassette*valves_per_board; 
const uint16_t valves_per_cassette = boards_per_cassette*valves_per_board;

const uint16_t max_space_time = 2000; // time between frames of the drawing
const uint16_t min_space_time = 0;
const byte max_drawing_depth = max_cassettes; // number of rows producing the same frame together
const byte min_drawing_depth = 1;
const byte max_valve_on_time = 100; // how much time will the valve be on during one row of the frame
const byte min_valve_on_time = 1;
const uint16_t max_led_start = 1000; // how much time will the led be offset by the frame
const uint16_t min_led_start = 0;
const uint16_t max_led_on_time = 1500; // how much time will the led be on during the frame
const uint16_t min_led_on_time = 0;
const byte min_led_power = 0;
const byte max_led_power = 255;

const uint16_t space_time_step = 50; // how much does the encoder increases the space time variable
const byte drawing_depth_step = 1;
const byte valve_on_time_step = 1;
const uint16_t led_start_step = 10;
const uint16_t led_on_time_step = 10;
const byte led_power_step = 5;

byte valve_on_time = 5;
uint16_t space_time = 750;
uint16_t led_start = 350;
uint16_t led_on_time = 550;
byte drawing_depth = 2;
byte color = 0;
byte colors[][3] = {{255,0,255},{255,255,0},{255,215,215}};
byte led_power = 255; // for PWM
byte colors_num = 3;
         
bool on_button = false;
bool drawing_flag = false;
bool valve_on_flag = false;
bool space_flag = false;
bool led_start_flag = false;
bool led_on_flag = false;
bool full_light = true;
bool auto_factor_flag = false; // true if you want the program to automatically factor the timming of the valves per row to match the drawing
bool random_cassette = true;

//uint32_t last_click = 0;
uint32_t last_valve_on = 0;
uint32_t last_space_time = 0;
uint32_t last_led_start = 0;
uint32_t last_led_on = 0;

uint8_t current_setting = 0;
const uint8_t max_settings = 8;
byte old_encoder_read = 0;
byte new_encoder_read = 0;
signed int encoder_pos = 0;
const byte prescaler = 2;

int auto_valve_on_time = valve_on_time;
signed char row_in_drawing = image_h - 1; //which row in the drawing is currently produced 2D
byte cassette_drawing = 0; //in which cassette is the current frame (drawing) being produced 2D
byte image[160];

#endif

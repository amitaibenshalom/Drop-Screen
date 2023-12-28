#include "Routines.h"

void setup() {
  pinMode(SR_en_pin, OUTPUT);
  digitalWrite(SR_en_pin, HIGH);
  pinMode(SR_st_pin, OUTPUT);
  pinMode(SR_clk_pin, OUTPUT);
  pinMode(SR_data_pin, OUTPUT);
  digitalWrite(SR_st_pin, LOW);
  digitalWrite(SR_clk_pin, LOW);
  digitalWrite(SR_data_pin, LOW);
  off_all_valves(num_of_valves);
  
  pinMode(encoder_sw, INPUT_PULLUP);
  pinMode(encoder_pinA, INPUT); 
  pinMode(encoder_pinB, INPUT); 
  Serial.begin(BAUDRATE);
  delay (100);// wait to make sure serial begin
  start_display();
  Serial.println("START");
  digitalWrite(SR_en_pin, LOW); 
}

void loop() {
  do_encoder();
  display_settings();
//  off_all_valves(num_of_valves);
//  cycle_all_valves(num_of_valves);
//  reverse_cycle_all_valves(num_of_valves);
  
//  off_all_valves(num_of_valves);
//  for (int k = 0; k < 2; k++) {
//    cycle_all_valves(num_of_valves);
//    reverse_cycle_all_valves(num_of_valves);
//  }
//  for (int k = 0; k < 3; k++) {
//    on_all_valves(num_of_valves);
//    delay(300);
//    off_all_valves(num_of_valves);
//    delay(300);
//  }
//  bool arr[17] = {1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1};
//  bool* arr2 = arr;
//  for (int k = 0; k < 3; k++) {
//    array_to_valves(arr2, 16);
//    delay(space_time);
//    array_to_valves(arr2+1, 16);
//    delay(space_time);
//  }
}

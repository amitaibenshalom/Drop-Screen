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
  
  Serial.begin(BAUDRATE);
  delay (100);// wait to make sure serial begin
  Serial.println("START");
  digitalWrite(SR_en_pin, LOW); 
}

void loop() {
  off_all_valves(num_of_valves);
  cycle_all_valves(num_of_valves);
  delay(space_time);
}

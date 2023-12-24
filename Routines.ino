#include "Routines.h"

// make a pulse in a given IO
void pulse_io(int io_num){
  digitalWrite(io_num, HIGH);   
  digitalWrite(io_num, LOW);
}

// turn of all valves
void off_all_valves(int num_of_valves){
  digitalWrite(SR_data_pin, LOW);   
  for (int i = 0; i < num_of_valves; i++)
    pulse_io(SR_clk_pin);
  pulse_io(SR_st_pin);
}

// turn on all valves
void on_all_valves(int num_of_valves){
  digitalWrite(SR_data_pin, HIGH);   
  for (int i = 0; i < num_of_valves; i++) 
    pulse_io(SR_clk_pin);
  digitalWrite(SR_data_pin, LOW);
  pulse_io(SR_st_pin);
}

//----cycle single pulse on each valve - make sure reset(off)all valves befour---------------------------------------------
void cycle_all_valves(int num_of_valves){
  digitalWrite(SR_data_pin, HIGH);
  pulse_io(SR_clk_pin);
  digitalWrite(SR_data_pin, LOW);
  pulse_io(SR_st_pin);
  Serial.print ("* ");
  delay(pulse_time);
  for (int i = 1; i <= num_of_valves; i++) {
    pulse_io(SR_clk_pin);
    pulse_io(SR_st_pin);// update all valves immediately after every shift
    Serial.print ("* ");
    delay(pulse_time);
  }
  Serial.println (" done all valves ");
}

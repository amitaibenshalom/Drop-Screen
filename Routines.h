

#include "Display.h"

//void pulse_io(int);
//void off_all_valves(int);
//void on_all_valves(int);
//void cycle_all_valves(int);
//void valve_on(int);
//void reverse_cycle_all_valves(int);
//void array_to_valves(bool[], int);
//int read_encoder();
//void do_encoder();
//void delay_with_encoder(uint32_t);

void pulse_io(int io_num){
  digitalWrite(io_num, HIGH);   
  digitalWrite(io_num, LOW);
}

void off_all_valves(int num_of_valves){
  digitalWrite(SR_data_pin, LOW);   
  for (int i = 0; i < num_of_valves; i++)
    pulse_io(SR_clk_pin);
  pulse_io(SR_st_pin);
}

void on_all_valves(int num_of_valves){
  digitalWrite(SR_data_pin, HIGH);   
  for (int i = 0; i < num_of_valves; i++) 
    pulse_io(SR_clk_pin);
  digitalWrite(SR_data_pin, LOW);
  pulse_io(SR_st_pin);
}

int read_encoder() {
  old_encoder_read = new_encoder_read;
  new_encoder_read = ((digitalRead(encoder_pinA)) << 1) + (digitalRead(encoder_pinB));
  byte dir = (old_encoder_read << 2) + new_encoder_read;
  switch (dir) {
    case 1: case 7: case 8: case 14:
      return 1;
    case 2: case 4: case 11: case 13:
      return -1;
    case 0: case 5: case 10: case 15:
      return 0;
    default:
      return 0; 
  }
}

void do_encoder() {
  int value = read_encoder();
  encoder_pos += ((encoder_pos == 0 && value == -1) ? 0 : 
    ((encoder_pos == max_pulse_time/pulse_encoder_inc && value == 1) ? 0 : value));
  pulse_time += ((pulse_time == min_pulse_time && value == -1) ? 0 : 
    ((pulse_time == max_pulse_time && value == 1) ? 0 : value * pulse_encoder_inc));

  if (value != 0) {
    Serial.println(pulse_time);
  }
}

void delay_with_encoder(uint32_t delay_time) {
  do_encoder();
  for (int i = 0; i < delay_time/delay_tick; i++) {
    delay(delay_tick);
    do_encoder();
  }
//  display_settings();
}

void cycle_all_valves(int num_of_valves){
  digitalWrite(SR_data_pin, HIGH);
  pulse_io(SR_clk_pin);
  digitalWrite(SR_data_pin, LOW);
  pulse_io(SR_st_pin);
  delay(pulse_time);
  for (int i = 1; i <= num_of_valves; i++) {
    pulse_io(SR_clk_pin);
    pulse_io(SR_st_pin);// update all valves immediately after every shift
    do_encoder();
    display_settings();
    delay_with_encoder(pulse_time);
  }
}

void valve_on(int valve_index) {
  digitalWrite(SR_data_pin, HIGH);
  pulse_io(SR_clk_pin);
  digitalWrite(SR_data_pin, LOW);
  for (int i = 0; i < valve_index; i++)
    pulse_io(SR_clk_pin);
  pulse_io(SR_st_pin);
}

void reverse_cycle_all_valves(int num_of_valves){
  for (int i = num_of_valves-1; i >= 0; i--) {
    valve_on(i);
    do_encoder();
    display_settings();
    delay_with_encoder(pulse_time);
    off_all_valves(num_of_valves);
  }
}

void array_to_valves(bool arr[], int arr_length){
  for (int i = arr_length-1; i >= 0; i--) {
    digitalWrite(SR_data_pin, arr[i]); // "true"=1=HIGH, "false"=0=LOW
    pulse_io(SR_clk_pin);
  }
  pulse_io(SR_st_pin);
}

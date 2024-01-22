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
  pinMode(led_pin, OUTPUT);
  led_off();
  Serial.begin(BAUDRATE);
  delay (100);// wait to make sure serial begin
  start_display();
  Serial.println("START");
  digitalWrite(SR_en_pin, LOW); 
  drawing_index = 0;
  display_settings();
}

void loop() {

  do_encoder();

  if (space_flag && millis() - last_space_time > space_time) {
    space_flag = false;
  }
  if (valve_on_flag && millis() - last_valve_on > valve_on_time) {
    off_all_valves(num_of_valves);
    valve_on_flag = false;
  }
  if (led_on_flag && millis() - last_led_on > led_on_time) {
    if (!full_light)
      led_off();
    led_on_flag = false;
  }
  if (led_start_flag && millis() - last_led_start > led_start) {
    led_start_flag = false;
    led_on_flag = true;
    last_led_on = millis();
    if (!full_light && led_on_time > 0)
      led_on();
  }

  if (!drawing_flag && !space_flag) {
    init_drawing(drawing_index);
  }
  if (drawing_flag) {
    if (check_drawing()) {
//      Serial.println("done drawing");
      drawing_flag = false;
      off_all_valves(num_of_valves);
      space_flag = true;
      last_space_time = millis();
      cassette_drawing += drawing_depth;
      if (cassette_drawing >= 10) {
        cassette_drawing = 0;
        drawing_index++;
        if (drawing_index >= drawings_num)
          drawing_index = 0;
      }
    }
  }
}

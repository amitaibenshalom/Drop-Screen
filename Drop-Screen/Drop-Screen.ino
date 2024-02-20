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
  pulse_io(SR_st_pin);
  pinMode(encoder_sw, INPUT_PULLUP);
  pinMode(encoder_pinA, INPUT); 
  pinMode(encoder_pinB, INPUT); 
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(blue_led_pin, OUTPUT);
  led_off();
  Serial.begin(BAUDRATE);
  delay (100);// wait to make sure serial begin
//  Serial.println("START");
  start_display();
//  Serial.println("START");
  digitalWrite(SR_en_pin, LOW); 
  update_height(); // for auto factoring
//  drawing_index = 0;
  delay(1000);
}

void loop() {

//  do_encoder();
    
  if (Serial.available() > 0 && Serial.read() == START_KEY) {
    Serial.write(START_KEY);
    for (int i = 0; i < 160; i++) {
//      image[i] = Serial.read();
//      Serial.write(image[i]);
//      byte temp = Serial.read();
//      Serial.write(&temp,1);
      byte test;
      if (Serial.available() == 0) {
        i--;
        continue;
      }
      test = Serial.read();
      image[i] = test;
      Serial.println(test, DEC);
//      Serial.print(test); 
//      Serial.write(test);
      if ((i + 1) % 8 == 0) {
        Serial.write('g');
//          Serial.write(image[i]);
//        Serial.println();
//        while(Serial.available() == 0 && i < 159) {
//          delay(1);
//        }
//        for (int j = 7; j >=0; j--) {
//          Serial.write(image[i-j]);
//        }
      }
    }
    off_all_valves(num_of_valves);
    pulse_io(SR_st_pin);
    init_drawing();
  }

  if (space_flag && millis() - last_space_time > space_time) {
    space_flag = false;
  }
  if (valve_on_flag && millis() - last_valve_on > (auto_factor_flag ? auto_valve_on_time : valve_on_time)) {
    off_all_valves(num_of_valves); // without pulsing ST because layers should be continuous
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
      led_on(color);
  }

  if (!drawing_flag && !space_flag) {
    off_all_valves(num_of_valves);
    pulse_io(SR_st_pin);
    init_drawing();
  }
  if (drawing_flag && !space_flag) {
    if (check_drawing()) {
//      Serial.println("done drawing");
//      drawing_flag = false;
      off_all_valves(num_of_valves);
      pulse_io(SR_st_pin);
      space_flag = true;
      drawing_flag = false;
      last_space_time = millis();
      cassette_drawing += 2;
      color += 1;
      if (color >= colors_num)
        color = 0;
      if (cassette_drawing >= cassettes_num) {
        cassette_drawing = 0;
//        drawing_index++;
//        if (drawing_index >= drawings_num)
//          drawing_index = 0;
      }
    }
  }
}

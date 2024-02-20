#ifndef ROUTINES_H
#define ROUTINES_H

#include "Consts.h"
#include "Display.h"

void pulse_io(byte);
void off_all_valves(uint16_t);
//void on_all_valves(int);
//void valve_on(int);
void image_to_valves(byte, bool);
//void array_to_valves_progmem(byte, byte, bool);
//void on_all_board(int, int);
void init_drawing();
bool check_drawing();
void do_encoder();
int read_encoder();
void led_on(byte);
void led_off();
int calc_time_factor(byte);

void pulse_io(byte io_num) {
  digitalWrite(io_num, HIGH);
  digitalWrite(io_num, LOW);
}

void off_all_valves(uint16_t num_of_valves) {
  digitalWrite(SR_data_pin, LOW);
  for (int i = 0; i < num_of_valves; i++)
    pulse_io(SR_clk_pin);
//  pulse_io(SR_st_pin);
}

//void on_all_valves(int num_of_valves){
//  digitalWrite(SR_data_pin, HIGH);
//  for (int i = 0; i < num_of_valves; i++)
//    pulse_io(SR_clk_pin);
//  digitalWrite(SR_data_pin, LOW);
//  pulse_io(SR_st_pin);
//}

//void valve_on(int valve_index) {
//  digitalWrite(SR_data_pin, HIGH);
//  pulse_io(SR_clk_pin);
//  digitalWrite(SR_data_pin, LOW);
//  for (int i = 0; i < valve_index; i++)
//    pulse_io(SR_clk_pin);
//  pulse_io(SR_st_pin);
//}

//void on_all_board(int cassette, int board) {
//  digitalWrite(SR_data_pin, HIGH);
//  for (int i = 0; i < valves_per_board; i++)
//    pulse_io(SR_clk_pin);
//  digitalWrite(SR_data_pin, LOW);
//  for (int i = 0; i < valves_per_cassette*cassette+valves_per_board*board; i++)
//    pulse_io(SR_clk_pin);
//  pulse_io(SR_st_pin);
//}

void image_to_valves(byte r_index, bool reverse) {
  if (reverse) {
    for (int i = image_w / 8 - 1; i >= 0; i--) {
      byte byteValue = image[r_index * 8 + i];
      for (int i = 0; i < 8; i++) {
        bool bitValue = bitRead(byteValue, i);
//        Serial.print(bitValue == true ? " " : "#");
        digitalWrite(SR_data_pin, !bitValue); // "true"=1=HIGH, "false"=0=LOW
        pulse_io(SR_clk_pin);
      }
    }
  }
  else {
    for (int i = 0; i < image_w / 8; i++) {
      byte byteValue = image[r_index * 8 + i];
      for (int i = 7; i >= 0; i--) {
        bool bitValue = bitRead(byteValue, i);
//        Serial.print(bitValue == true ? " " : "#");
        digitalWrite(SR_data_pin, !bitValue); // "true"=1=HIGH, "false"=0=LOW
        pulse_io(SR_clk_pin);
      }
    }
  }
//  Serial.println();
}

//void array_to_valves_progmem(byte d_index, byte r_index, bool reverse) {
//  if (reverse) {
//    for (int i = image_w / 8 - 1; i >= 0; i--) {
//      byte byteValue = pgm_read_byte(&drawings[d_index][r_index * 8 + i]);
//      for (int i = 0; i < 8; i++) {
//        bool bitValue = bitRead(byteValue, i);
//        Serial.print(bitValue == true ? " " : "#");
//        digitalWrite(SR_data_pin, !bitValue); // "true"=1=HIGH, "false"=0=LOW
//        pulse_io(SR_clk_pin);
//      }
//    }
//  }
//  else {
//    for (int i = 0; i < image_w / 8; i++) {
//      byte byteValue = pgm_read_byte(&drawings[d_index][r_index * 8 + i]);
//      for (int i = 7; i >= 0; i--) {
//        bool bitValue = bitRead(byteValue, i);
//        Serial.print(bitValue == true ? " " : "#");
//        digitalWrite(SR_data_pin, !bitValue); // "true"=1=HIGH, "false"=0=LOW
//        pulse_io(SR_clk_pin);
//      }
//    }
//  }
//  Serial.println();
//}


void init_drawing() {
  row_in_drawing = image_h - 1;
  led_start_flag = true;
  last_led_start = millis();
  if (full_light)
    led_on(color);
  else {
    if (led_start > 0)
      led_off();
    else
      led_on(color);
  }
  valve_on_flag = false;
  off_all_valves(num_of_valves);
  space_flag = false;
  led_on_flag = false;
  drawing_flag = true;
//  Serial.println("drawing...");
}

bool check_drawing() {
//    off_all_valves(num_of_valves);
//    for (int i = 0; i < image_w; i++)
//      Serial.print(drawings[drawing_index][row_in_drawing * image_w + i] == 1 ? "1":"0");
//    Serial.println();

  if (!valve_on_flag) {
    if (row_in_drawing == -1)
      return true;
    // drawing depth is the depth of the image - reapet for the neccesary amout of layers
    for (int i = 0; i < drawing_depth; i++) {
//      array_to_valves_progmem(drawing_index, row_in_drawing, cassette_drawing%2);
        image_to_valves(row_in_drawing, cassette_drawing%2);
    }
    digitalWrite(SR_data_pin, LOW);
    for (int i = 0; i < cassette_drawing * image_w; i++) {
      pulse_io(SR_clk_pin);
    }
    pulse_io(SR_st_pin);
    valve_on_flag = true;
    if (auto_factor_flag)
      auto_valve_on_time = calc_time_factor(row_in_drawing);
    last_valve_on = millis();
    row_in_drawing--;
  }
  return false;
}

int calc_time_factor(byte row) {
  return int(sqrt(view_height/(image_h*g*(row + 0.5))) * 1000);
}

// update height for auto factoring
void update_height() {
  view_height = valve_on_time*valve_on_time*image_h*g*(image_h-0.5) / 1000000;  // adjusting units
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
  encoder_pos += value;
  if (value == 0 && !on_button && digitalRead(encoder_sw) == LOW) {
    on_button = true;
    //    last_click = millis();
  }
  if (value == 0 && on_button && digitalRead(encoder_sw) == HIGH) {
    on_button = false;
    current_setting++;
    if (current_setting == 6)
      current_setting = 7;
    if (current_setting >= max_settings)
      current_setting = 0;
    display_settings();
  }
  if (value != 0 && (encoder_pos >= prescaler || encoder_pos <= -prescaler) && digitalRead(encoder_sw) == HIGH) {
    // TODO: write all the settings in a byte array. this is for now:
    encoder_pos = 0;
    switch (current_setting) {
      case 0:
        valve_on_time += (valve_on_time == max_valve_on_time && value == 1 ? 0 : (valve_on_time == min_valve_on_time && value == -1 ? 0 : value * valve_on_time_step));
        if (auto_factor_flag) update_height();
        break;
      case 1:
        space_time += (space_time == max_space_time && value == 1 ? 0 : (space_time == min_space_time && value == -1 ? 0 : value * space_time_step));
        break;
      case 2:
        led_start += (led_start == max_led_start && value == 1 ? 0 : (led_start == min_led_start && value == -1 ? 0 : value * led_start_step));
        break;
      case 3:
        led_on_time += (led_on_time == max_led_on_time && value == 1 ? 0 : (led_on_time == min_led_on_time && value == -1 ? 0 : value * led_on_time_step));
        break;
      case 4:
        drawing_depth += (drawing_depth == max_drawing_depth && value == 1 ? 0 : (drawing_depth == min_drawing_depth && value == -1 ? 0 : value * drawing_depth_step));
        break;
      case 5:
        full_light = !full_light;
        break;
//      case 6:
//        auto_factor_flag = !auto_factor_flag;
//        break;
      case 7:
        led_power += (led_power == max_led_power && value == 1 ? 0 : (led_power == min_led_power && value == -1 ? 0 : value * led_power_step));
        break;
    }
    display_settings();
  }
}

void led_on(byte index) {
  led_off();
  analogWrite(red_led_pin, colors[index][0]);
  analogWrite(green_led_pin, colors[index][1]);
  analogWrite(blue_led_pin, colors[index][2]);
}
void led_off() {
  analogWrite(red_led_pin, 0);
  analogWrite(green_led_pin, 0);
  analogWrite(blue_led_pin, 0);
}

#endif

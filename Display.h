
#include "Consts.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define i2c_Address 0x3c //initialize with the I2C Oled's addr 0x3C. Typically aliexpress, 0x3D Adafrout

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void start_display(){
  display.begin(i2c_Address, true); // Address 0x3C default
  display.clearDisplay();
  display.display();

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println(F(__FILE__ " " __DATE__ " " __TIME__));
  display.display();
  delay(4000);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("START");
  display.display();
  delay(500);
  display.clearDisplay();
  display.display();
}

void display_settings(){

//  cassette_num = min(max_cassettes, max(0, cassette_num)); //just in case
//  board_num = min(boards_per_cassette, max(0, board_num)); //just in case
//  valve_num = min(valves_per_board, max(0, valve_num)); //just in case
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print(current_setting==0?">Cst= ":" Cst=");
  display.println(cassette_num+1);
  display.print(current_setting==1?">Brd= ":" Brd=");
  char board_char = 'a'+board_num;
  display.println(board_char);  // 0->a, 1->b...
  display.print(current_setting==2?">Vlv= ":" Vlv=");
  display.println(valve_num+1);
  display.display();
//  display.print(cassette_num);
//  display.print("Pt=");
//  display.print(pulse_time);
//  display.println("ms");
//  display.print("St=");
//  display.print(space_time);
//  display.println("ms");
//  display.display();
}

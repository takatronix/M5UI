#include <M5UnitMiniOLED.h>
#include <M5UI.h>
#include <Wire.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

/*
M5UnitMiniOLED + M5CardKeyboardをキーボードにするサンプル
*/

#define CARDKB_ADDR 0x5F
USBHIDKeyboard Keyboard;

// offscreen buffer
M5UICanvas screen(&M5.Display);
TextSprite helloWorld(&screen);


void setup() {
  M5.begin();

  Wire.begin();
  Keyboard.begin();
  USB.begin();

  // create off-screen buffer and set up
  screen.setup();

  helloWorld.setTextSize(1);
  helloWorld.setLayout(LayoutType::ScreenCenter);
  helloWorld.setText("Hello World");
}

void loop() {
  M5.update();
  
  // Clear off-screen buffer
  screen.start();

  Wire.requestFrom(CARDKB_ADDR, 1);
  while (Wire.available()) {
    char c = Wire.read();
    if (c != 0) {
      Keyboard.print(c);
      Serial.println(c, HEX);
    }
  }



  // Draw off-screen on LCD
  screen.update();
}

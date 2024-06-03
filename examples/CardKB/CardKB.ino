/*
# CardKB
https://docs.m5stack.com/en/unit/cardkb

# Supported models

## OTG(USB) 
M5Stack FIRE/Core2/CoreS3
Atom S3

## Bluetooth
M5Stack Core2/CoreS3/AtomS3
M5Stick StickC/StickC Plus/StickC Plus2
M5Atom S3/S3 Lite

tyy7777uuhvvvv# Required Libraries

## ESP32-BLE-Keyboard
https://github.com/T-vK/ESP32-BLE-Keyboard

## M5UI
https://github.com/takatronix/M5UI

*/
#include <M5UI.h>
#include <Wire.h>


#define CARDKB_ADDR 0x5F
#define BT_NAME "M5 CardKeyboard"

// offscreen buffer
M5UICanvas screen(&M5.Display);
// 
ConsoleSprite console(&screen);
// battery level sprite
BatterySprite battery(&screen);

#ifdef CONFIG_USB_OTG_SUPPORTED
  #include "USB.h"
  #include "USBHIDKeyboard.h"
  USBHIDKeyboard USBKeyboard;
#endif

#ifdef CONFIG_BT_ENABLED
  #include <BleKeyboard.h>
  BleKeyboard BLEKeyboard(BT_NAME);
  bool blestate = false;
#endif

void setup() {
  M5.begin();
  Wire.begin();

  bool enabled = false;
#ifdef CONFIG_USB_OTG_SUPPORTED
  USBKeyboard.begin();
  USB.begin();
  console.println("USB Enabled");
  enabled = true;
#endif
#ifdef CONFIG_BT_ENABLED
  BLEKeyboard.begin();
  console.println("Bluetooth Enabled");
  enabled = true;
#endif
  if(enabled){
    console.println("Ready");
  }
  else{
    console.printf("This example requires USB or Bluetooth Keyboard support.");
  }

  // create off-screen buffer and set up
  screen.setup();
  //screen.enableRotation = false;

}

void loop() {
  M5.update();
  
  // Clear off-screen buffer
  screen.start();

  Wire.requestFrom(CARDKB_ADDR, 1);
  while (Wire.available()) {
    char c = Wire.read();
    if (c != 0) {
      char cc = convertKey(c);
      console.printf("%c",cc);

      #ifdef CONFIG_USB_OTG_SUPPORTED
      USBKeyboard.print(cc);
      #endif
      
      #ifdef CONFIG_BT_ENABLED
      if(BLEKeyboard.isConnected()){
        BLEKeyboard.print(cc);
      }
      #endif

      Serial.println(cc, HEX);
    }
  }

  screen.update();
}

char convertKey(char ch){
  switch(ch){
    case 0x0D:
      return KEY_RETURN;
    case 0x1B:
      return KEY_ESC;
    case 0x09:
      return KEY_TAB;
    case 0xB4:
      return KEY_LEFT_ARROW;
    case 0xB7:
      return KEY_RIGHT_ARROW;
    case 0xB5:
      return KEY_UP_ARROW;
    case 0xB6:
      return KEY_DOWN_ARROW;
    default:
      return ch;
  }
}

#include <M5UI.h>

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);
// battery level sprite
TextSprite battery(&screen,20,8);
// FPS sprite
TextSprite fps(&screen,30,8);

void setup() {
  M5.begin();

  // create off-screen buffer
  screen.setup();
  screen.printDeviceInfo();

  // startup sound
  Sound::beep(2000,100);
  Sound::beep(1000,100);
  
  // set font size
  screen.setTextSize(2);
  screen.println("Hello World");

  // set battery sprite position
  battery.moveToTopRight();
  // set FPS sprite position
  fps.moveToTopLeft();

}

void loop() {
  M5.update();
  
  // Displays remaining battery charge
  battery.setText(String(Device::getBatteryLevel()) + "%");
  // Displays the number of frames per second
  fps.setText(String(screen.getFPS()) + " FPS");

  // Draw off-screen on LCD
  screen.update();
}

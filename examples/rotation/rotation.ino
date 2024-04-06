#include <M5UI.h>


// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

// battery level sprite
TextSprite battery(&screen,60,16);
// FPS sprite
TextSprite fps(&screen,60,16);
// Time to draw the screen
TextSprite drawTime(&screen,50,16);
// Screen orientation sprite
TextSprite orientationName(&screen,180,32);

void setup() {
  M5.begin();

  // create off-screen buffer
  screen.setup();
  screen._enableRotation = true;

  // startup sound
  Sound::playNote(Note::C5,100);
  Sound::playNote(Note::G5,100);
  
  battery.setPosition(PositionType::TopRight);
  fps.setPosition(PositionType::TopLeft);
  drawTime.setPosition(PositionType::TopCenter);
  orientationName.setPosition(PositionType::Center);

  battery.setTextSize(2);
  fps.setTextSize(2);
  drawTime.setTextSize(2);
  orientationName.setTextSize(2);
  orientationName.setTextColor(TFT_RED);
}

void loop() {
  M5.update();
  
  screen.clear();

  // Displays remaining battery charge
  battery.setText(String(Device::getBatteryLevel()) + "%");
  // Displays the number of frames per second
  fps.setText(String(screen.getFPS()) + "FPS");
  // Displays the time it takes to draw the screen
  drawTime.setText(String(screen.getDrawTime()) + "ms");
  // Displays the orientation of the screen
  orientationName.setText(Device::getOrientationName());

  // Draw off-screen on LCD
  screen.update();
}

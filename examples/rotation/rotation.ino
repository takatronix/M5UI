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
  
  battery.setPositionType(PositionType::TopRight);
  fps.setPositionType(PositionType::TopLeft);
  drawTime.setPositionType(PositionType::TopCenter);
  orientationName.setPositionType(PositionType::Center);

  int size = 1;
  if(Device::isStack()) {
    size = 2;
  }

  battery.setTextSize(size);
  fps.setTextSize(size);
  drawTime.setTextSize(size);
  orientationName.setTextSize(size);
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

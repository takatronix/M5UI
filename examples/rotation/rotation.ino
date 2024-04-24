#include <M5UI.h>

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

// battery level sprite
BatterySprite battery(&screen);
// FPS sprite
TextSprite fps(&screen);
// Screen orientation sprite
TextSprite orientationName(&screen);

void setup() {
  M5.begin();

  // create off-screen buffer
  screen.setup();
  screen.enableRotation = true;

  // startup sound
  Sound::playNote(Note::C5,100);
  Sound::playNote(Note::G5,100);
  
  int size = 2;
  battery.setLayout(LayoutType::ScreenTopRight);

  fps.setLayout(LayoutType::ScreenTopLeft);
  fps.setTextSize(size);

  orientationName.setTextSize(size);
  orientationName.setTextColor(TFT_YELLOW);
  orientationName.setLayout(LayoutType::ScreenCenter);
}

void loop() {
  M5.update();
  
  // Clear off-screen buffer
  screen.start();

  // Displays the number of frames per second
  fps.setText(String(screen.getFPS()) + "FPS");
  // Displays the orientation of the screen
  orientationName.setText(Device::getOrientationName());
 
  // Draw off-screen on LCD
  screen.update();
}

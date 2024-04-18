#include <M5UI.h>

/*
Samples of basic off-screen buffer creation and sprite creation
基本的なオフスクリーンバッファの作成とスプライトの作成のサンプル
*/

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);
// battery level sprite
TextSprite battery(&screen,20,8);
// FPS sprite
TextSprite fps(&screen,30,8);
// 描画時間
TextSprite drawTime(&screen,40,8);

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

  battery.setPosition(LayoutType::ScreenTopRight);
  fps.setPosition(LayoutType::ScreenTopLeft);
  drawTime.setPosition(LayoutType::ScreenTopCenter);
 

}

void loop() {
  M5.update();
  
  // Displays remaining battery charge
  battery.setText(String(Device::getBatteryLevel()) + "%");
  // Displays the number of frames per second
  fps.setText(String(screen.getFPS()) + "FPS");
  // Displays the drawing time
  drawTime.setText(String(screen.getDrawTime()) + "ms");

  // Draw off-screen on LCD
  screen.update();
}

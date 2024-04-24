#include <M5UI.h>

/*
Samples of basic off-screen buffer creation and sprite creation
基本的なオフスクリーンバッファの作成とスプライトの作成のサンプル
*/

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);
// battery level sprite
BatterySprite battery(&screen);
// FPS sprite
TextSprite fps(&screen);
// 描画時間
TextSprite drawingTime(&screen);
// Hello World sprite
TextSprite helloWorld(&screen);

void setup() {
  M5.begin();

  // create off-screen buffer
  screen.setup();
  // 自動回転を無効にする場合はfalseに設定
  //screen.enableRotation = false;

  // startup sound
  Sound::beep(2000,100);
  Sound::beep(1000,100);
  
  // set postion
  battery.setLayout(LayoutType::ScreenTopRight);
  fps.setTextSize(2);
  fps.setLayout(LayoutType::ScreenTopLeft);
  drawingTime.setLayout(LayoutType::ScreenTopCenter);

  helloWorld.setTextSize(2);
  helloWorld.setLayout(LayoutType::ScreenCenter);
  helloWorld.setText("Hello World");
}

void loop() {
  M5.update();
  
  // Clear off-screen buffer
  screen.start();

  // Displays the number of frames per second
  fps.setText(String(screen.getFPS()) + "FPS");
  // Displays the drawing time
  drawingTime.setText(String(screen.getDrawingTime()) + "ms");

  // Draw off-screen on LCD
  screen.update();
}

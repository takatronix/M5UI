#include <M5UI.h>

/*
Samples of basic off-screen buffer creation and sprite creation
基本的なオフスクリーンバッファの作成とスプライトの作成のサンプル
*/

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

TextSprite topLeft(&screen);
TextSprite topCenter(&screen);
TextSprite topRight(&screen);

TextSprite middleLeft(&screen);
TextSprite center(&screen);
TextSprite middleRight(&screen);

TextSprite bottomLeft(&screen);
TextSprite bottomCenter(&screen);
TextSprite bottomRight(&screen);


void setup() {
  M5.begin();

  // create off-screen buffer and set up
  screen.setup();
  // 自動回転を無効にする場合はfalseに設定
  //screen.enableRotation = false;

  // startup sound
  Sound::beep(2000,100);
  Sound::beep(1000,100);
  
  
  int textSize = 1;

  topLeft.setTextSize(textSize);
  topCenter.setTextSize(textSize);
  topRight.setTextSize(textSize);

  middleLeft.setTextSize(textSize);
  center.setTextSize(textSize);
  middleRight.setTextSize(textSize);
  
  bottomLeft.setTextSize(textSize);
  bottomCenter.setTextSize(textSize);
  bottomRight.setTextSize(textSize);
  
  // set postion
  topLeft.setLayout(LayoutType::ScreenTopLeft);
  topCenter.setLayout(LayoutType::ScreenTopCenter);
  topRight.setLayout(LayoutType::ScreenTopRight);

  middleLeft.setLayout(LayoutType::ScreenMiddleLeft);
  center.setLayout(LayoutType::ScreenCenter);
  middleRight.setLayout(LayoutType::ScreenMiddleRight);

  bottomLeft.setLayout(LayoutType::ScreenBottomLeft);
  bottomCenter.setLayout(LayoutType::ScreenBottomCenter);
  bottomRight.setLayout(LayoutType::ScreenBottomRight);

  // set text
  topLeft.setText("TopLeft");
  topCenter.setText("TopCenter");
  topRight.setText("TopRight");

  middleLeft.setText("MiddleLeft");
  center.setText("Center");
  middleRight.setText("MiddleRight");

  bottomLeft.setText("BottomLeft");
  bottomCenter.setText("BottomCenter");
  bottomRight.setText("BottomRight");


}

void loop() {
  M5.update();
  
  // Clear off-screen buffer
  screen.start();


  // Draw off-screen on LCD
  screen.update();
}

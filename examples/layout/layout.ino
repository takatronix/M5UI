#include <M5UI.h>

/*
Samples of basic off-screen buffer creation and sprite creation
基本的なオフスクリーンバッファの作成とスプライトの作成のサンプル
*/

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

TextSprite topLeft(&screen,"TopLeft",LayoutType::ScreenTopLeft);
TextSprite topCenter(&screen,"TopCenter",LayoutType::ScreenTopCenter);
TextSprite topRight(&screen,"TopRight",LayoutType::ScreenTopRight);

TextSprite middleLeft(&screen,"MiddleLeft",LayoutType::ScreenMiddleLeft);
TextSprite center(&screen,"Center",LayoutType::ScreenCenter);
TextSprite middleRight(&screen,"MiddleRight",LayoutType::ScreenMiddleRight);

TextSprite bottomLeft(&screen,"BottomLeft",LayoutType::ScreenBottomLeft);
TextSprite bottomCenter(&screen,"BottomCenter",LayoutType::ScreenBottomCenter);
TextSprite bottomRight(&screen,"BottomRight",LayoutType::ScreenBottomRight);


void setup() {
  M5.begin();

  // create off-screen buffer and set up
  screen.setup();
  // 自動回転を無効にする場合はfalseに設定
  //screen.enableRotation = false;

}

void loop() {
  M5.update();
  
  // Clear off-screen buffer
  screen.start();


  // Draw off-screen on LCD
  screen.update();
}

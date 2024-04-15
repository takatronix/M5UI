#include <M5UI.h>

// resouce
#include "image_man10.h"

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

Sprite test(&screen,50,50,100,100 );

// sprites -> offscreen buffer
ImageSprite man10(&screen,image_man10_blue, 26, 54,160,120,true,0x03);  

TextSprite fps(&screen,60,16);
TextSprite drawTime(&screen,50,16);
TextSprite battery(&screen,60,16);
TextSprite animationName(&screen,180,16);

struct Star {
  float angle;
  float radius;
  float speed;
  uint32_t color;  // 星の色
};

constexpr int numStars = 1000;
Star stars[numStars];
constexpr int centerX = 160;  // 画面中心のX座標
constexpr int centerY = 120;  // 画面中心のY座標
constexpr float maxRadius = sqrt(centerX * centerX + centerY * centerY);  // 画面の対角線の長さの半分


void setup() {
  M5.begin();

  // startup sound
  Sound::beep(2000,100);
  Sound::beep(1000,100);


  // 星の初期化
  for (int i = 0; i < numStars; ++i) {
    stars[i].angle = random(0, 360) * PI / 180.0;
    stars[i].radius = 0;
    // より大きな速度範囲を持たせる
    stars[i].speed = random(5, 50) / 10.0;  // 0.5から5.0の間でランダムに設定
    stars[i].color = WHITE;  // 星の色を白に固定
  }

  // create off-screen buffer
  screen.setup();

  // set postion
  battery.setPosition(PositionType::TopRight);
  fps.setPosition(PositionType::TopLeft);
  drawTime.setPosition(PositionType::TopCenter);
  animationName.setPosition(PositionType::BottomCenter);

  if(Device::isStack()) {
    battery.setTextSize(2);
    fps.setTextSize(2);
    drawTime.setTextSize(2);
    animationName.setTextSize(2);
  }

  man10.enableTransparent = true;
  man10.enableAffine = true;
  //man10.enableAA = true;
 // man10.setPivotCenter();

    man10.setOriginCenter();
  test.enableAffine = false;
  test.enableTransparent = true;
  test.setTransparentColor(0x03);

  //test.setScale(1.5);
  fps.enableAffine = true;

}

void drawStarField() {
  for (int i = 0; i < numStars; ++i) {
    stars[i].radius += stars[i].speed;
    uint8_t brightness = map(stars[i].radius, 0, maxRadius, 0, 255);  // 輝度を調整
    uint32_t fadedColor = M5.Display.color888(brightness, brightness, brightness);  // 輝度に基づいた色
    int x = centerX + stars[i].radius * cos(stars[i].angle);
    int y = centerY + stars[i].radius * sin(stars[i].angle);

    if (x < 0 || x >= M5.Display.width() || y < 0 || y >= M5.Display.height() || stars[i].radius > maxRadius) {
      stars[i].angle = random(0, 360) * PI / 180.0;
      stars[i].radius = 0;
      stars[i].speed = random(5, 50) / 10.0;  // 速度を再設定
      stars[i].color = WHITE;
    }

    screen.drawPixel(x, y, fadedColor);
  }
}

void loop() {
  M5.update();

  // clear off-screen buffer
  screen.clear();

  drawStarField();


  int x = random(0,320);
  int y = random(0,240);
  test.moveTo(x,y,10);

 if (M5.Touch.isEnabled()) {
    // タッチしたX座標、Y座標を取得する
    auto t = M5.Touch.getDetail();
    auto x = t.x;
    auto y = t.y;
    auto p = t.isPressed();
    auto r = t.isReleased();
    auto h = t.isHolding();
    auto d = t.isDragging();
    auto f = t.isFlicking();
    auto c = t.getClickCount();
    if(t.isPressed()){
      LOG_D("pressed is x:%d y:%d p:%d r:%d h:%d d:%d f:%d c:%d",x,y,p,r,h,d,f,c);
     //  man10.setX(x);
      // man10.setY(y);

      // man10.setPivotCenter();
       
    }
  }
  // Aボタンでアニメーションを再生
  if(M5.BtnA.wasPressed()) {
    Sound::beep();
    screen.clear();
    man10.setAngle(0);
    man10.setScale(1);
    man10.setOrigin(0,0);
    animationName.setText("reset"); 
  }


  if(M5.BtnB.isPressed()){
      float angle = man10.angle();
      man10.setAngle(man10.angle()+5);
      man10.setOriginCenter();
   }

  if(M5.BtnC.isPressed()) {
    float scale = man10.scale();
    scale *= 1.1;
    man10.setScale(scale);
    man10.setOriginCenter();
  }



  battery.setText(String(Device::getBatteryLevel()) + "%");
  fps.setText(String(screen.getFPS()) + "FPS");
  drawTime.setText(String(screen.getDrawTime()) + "ms");

  // show all sprites
  screen.update();
  delay(1);   // for battery

}


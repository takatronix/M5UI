#include <M5UI.h>

// resouce
#include "image_man10.h"

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

Sprite test(&screen,50,50,100,100 );

// sprites -> offscreen buffer
ImageSprite man10(&screen,image_man10_blue, 26, 54,70,70, true,0x03);  

TextSprite fps(&screen,60,16);
TextSprite drawTime(&screen,50,16);
TextSprite battery(&screen,60,16);
TextSprite animationName(&screen,180,16);




void setup() {
  M5.begin();

  // startup sound
  Sound::beep(2000,100);
  Sound::beep(1000,100);
  Sound::playNote(Note::E5,100);
  Sound::playNote(Note::A3,100);

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

  man10.setPosition(PositionType::Center);
  man10.enableTransparent = true;
  man10.enableAffine = true;
  man10.setScale(3);



  man10.setAngle(45, 3000, TweenType::LINEAR, [](){
      LOG_V("Tween Complete");
      Sound::beep();


  });



}


void loop() {
  M5.update();

  // clear off-screen buffer
  screen.clear();

  // Aボタンでアニメーションを再生
  if(M5.BtnA.wasPressed()) {
    Sound::beep();
    screen.clear(BLUE);
    man10.moveTo(0,0,100);
    animationName.setText("Animation 0");
  }
  int x = random(0,320);
  int y = random(0,240);
  test.moveTo(x,y);

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
    LOG_D("x:%d y:%d p:%d r:%d h:%d d:%d f:%d c:%d",x,y,p,r,h,d,f,c);
    if(x!=-1 && y!=-1){
       man10.setX(x);
       man10.setY(y);
    }
  }



  battery.setText(String(Device::getBatteryLevel()) + "%");
  fps.setText(String(screen.getFPS()) + "FPS");
  drawTime.setText(String(screen.getDrawTime()) + "ms");

  // show all sprites
  screen.update();
}


#include <M5UI.h>


// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

// battery level sprite
TextSprite battery(&screen,60,16);
// FPS sprite
TextSprite fps(&screen,60,16);
// Time to onDraw the screen
TextSprite drawTime(&screen,50,16);

TextSprite animationName(&screen,180,16);
 Sprite ball(&screen,64,64);

auto tween = TweenType::LINEAR;

void setup() {
  M5.begin();

  // create off-screen buffer
  screen.setup();

  // startup sound
  Sound::playNote(Note::E5,100);
  Sound::playNote(Note::A3,100);
  
  battery.setPosition(LayoutType::ScreenTopRight);
  fps.setPosition(LayoutType::ScreenTopLeft);
  drawTime.setPosition(LayoutType::ScreenTopCenter);
  animationName.setPosition(LayoutType::ScreenBottomCenter);

  int size = 1;
  if(Device::isStack()) {
    size = 2;
  }

  battery.setTextSize(size);
  fps.setTextSize(size);
  drawTime.setTextSize(size);
  animationName.setTextSize(size);

  tween =getNextTweenType();

  // 一番したに移動
  ball.setPosition(LayoutType::ScreenBottomCenter);
  ball.moveTo(ball.x(),0,tween,1000,true);

}

void loop() {
  M5.update();
  
  screen.clear();

  // Aボタンを押すと、アニメーションを再生
  if(M5.BtnA.wasPressed()) {
    Sound::beep();
    tween =getNextTweenType();

  // 一番したに移動
    ball.setPosition(LayoutType::ScreenBottomCenter);
    ball.moveTo(ball.x(),0,tween,1000,true);
  }


  // Displays remaining battery charge
  battery.setText(String(Device::getBatteryLevel()) + "%");
  // Displays the number of frames per second
  fps.setText(String(screen.getFPS()) + "FPS");
  // Displays the time it takes to onDraw the screen
  drawTime.setText(String(screen.getDrawTime()) + "ms");

  animationName.setText(Tween::tweenName(tween));
  // Draw off-screen on LCD
  screen.update();
}

// 次のTweenTypeを返す関数
TweenType getNextTweenType() {
    static int currentType = static_cast<int>(TweenType::LINEAR); // LINEARから開始
    TweenType nextType = static_cast<TweenType>(currentType);
    
    // 次のタイプに進める
    currentType = currentType % static_cast<int>(TweenType::SINUSOIDAL) + 1;
    
    return nextType;
}

#include <M5UI.h>


// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);


// 画面に表示するスプライト(宣言の順序で下から描画されます)

// battery level sprite
TextSprite battery(&screen,60,16);
// FPS sprite
TextSprite fps(&screen,60,16);
//　Drawing time
TextSprite drawingTime(&screen,50,16);
// Ball sprite
Sprite ball(&screen,64,64,0,0,0,true);
// Animation name
TextSprite animationName(&screen,180,16);

// アニメーションの種類
auto tween = TweenType::LINEAR;

void setup() {
  M5.begin();

  // create off-screen buffer
  screen.setup();

  // startup sound
  Sound::playNote(Note::E5,100);
  Sound::playNote(Note::A3,100);
  
  battery.setLayout(LayoutType::ScreenTopRight);
  fps.setLayout(LayoutType::ScreenTopLeft);
  drawingTime.setLayout(LayoutType::ScreenTopCenter);
  animationName.setLayout(LayoutType::ScreenCenter);
  ball.canvas.fillCircle(32,32,30,RED);

  int size = 1;
  if(Device::isStack()) {
    size = 2;
  }

  battery.setTextSize(size);
  fps.setTextSize(size);
  drawingTime.setTextSize(size);
  animationName.setTextSize(size);

  tween = getNextTweenType();

  // 一番したに移動
  ball.setLayout(LayoutType::ScreenBottomCenter);
  ball.moveTo(ball.x(),0,1000,tween);

}

void loop() {
  M5.update();

  // 画面をクリアして描画を開始
  screen.start();

  // Aボタンを押すと、アニメーションを再生
  if(M5.BtnA.wasPressed()) {
    Sound::beep();
    tween = getNextTweenType();
    startAnimation();
  }


  // Displays remaining battery charge
  battery.setText(String(Device::getBatteryLevel()) + "%");
  // Displays the number of frames per second
  fps.setText(String(screen.getFPS()) + "FPS");
  // Displays the time it takes to onDraw the screen
  drawingTime.setText(String(screen.getDrawingTime()) + "ms");

  // Draw off-screen on LCD
  screen.update();
}

void startAnimation()
{
    
  animationName.setText(Tween::tweenName(tween));

  ball.setLayout(LayoutType::ScreenBottomCenter);
  ball.moveTo(ball.x(),0,1000,tween);

}


// 次のTweenTypeを返す関数
TweenType getNextTweenType() {
    static int currentType = static_cast<int>(TweenType::LINEAR); // LINEARから開始
    TweenType nextType = static_cast<TweenType>(currentType);
    
    // 次のタイプに進める
    currentType = currentType % static_cast<int>(TweenType::SINUSOIDAL) + 1;    
    return nextType;
}

#include <M5UI.h>


// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);


// 画面に表示するスプライト(宣言の順序で下から描画されます)
// Ball sprite
Sprite ball(&screen,64,64);

// battery level sprite
BatterySprite battery(&screen,32,16);

// FPS sprite
TextSprite fps(&screen);

// Animation name
TextSprite animationName(&screen);

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

  animationName.setLayout(LayoutType::ScreenCenter);
  animationName.setTextSize(2);
  animationName.enableTransparent = true;

  fps.setTextSize(2);
  fps.setLayout(LayoutType::ScreenTopLeft);
  fps.enableTransparent = true;

  ball.setLayout(LayoutType::ScreenBottomCenter);
  ball.canvas.fillCircle(32,32,30,RED);
  ball.moveTo(ball.x(),0,1000,tween);

  tween = getNextTweenType();
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

  // Displays the number of frames per second
  fps.setText(String(screen.getFPS()) + "FPS");

  // Draw off-screen on LCD
  screen.update();
}

void startAnimation()
{
    
  animationName.setText(Tween::tweenName(tween));

  // ボールを画面下から中央に移動
  ball.setPosition(LayoutType::ScreenBottomCenter);
  auto pos = ball.getScreenPosition(LayoutType::ScreenTopCenter);
  ball.moveTo(pos.first,pos.second,1000,tween);

}


// 次のTweenTypeを返す関数
TweenType getNextTweenType() {
    static int currentType = static_cast<int>(TweenType::LINEAR); // LINEARから開始
    TweenType nextType = static_cast<TweenType>(currentType);
    
    // 次のタイプに進める
    currentType = currentType % static_cast<int>(TweenType::SINUSOIDAL) + 1;    
    return nextType;
}

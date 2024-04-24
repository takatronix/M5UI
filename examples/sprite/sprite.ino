#include <M5UI.h>

// resouce
#include "image_man10.h"

#include "StarFieldRenderer.hpp"
#include "RippleRenderer.hpp"

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

//ConsoleSprite console(&screen);

// sprites -> offscreen buffer
ImageSprite man10(&screen, image_man10_blue, image_man10_width, image_man10_height, 0, 0, true, 0x03);
TextSprite fps(&screen);
TextSprite title(&screen);
BatterySprite battery(&screen);
\

void setup()
{

  auto cfg = M5.config();
  // cfg.external_speaker.hat_spk = true;
  cfg.external_speaker.hat_spk2 = true;

  M5.begin(cfg);
  //WiFi.beginSmartConfig();

  // startup sound
  Sound::beep(2000, 100);
  Sound::beep(1000, 100);

  // create off-screen buffer
  screen.setup();
  screen.enableRotation = true;

  // set postion
  battery.setLayout(LayoutType::ScreenTopRight);
  fps.setLayout(LayoutType::ScreenTopLeft);
  title.setLayout(LayoutType::ScreenCenter);
  fps.setTextSize(2);
  man10.enableTransparent = true;

  title.setTextSize(2);
  title.setText("Hello M5UI!");
  title.enableTransparent = true;
  title.setOriginToCenter();
  title.setAngle(360, 1000, TweenType::EASE_OUT);

  // console.setup();
 // console.hidden = true;
  printDeviceInformation();

  title.hide();
  man10.hide();
  screen.add(new StarFieldRenderer());
  screen.add(new RippleRenderer());

  // 回転する原点を中心点に設定
  man10.setOriginToCenter();

  // 画面が回転したときに中央に移動回転する
  screen.onRotate([](int rotation){
    title.moveToCenter();
    man10.moveToCenter(); 
  });

  startAnimation();



}

TweenType animationType = TweenType::EASE_IN_OUT;

void startAnimation()
{
  man10.hidden = false;
  man10.setOriginToCenter();
  man10.moveToCenter();
  man10.setAngle(360, 2000, animationType);
  man10.setScale(5, 2000, animationType, []()
                 {
                   man10.setScale(1, 1500, animationType, []()
                                  { startAnimation(); });
                   man10.setAngle(0, 1500, animationType);
                 });
}

void printDeviceInformation()
{
  /*
  console.setTextColor(TFT_YELLOW);
  console.printf("Board:%s\n", Device::getBoardName());
  console.printf("Screen:%dx%d\n", M5.Display.width(), M5.Display.height());
  console.printf("Battery:%d\n", Device::getBatteryLevel());
  console.printf("Orientation:%s\n", Device::getOrientationName());
  console.printf("FreeMemory:%d\n", Device::getFreeDmaSize());
  console.printf("FreeBlock:%d\n", Device::getLargestFreeBlock());
  console.setTextColor(TFT_GREEN);
*/
  Serial.printf("Board:%s\n", Device::getBoardName());
  Serial.printf("Screen:%dx%d\n", M5.Display.width(), M5.Display.height());
  Serial.printf("Battery:%d\n", Device::getBatteryLevel());
  Serial.printf("FreeMemory:%d\n", Device::getFreeDmaSize());
  Serial.printf("FreeBlock:%d\n", Device::getLargestFreeBlock());
}

void loop()
{
  M5.update();
  screen.start();

  if (Device::wasShake())
  {
    //console.toggle();
    Sound::beep();
    screen.resetFPS();
    printDeviceInformation();
   // console.println("Shake!!");
  }

  if (M5.Touch.isEnabled())
  {
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
    if (t.isPressed())
    {
      LOG_D("pressed is x:%d y:%d p:%d r:%d h:%d d:%d f:%d c:%d", x, y, p, r, h, d, f, c);
    }
  }

  if (M5.BtnA.wasPressed())
  {
    screen.resetFPS();
    screen.clear();
    man10.setAngle(0);
    man10.setScale(1);

    man10.moveToCenter();
    man10.setOriginToCenter();

    printDeviceInformation();
  //  console.println("A Button is pressed");
  }
  // Aボタン長押し
  if (M5.BtnA.wasHold())
  {
  //  console.println("A Button is holding");
    screen.resetFPS();
    //  console.resize(16,16);
  }

  if (M5.BtnB.wasPressed())
  {
    float angle = man10.angle();
    man10.setAngle(man10.angle() + 5);
    man10.setOriginToCenter();
   // console.println("B Button is pressed");
  }

  if (M5.BtnC.wasPressed())
  {
    float scale = man10.scale();
    scale *= 1.1;
    man10.setScale(scale);
    man10.setOriginToCenter();
   // console.println("C Button is pressed");
  }

  char buf[128];
  sprintf(buf, "%dFPS %dms %dKB",
          screen.getFPS(),
          screen.getDrawingTime(),
          Device::getLargestFreeBlock() / 1024);
  fps.setText(buf);

  // show all sprites
  screen.update();
  delay(1); // for battery
}

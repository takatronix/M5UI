#include <M5UI.h>

// resouce
#include "image_man10.h"

// offscreen buffer -> LCD
M5UICanvas screen(&M5.Display);

Sprite test(&screen, 50, 50, 100, 100);
ConsoleSprite console(&screen);

// sprites -> offscreen buffer
ImageSprite man10(&screen, image_man10_blue, 26, 54, 160, 120, true, 0x03);
TextSprite fps(&screen);

TextSprite infoText(&screen);
BatterySprite battery(&screen, 32, 16);

struct Star
{
  float angle;
  float radius;
  float speed;
  uint32_t color; // 星の色
};

constexpr int numStars = 100;
Star stars[numStars];
constexpr int centerX = 160;                                             // 画面中心のX座標
constexpr int centerY = 120;                                             // 画面中心のY座標
constexpr float maxRadius = sqrt(centerX * centerX + centerY * centerY); // 画面の対角線の長さの半分
// #include "Audio.h"

// Digital I/O used
#define I2S_DOUT 25
#define I2S_BCLK 26
#define I2S_LRC 0

void setup()
{

  auto cfg = M5.config();
  // cfg.external_speaker.hat_spk = true;
  cfg.external_speaker.hat_spk2 = true;

  M5.begin(cfg);
  //  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  //  audio.setVolume(21);  // default 0...21
  // startup sound
  Sound::beep(2000, 100);
  Sound::beep(1000, 100);

  // 星の初期化
  for (int i = 0; i < numStars; ++i)
  {
    stars[i].angle = random(0, 360) * PI / 180.0;
    stars[i].radius = 0;
    // より大きな速度範囲を持たせる
    stars[i].speed = random(5, 50) / 10.0; // 0.5から5.0の間でランダムに設定
    stars[i].color = WHITE;                // 星の色を白に固定
  }

  // create off-screen buffer
  screen.setup();

  // set postion
  // battery.setLayout(LayoutType::ScreenTopRight);
  battery.setLayout(LayoutType::ScreenTopRight);

  fps.setLayout(LayoutType::ScreenTopLeft);
  infoText.setLayout(LayoutType::ScreenCenter);

  fps.setTextSize(2);
  man10.EnableTransparent = true;

  infoText.setTextSize(2);
  infoText.setText("Hello M5UI!");
  infoText.EnableTransparent = true;
  infoText.setOriginCenter();
  infoText.setAngle(360, 1000, TweenType::EASE_OUT);

  
  console.setTextSize(2);
  console.EnableTransparent = true;
  console.setLayout(LayoutType::ScreenBottomLeft);
  printDeviceInformation();
}

void drawStarField()
{
  float centerX = M5.Display.width() / 2;
  float centerY = M5.Display.height() / 2;

  for (int i = 0; i < numStars; ++i)
  {
    stars[i].radius += stars[i].speed;
    uint8_t brightness = map(stars[i].radius, 0, maxRadius, 0, 255);               // 輝度を調整
    uint32_t fadedColor = M5.Display.color888(brightness, brightness, brightness); // 輝度に基づいた色
    int x = centerX + stars[i].radius * cos(stars[i].angle);
    int y = centerY + stars[i].radius * sin(stars[i].angle);

    if (x < 0 || x >= M5.Display.width() || y < 0 || y >= M5.Display.height() || stars[i].radius > maxRadius)
    {
      stars[i].angle = random(0, 360) * PI / 180.0;
      stars[i].radius = 0;
      stars[i].speed = random(5, 50) / 10.0; // 速度を再設定
      stars[i].color = WHITE;
    }

    screen.drawPixel(x, y, fadedColor);
  }
}


void printDeviceInformation()
{
  console.setTextColor(TFT_YELLOW);
  console.printf("Board:%s\n", Device::getBoardName());
  console.printf("Screen:%dx%d\n", M5.Display.width(), M5.Display.height());
  console.printf("Battery:%d\n", Device::getBatteryLevel());
  console.printf("Orientation:%s\n", Device::getOrientationName());
  console.printf("FreeMemory:%d\n", Device::getFreeDmaSize());
  console.printf("FreeBlock:%d\n", Device::getLargestFreeBlock());
  console.setTextColor(TFT_GREEN);
}



void loop()
{

  M5.update();

  // screen.clear( M5.Display.color888(random(0,255), random(0,255), random(0,255)));
  screen.clear();
  drawStarField();

  // デバイスの向きが変わったら画面の向きも変える
  if (Device::wasOrientationChanged())
  {
    int rotation = Device::getRotation();
    if (rotation != -1)
    {
      M5.Display.setRotation(rotation);
      screen.updateSpritePosition();
      man10.moveToCenter();
      man10.setOriginCenter();

    }
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

      //  man10.setX(x);
      // man10.setY(y);

      // man10.setPivotCenter();
    }
  }

  if (M5.BtnA.wasPressed())
  {
    Sound::beep();
    screen.resetFPS();
    screen.clear();
    man10.setAngle(0);
    man10.setScale(1);

    man10.moveToCenter();
    man10.setOriginCenter();

    printDeviceInformation();
  }

  if (M5.BtnB.isPressed())
  {
    float angle = man10.angle();
    man10.setAngle(man10.angle() + 5);
    man10.setOriginCenter();
    console.println("B Button is pressed");
  }

  if (M5.BtnB.wasPressed())
  {
  }

  if (M5.BtnC.wasPressed())
  {
    float scale = man10.scale();
    scale *= 1.1;
    man10.setScale(scale);
    man10.setOriginCenter();
    console.println("C Button is pressed");
  }

  fps.setText(String(screen.getFPS()) + "FPS" + " " + String(screen.getDrawTime()) + "ms");


  // show all sprites
  screen.update();
  delay(1); // for battery
}

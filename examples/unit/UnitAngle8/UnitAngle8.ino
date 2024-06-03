#include <M5UI.h>

// offscreen buffer
M5UICanvas screen(&M5.Display);
// battery level sprite
TextSprite fps(&screen);
BatterySprite battery(&screen);

UnitAngle8  unitAngle;

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);

    // オフスクリーンバッファの初期化
    screen.enableRotation = false;

    screen.setup();
    screen.removeAllRenderers();
    screen.setCursor(0, 0);
    screen.setTextSize(1);
    screen.setTextColor(GREEN);
    screen.setRotation(2);

    unitAngle.begin();
}

void loop()
{
    M5.update();
    screen.start();

    
    unitAngle.update();

    // 画面表示
    screen.clear();
    screen.setCursor(0, 0);
 

    fps.setText("FPS:" + String(screen.getFPS()));

    screen.update();

   // delay(10);
}

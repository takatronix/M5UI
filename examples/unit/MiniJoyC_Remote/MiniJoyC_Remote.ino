#include <M5UI.h>
#include "RaderSprite.hpp"
#include "LevelSprite.hpp"
#include "BTSerial.h"

// Bluetooth device name
#define BT_DEVICE_NAME "MiniJoyC_Remote"
// Destination device name
#define BT_TARGET_DEVICE_NAME "InvertedPendulum"

BTSerial btSerial(BT_DEVICE_NAME, true, BT_TARGET_DEVICE_NAME);

// offscreen buffer
M5UICanvas screen(&M5.Display);
// battery level sprite
TextSprite fps(&screen);
BatterySprite battery(&screen);

RaderSprite rader(&screen,120,120,LayoutType::ScreenBottomCenter);
LevelSprite level(&screen, 128, 8, LayoutType::ScreenTopLeft);
TextSprite message(&screen,"Hello",LayoutType::ScreenCenter);


UnitMiniJoyC unitJoyC;
UnitAngle8  unitAngle;

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);

    // オフスクリーンバッファの初期化
   // screen.enableRotation = false;

    screen.setup();
    screen.removeAllRenderers();
    screen.setCursor(0, 0);
    screen.setTextSize(1);
    screen.setTextColor(GREEN);
    screen.setRotation(0);

    message.setTextSize(2);
    message.setText("MiniJoyC Remote");
    screen.update();
  //  unitAngle.begin();
    if(!unitJoyC.begin()){
        screen.printf("JoyC: Disconnected\n");
    } 
    LOG_I("BT devce name:%s", BT_DEVICE_NAME);
    LOG_I("BT target device name:%s", BT_TARGET_DEVICE_NAME);

    LOG_I("BT devce name:%s", btSerial.getDeviceName());

    message.setText("BT Serial");
    Sprite::updateLayout();
    screen.update();
    message.setText("test");
    screen.update();
    message.setText("test2");

  //  btSerial.begin();
}

void loop()
{
    M5.update();
    screen.start();

    unitJoyC.update();
//    unitAngle.update();

    // 画面表示
    screen.clear();
    screen.setCursor(0, 0);
    screen.setTextSize(1);

    if(unitJoyC.isConnected()){
  //      screen.printf("%d, %d\n", unitJoyC.x, unitJoyC.y);
    }else{
        screen.printf("JoyC: Disconnected\n");
    }

    //screen.printf("minx:%.2f, maxx:%.2f\n", unitJoyC.ad_min_x, unitJoyC.ad_max_x);
    //screen.printf("miny:%.2f, maxy:%.2f\n", unitJoyC.ad_min_y, unitJoyC.ad_max_y);
    



    level.setLevel(unitJoyC.x);

    // 小数点以下2桁まで表示
    char buf[128];
    sprintf(buf,"%.2f,%.2f\n", unitJoyC.x, unitJoyC.y);
    rader.addPosition(unitJoyC.x, unitJoyC.y,buf);

    btSerial.send(buf);
    btSerial.update();

    fps.setText("FPS:" + String(screen.getFPS()));

    screen.update();

   // delay(10);
}

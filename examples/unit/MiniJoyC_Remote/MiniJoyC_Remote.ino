#include <M5UI.h>
#include "RaderSprite.hpp"
#include "LevelSprite.hpp"

#include "BluetoothSprite.h"
#include "UnitAngle8Task.h"
#include "UnitMiniJoyCTask.h"

// Bluetooth device name
#define BT_DEVICE_NAME "MiniJoyC_Remote"
// Destination device name
#define BT_TARGET_DEVICE_NAME "InvertedPendulum"

BTSerial btSerial(BT_DEVICE_NAME, true, BT_TARGET_DEVICE_NAME);

// offscreen buffer
M5UICanvas screen(&M5.Display);

RaderSprite rader(&screen,128,128,LayoutType::ScreenBottomCenter);
TextSprite message(&screen,"",LayoutType::ScreenCenter);

// battery level sprite
TextSprite fps(&screen,"",LayoutType::ScreenTopLeft);
TextSprite mem(&screen,"",LayoutType::ScreenMiddleLeft);
BatterySprite battery(&screen);

//BluetoothSprite bt(&screen, 32, 32, LayoutType::ScreenBottomRight);

#define LV_WIDTH 128
#define LV_HEIGHT 8
#define LV_X 0
#define LV_Y 24

LevelSprite* lv[8];
UnitAngle8Task* unitAngleTask;
UnitMiniJoyCTask* unitMiniJoyCTask;

void setup()
{
    auto cfg = M5.config();
    M5.begin(cfg);


    Sound::beep();
    // オフスクリーンバッファの初期化

    screen.setup();
    screen.removeAllRenderers();
    screen.setRotation(0);
    screen.enableRotation = false;

    message.setTextSize(2);
    screen.update();

    // lv配列を初期化
    for(int i = 0; i < 8; i++){
        lv[i] = new LevelSprite(&screen, LV_WIDTH, LV_HEIGHT, LV_X, LV_Y + i * 10);
    }

    fps.setTextSize(2);
    mem.setTextSize(2);
        
    unitAngleTask = new UnitAngle8Task();
    unitMiniJoyCTask = new UnitMiniJoyCTask();
    LOG_D("UnitAngle8Task created");
}

void loop()
{
    M5.update();
    screen.start();


    //　ノブの値を表示
    for(int i = 0; i < 8; i++){
        lv[i]->setLevel(unitAngleTask->unit.level[i]);
    }

    // レーダーを表示
    rader.addPosition(unitMiniJoyCTask->unit.x, unitMiniJoyCTask->unit.y);


    char buf[128];
    sprintf(buf,"%.2f,%.2f\n", unitMiniJoyCTask->unit.x, unitMiniJoyCTask->unit.y);
    btSerial.send(buf);
    btSerial.update();

    fps.setText("FPS:" + String(screen.getFPS()));
    mem.setText("MEM:" + String(Device::getFreeDmaSize()));

    screen.update();

   // delay(10);
}

void showMessage(char* msg)
{
    message.setText(msg);
    screen.update();
}
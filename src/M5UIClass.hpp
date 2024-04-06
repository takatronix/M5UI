#pragma once
#include <M5UI.h>
#include "M5UICanvas.hpp"

class M5UIClass
{
public:
    M5UICanvas Screen;
    M5UIClass(M5GFX* pDisplay) : Screen(pDisplay) {}
    bool begin(){
        M5.begin();
        Serial.begin(115200);        
        setup();
        return true;
    }

    bool setup(){
        Screen.setup();
        // 起動音
        //Sound::setVolume(255);
        //Sound::beep(2000);
        //Sound::beep(1000);
        return true;
    }

    bool update(){
        M5.update();

        Tween::updateAll();
        Sprite::updateAll();

        // LCDへ描画
        Screen.update();
        return true;
    }
};


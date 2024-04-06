#ifndef _M5UI_H_
#define _M5UI_H_

#include <M5UI.h>

class M5UIClass
{
public:
    M5UICanvas Screen;
    M5UIClass() : Screen(&M5.Display) {}
    bool begin(){
        M5.begin();
        setup();
        return true;
    }

    bool setup(){
        Screen.setup();

        // 起動音
        Sound::setVolume(255);
        Sound::beep(2000);
        Sound::beep(1000);

        return true;
    }

    bool update(){
        LOG_V("M5UI::update()");
        Tween::updateAll();
        return true;
    }
};

#endif
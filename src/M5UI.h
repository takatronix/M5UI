#ifndef _M5UI_hpp_
#define _M5UI_hpp_

#include <M5Unified.h>


#define LOG_ENABLE_ERROR
#define LOG_ENABLE_WARN
#define LOG_ENABLE_INFO
#define LOG_ENABLE_DEBUG
#define LOG_ENABLE_VERBOSE
#include "Log.hpp"

#include "Device.hpp"
#include "Timer.hpp"
#include "Sound.hpp"
#include "StopWatch.hpp"
#include "Tween.hpp"

#include "Sprite.hpp"
#include "TextSprite.hpp"
#include "ImageSprite.hpp"
#include "M5UICanvas.hpp"

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
        Tween::updateAll();
        return true;
    }
};



#endif
#pragma once
#include <M5Unified.h>

class M5UICanvas : public M5Canvas
{
    int frameCount = 0;
    StopWatch startWatch;
    StopWatch drawWatch;
    int _drawTime = 0;
public:
    bool _enableRotation = false;
    M5UICanvas(M5GFX* pDisplay) : M5Canvas(pDisplay) {
    }
    void update(){
        StopWatch drawWatch;

        // デバイスの向きが変わったら画面の向きも変える
        if(Device::wasOrientationChanged() && _enableRotation) {
            LOG_D("Orientation changed");
            int rotation = Device::getRotation();
            if(rotation != -1){
                M5.Display.setRotation(rotation);

                this->updateAllPosition();
            }
        }

        Tween::updateAll();
        bool shouldRefresh = Sprite::updateAll();
        _drawTime = drawWatch.Elapsed();
        
        pushSprite(0, 0);
        frameCount++;
    }
    void updateAllPosition(){
        for(auto sprite : Sprite::_sprites){
            sprite->updatePosition();
        }
    }

    int getFPS(){
        auto second = startWatch.Second();
        if(second == 0){
            return 0;
        }
        return frameCount / second;
    }
    int getDrawTime(){
        return _drawTime;
    }
    void resetFPS(){
        frameCount = 0;
        startWatch.reset();
    }
    M5UICanvas& add(Sprite* sprite){
        Sprite::add(sprite);
        return *this;
    }

    bool setup(bool canRotate = true)
    {
        LOG_I("setupOffscreen");

        // 回転に対応する場合は、最大サイズを取得
        int width = M5.Display.width();
        int height = M5.Display.height();
        if (canRotate)
        {
            width = max(width, height);
            height = width;
        }

        this->setColorDepth(8);
        if (this->createSprite(width, height) == NULL)
        {
            LOG_E("createSprite error");
            M5.Display.fillScreen(TFT_RED);
            M5.Display.setTextSize(2);
            M5.Display.setTextColor(TFT_WHITE);
            M5.Display.println("Insufficient memory");
            return false;
        }

        this->setFont(&fonts::lgfxJapanGothic_16);
        this->clear();
        this->setTextSize(1);
        this->setTextScroll(true);

        this->setTextColor(CYAN);
        this->setTextColor(GREEN);

        LOG_I("M5UICanvas createSprite OK");
        return true;
    }

    void printDeviceInfo()
    {
        this->setTextColor(TFT_YELLOW);
        this->printf("Board:%s\n", Device::getBoardName());
        this->printf("Screen:%dx%d\n", M5.Display.width(), M5.Display.height());
        this->printf("Battery:%d\n", Device::getBatteryLevel());
        this->printf("Orientation:%s\n", Device::getOrientationName());
        this->printf("FreeMemory:%d\n", Device::getFreeDmaSize());
        this->printf("FreeBlock:%d\n", Device::getLargestFreeBlock());
        this->setTextColor(TFT_GREEN);
    }
    char* printf(const char *format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        this->print(buffer);
        return buffer;
    }
};


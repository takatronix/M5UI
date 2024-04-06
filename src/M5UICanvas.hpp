#ifndef _M5UICanvas_hpp_
#define _M5UICanvas_hpp_
#include <M5Unified.h>


class M5UICanvas : public M5Canvas
{
    int frameCount = 0;
    StopWatch startWatch;
public:
    M5UICanvas(M5GFX* pDisplay) : M5Canvas(pDisplay) {
    }
    void update(){

        // LCDへ描画
        pushSprite(0, 0);
        frameCount++;
    }
    float getFPS(){
        return frameCount / startWatch.Second();
    }
    void resetFPS(){
        frameCount = 0;
        startWatch.reset();
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
        this->fillSprite(TFT_BLUE);
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

#endif
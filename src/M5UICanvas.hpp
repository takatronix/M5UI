#pragma once
#include <M5Unified.h>

// オフスクリーンバッファを使った描画を行うためのクラス
class M5UICanvas : public M5Canvas
{
    int frameCount = 0;
    StopWatch startWatch;
    StopWatch drawWatch;
    int _drawTime = 0;
    std::vector<Renderer *> _renderers;
public:
    bool _enableRotation = false;
    M5UICanvas(M5GFX* pDisplay) : M5Canvas(pDisplay) {
    }
    void start(){
        drawWatch.reset();
    }
    void update(){
/*
        // デバイスの向きが変わったら画面の向きも変える
        if(Device::wasOrientationChanged() && _enableRotation) {
            LOG_D("Orientation changed");
            int rotation = Device::getRotation();
            if(rotation != -1){
                M5.Display.setRotation(rotation);
                Sprite::updateLayout();
            }
        }

*/
        Tween::updateAll();

        for(auto renderer : _renderers){
            renderer->draw(this);
        }

        Sprite::updateAll();
        _drawTime = drawWatch.Elapsed();
        
        // 画面に描画
        pushSprite(0, 0);
        frameCount++;
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
    M5UICanvas& add(Renderer* renderer){
        _renderers.push_back(renderer);
        return *this;
    }
    M5UICanvas& remove(Renderer* renderer){
        for(auto it = _renderers.begin(); it != _renderers.end(); ++it){
            if(*it == renderer){
                _renderers.erase(it);
                break;
            }
        }
        return *this;
    }
    M5UICanvas& removeRenderer(int index){
        if(index >= 0 && index < _renderers.size()){
            _renderers.erase(_renderers.begin() + index);
        }
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

        // rendererのsetupを呼び出す
        for(auto renderer : _renderers){
            renderer->setup();
        }
        // spriteのsetupを呼び出す
        Sprite::setupAll();

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


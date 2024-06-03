#pragma once
#include <M5Unified.h>

// オフスクリーンバッファを使った描画を行うためのクラス
class M5UICanvas : public M5Canvas
{
    int frameCount = 0;
    StopWatch startWatch;
    StopWatch drawWatch;
    int _drawingTime = 0;
    std::vector<Renderer *> _renderers;
    std::function<void(int)> _rotationCallBack;

public:
    // 画面の向きが変わった時に呼び出すコールバックを設定
    void onRotate(std::function<void(int)> callback)
    {
        _rotationCallBack = callback;
    }
    bool enableRotation = true;
    M5UICanvas(M5GFX *pDisplay) : M5Canvas(pDisplay)
    {
        // デフォルトでクリアするレンダラーを追加
        this->add(new ClearRenderer());
    }
    void start()
    {
        drawWatch.reset();

        // デバイスの向きが変わったら画面の向きも変える
        if (Device::wasOrientationChanged() && enableRotation)
        {
            int rotation = Device::getRotation();
            if (rotation != -1)
            {                
                M5.Display.setRotation(rotation);

                setup();
                if (_rotationCallBack != nullptr)
                {
                    _rotationCallBack(rotation);
                }
            }
            Sprite::updateLayout();
        }
    }

    void update()
    {
        // タイマーの更新
        Timer::updateAll();
        // Tweenの更新
        Tween::updateAll();
        // 背景の描画
        for (auto renderer : _renderers)
        {
            renderer->update();
            renderer->draw(this);
        }
        // Spriteの更新
        Sprite::updateAll();

        // 描画時間を計測
        _drawingTime = drawWatch.Elapsed();

        // 画面に描画
        pushSprite(0, 0);
        frameCount++;
    }

    int getFPS()
    {
        auto second = startWatch.Second();
        if (second == 0)
        {
            return 0;
        }
        return frameCount / second;
    }
    int getDrawingTime()
    {
        return _drawingTime;
    }
    void resetFPS()
    {
        frameCount = 0;
        startWatch.reset();
    }
    M5UICanvas &add(Sprite *sprite)
    {
        Sprite::add(sprite);
        return *this;
    }
    M5UICanvas &add(Renderer *renderer)
    {
        _renderers.push_back(renderer);
        return *this;
    }
    M5UICanvas &remove(Renderer *renderer)
    {
        for (auto it = _renderers.begin(); it != _renderers.end(); ++it)
        {
            if (*it == renderer)
            {
                _renderers.erase(it);
                break;
            }
        }
        return *this;
    }
    M5UICanvas &removeRenderer(int index)
    {
        if (index >= 0 && index < _renderers.size())
        {
            _renderers.erase(_renderers.begin() + index);
        }
        return *this;
    }
    M5UICanvas &removeAllRenderers()
    {
        _renderers.clear();
        return *this;
    }

    bool setup()
    {
        int width = M5.Display.width();
        int height = M5.Display.height();
        LOG_I("setupOffscreen width:%d height:%d", width, height);

        if (this->width() == width && this->height() == height)
        {
            LOG_I("Offscreen is already created(width:%d height:%d)", width, height);
            return true;
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

        // spriteをsetup
        Sprite::setupAll();
        Sprite::updateLayout();

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
    char *printf(const char *format, ...)
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

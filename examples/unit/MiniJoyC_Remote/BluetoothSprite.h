// BluetoothSprite.hpp
#pragma once
#include "Sprite.hpp"
#include <vector>

class BluetoothSprite : public Sprite
{
public:
    BluetoothSprite(M5Canvas *pDisplay, int width = 16, int height = 16, LayoutType layout = LayoutType::ScreenTopRight, int depth = M5UI_COLOR_DEPTH, bool psram = false) : Sprite(pDisplay, width, height, depth, psram)
    {
        if (create(width, height, 0, 0, depth, psram) == NULL)
        {
            LOG_E("Text Sprite create error");
        }
        Sprite::add(this);

        enableAffine = true;
        enableTransparent = true;
    
    }

    int margin = 2;

    bool update(void) override
    {
        // データ取得処理などを記述
        return false;
    }

    void draw(void) override
    {
        // データ描画処理などを記述
        canvas.clear(TFT_BLUE);
    }


};

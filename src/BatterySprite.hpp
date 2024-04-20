#pragma once
#include "Sprite.hpp"

class BatterySprite : public Sprite
{
public:
    float Level = 0.0;
    StopWatch sw;
    BatterySprite(M5Canvas *pDisplay, int width = 32, int height = 16, int depth = M5UI_COLOR_DEPTH, bool psram = false) : Sprite(pDisplay, width, height, depth, psram)
    {
        if (create(width, height, 0, 0, depth, psram) == NULL)
        {
            LOG_E("Text Sprite create error");
        }
        Sprite::add(this);

        enableAffine = true;
        enableTransparent = true;
    }
    
    bool update(void) override
    {
        // 1秒ごとにバッテリーレベルを取得
        if (sw.Elapsed() < 1000)
            return false;

        sw.reset();
        float level = Device::getBatteryLevel() / 100.0F;
        if (level != Level)
        {
            Level = level;
            return true;
        }

        // falseを返すと描画処理がスキップされる
        return false;
    }

    void draw(void) override
    {
        canvas.clear();
        drawBatteryIcon(&canvas, 0, 0, _width, _height, Level);
    }

    // バッテリーアイコンを描画する関数
    void drawBatteryIcon(M5Canvas *canvas, int x, int y, int width, int height, float level, uint16_t frameColor = TFT_WHITE)
    {
        int tipWidth = width * 0.05;      // 突起部の幅は全体の5%
        int tipHeight = height * 0.5;     // 突起部の高さは全体の50%
        int bodyWidth = width - tipWidth; // 本体の幅

        // バッテリー本体の枠を描画
        canvas->drawRect(x, y, bodyWidth, height, frameColor);

        // バッテリー突起部の描画（中央に配置）
        canvas->fillRect(x + bodyWidth, y + (height - tipHeight) / 2, tipWidth, tipHeight, frameColor);

        uint16_t batteryColor = TFT_GREEN; // バッテリーの色
        if (level < 0.2)
        {
            batteryColor = TFT_RED; // バッテリー残量が20%未満の場合は赤色
        }
        else if (level < 0.5)
        {
            batteryColor = TFT_ORANGE; // バッテリー残量が50%未満の場合は黄色
        }

        // バッテリーレベルの描画
        int padding = 2;                                    // 枠の内側のパディング
        int innerWidth = (bodyWidth - 2 * padding) * level; // レベルに応じた内部の幅を計算
        if (innerWidth > 0)
        {
            canvas->fillRect(x + padding, y + padding, innerWidth, height - 2 * padding, batteryColor);
        }
    }
};

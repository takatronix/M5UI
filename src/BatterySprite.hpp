#pragma once
#include "Sprite.hpp"

class BatterySprite : public Sprite
{
public:
    float Level = 0.0;
    bool isCharning = false;
    StopWatch sw;
    BatterySprite(M5Canvas *pDisplay, int width = 32, int height = 16,LayoutType layout = LayoutType::ScreenTopRight, int depth = M5UI_COLOR_DEPTH, bool psram = false) : Sprite(pDisplay, width, height, depth, psram)
    {
        if (create(width, height, 0, 0, depth, psram) == NULL)
        {
            LOG_E("Text Sprite create error");
        }
        Sprite::add(this);

        enableAffine = true;
        enableTransparent = true;
        setLayout(layout);
    }
    
    bool update(void) override
    {
        // 1秒ごとにバッテリーレベルを取得
        if (sw.Elapsed() < 1000)
            return false;

        sw.reset();

        bool shouldUpdate = false;

        if(Device::isStack()){
            bool charning = M5.Power.isCharging();
            if (charning != isCharning)
            {
                isCharning = charning;
                shouldUpdate = true;
            }

        }

        float level = Device::getBatteryLevel() / 100.0F;
        if (level != Level)
        {
            Level = level;
            shouldUpdate = true;
        }
        // falseを返すと描画処理がスキップされる
        return shouldUpdate;
    }

    void draw(void) override
    {
        canvas.clear();
        if(Device::hasBattery())
           drawBatteryIcon(&canvas, 0, 0, _width, _height, Level,isCharning);
    }

    // バッテリーアイコンを描画する関数
    void drawBatteryIcon(M5Canvas *canvas, int x, int y, int width, int height, float level,bool isCharging = false,uint16_t frameColor = TFT_WHITE)
    {
        int tipWidth = width * 0.05;      // 突起部の幅は全体の5%
        int tipHeight = height * 0.5;     // 突起部の高さは全体の50%
        int bodyWidth = width - tipWidth; // 本体の幅


        // いったん背景を塗りつぶす
        uint16_t background = M5.Display.color565(10, 0, 0);
        canvas->fillRect(x, y, width, height, background);

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
        int innerHeight = height - 2 * padding;             // レベルの高さ
        if (innerWidth > 0)
        {
            canvas->fillRect(x + padding, y + padding, innerWidth, height - 2 * padding, batteryColor);
        }

        if(isCharging)
        {
    
            int boltWidth = width / 7;  // 雷マークの最大幅
            int boltHeight = height * 0.6; // 雷マークの高さ

            // 雷マークの上部の三角形
            canvas->fillTriangle(
                x + width / 2, y +2,
                x + width / 2, y  + boltHeight+2,
                x + width / 2 - boltWidth, y + boltHeight,
                WHITE);

            canvas->fillTriangle(
                x + width / 2, y +height,
                x + width / 2, y +height - boltHeight,
                x + width / 2 + boltWidth, y + height - boltHeight,
                WHITE);
        }
    }
            
    
};

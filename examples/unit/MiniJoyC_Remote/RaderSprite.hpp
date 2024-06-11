// RaderSprite.hpp
#pragma once
#include "Sprite.hpp"
#include <vector>

class RaderSprite : public Sprite
{
public:
    uint16_t color = RED;
    int radius = 4;
    uint16_t backgroundColor = TFT_DARKGREEN;

    bool invertY = true;
    RaderSprite(M5Canvas *pDisplay, int width = 124, int height = 124, LayoutType layout = LayoutType::ScreenTopRight, int depth = M5UI_COLOR_DEPTH, bool psram = false) : Sprite(pDisplay, width, height, depth, psram)
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
    
    int margin = 2;

    bool update(void) override
    {
        // データ取得処理などを記述
        return false;
    }

    void draw(void) override
    {
        canvas.clear(backgroundColor);
        
        // レーダーの円を描画
        int centerX = width() / 2;
        int centerY = height() / 2;
        int maxRadius = width() / 2 - margin;

        // 中心の点
        canvas.fillCircle(centerX, centerY, 2, TFT_WHITE);

        // 中間の円と最大の円
        canvas.drawCircle(centerX, centerY, maxRadius / 2, TFT_WHITE);
        canvas.drawCircle(centerX, centerY, maxRadius , TFT_WHITE);
        canvas.setTextSize(1);

        // 過去の位置を順に描画
        for (size_t i = 0; i < positions.size(); ++i)
        {
            int x = positions[i].first;
            int y = positions[i].second;
            if(invertY){
                y = height() - y;
            }
            canvas.setCursor(x, y);
            canvas.fillCircle(x, y, radius, color);            
        }
    }

    void addPosition(float x, float y,char* text = NULL)
    {
        int centerX = width() / 2;
        int centerY = height() / 2;
        int maxRadius = width() / 2 - margin;

        // x, y を画面の座標に変換(マージンを考慮)
        int posX = centerX + x * maxRadius;
        int posY = centerY + y * maxRadius;
        
        // 新しい位置を追加
        positions.push_back(std::make_pair(posX, posY));

        // 古いデータを削除（彗星のしっぽの長さを制限）
        if (positions.size() > 10) // 長さを調整
        {
            positions.erase(positions.begin());
        }
        if(text != NULL){
            canvas.drawString(text, posX, posY);
        }

        // 再描画
        _shouldRedraw = true;
    }

private:
    std::vector<std::pair<int, int>> positions;
};

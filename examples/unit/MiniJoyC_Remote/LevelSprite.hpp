// LevelSprite.hpp
#pragma once
#include "Sprite.hpp"

class LevelSprite : public Sprite
{
public:
    LevelSprite(M5Canvas *pDisplay, int width = 128, int height = 20, LayoutType layout = LayoutType::ScreenTopLeft, int depth = M5UI_COLOR_DEPTH, bool psram = false) : Sprite(pDisplay, width, height, depth, psram)
    {
        if (create(width, height, 0, 0, depth, psram) == NULL)
        {
            LOG_E("Level Sprite create error");
        }
        Sprite::add(this);

        enableAffine = false;
        enableTransparent = false;
        setLayout(layout);
    }

    void setLevel(float level)
    {
        // レベルを0から1の範囲に収める
        _level = constrain(level, 0.0f, 1.0f);
        _shouldRedraw = true;
    }

    void setColor(uint16_t color)
    {
        _color = color;
        _shouldRedraw = true;
    }

    void setMargin(int margin)
    {
        _margin = margin;
        _shouldRedraw = true;
    }

    void setVerticalMode(bool vertical)
    {
        _vertical = vertical;
        _shouldRedraw = true;
    }

    void setInvertedMode(bool inverted)
    {
        _inverted = inverted;
        _shouldRedraw = true;
    }

    void setText(const char* text, bool centered = false)
    {
        _text = text;
        _centered = centered;
        _shouldRedraw = true;
    }

    void draw(void) override
    {
        canvas.clear(RED);

        // レベルの描画
        int pos = 0;
        int length = 0;

        float displayLevel = _inverted ? 1.0f - _level : _level;

        if (_vertical)
        {
            length = displayLevel * (height() - _margin * 2);
            pos = height() - length - _margin;
            canvas.fillRect(_margin, pos, width() - _margin * 2, length, _color);
        }
        else
        {
            length = displayLevel * (width() - _margin * 2);
            pos = _margin;
            canvas.fillRect(pos, _margin, length, height() - _margin * 2, _color);
        }

        // テキストの描画
        if (_text != nullptr)
        {
            if (_centered)
            {
                /*
                int16_t x1, y1;
                uint16_t w, h;
                canvas.getTextBounds(_text, 0, 0, &x1, &y1, &w, &h);
                int textX = (width() - w) / 2;
                int textY = (height() - h) / 2;
                canvas.setCursor(textX, textY);
                */
            }
            else
            {
                canvas.setCursor(_margin, _margin);
            }
            canvas.setTextColor(_textColor);
            canvas.print(_text);
        }
    }

private:
    float _level = 0.0f;   // レベル（0.0 ~ 1.0）
    uint16_t _color = GREEN; // レベル0の時の色
    uint16_t _textColor = WHITE; // テキストの色
    int _margin = 0; // マージン（ピクセル数）
    bool _vertical = false; // 縦モードかどうか
    bool _inverted = false; // 反転モードかどうか
    const char* _text = nullptr; // 表示するテキスト
    bool _centered = false; // テキストをセンタリングするかどうか
};

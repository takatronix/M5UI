#pragma once
#include <M5UI.h>

class Renderer
{
public:
    virtual bool setup(){};
    virtual void draw(M5Canvas* canvas){};
    virtual void update(){};
};

class ClearRenderer : public Renderer
{
    uint16_t _color;
public:
    ClearRenderer(uint16_t color = BLACK)
    {
        _color = color;
    }
    void draw(M5Canvas* canvas) override
    {
        canvas->clear(_color);
    }
};

class ImageRenderer : public Renderer
{
    uint8_t* _image;
    int _width;
    int _height;
    int _x;
    int _y;
public:
    ImageRenderer(uint8_t* image, int width, int height, int x = 0, int y = 0)
    {
        _image = image;
        _width = width;
        _height = height;
        _x = x;
        _y = y;
    }
    void draw(M5Canvas* canvas) override
    {
        canvas->pushImage(_x, _y, _width, _height, _image);
    }
};


class TextRenderer : public Renderer
{
    char* _text;
    int _x;
    int _y;
    uint16_t _color;
    uint16_t _bgColor;
    int _size;
public:
    TextRenderer(char* text, int x, int y, uint16_t color = WHITE, uint16_t bgColor = BLACK, int size = 1)
    {
        _text = text;
        _x = x;
        _y = y;
        _color = color;
        _bgColor = bgColor;
        _size = size;
    }
    void draw(M5Canvas* canvas) override
    {
        canvas->setTextSize(_size);
        canvas->setTextColor(_color, _bgColor);
        canvas->drawString(_text, _x, _y);
    }
};


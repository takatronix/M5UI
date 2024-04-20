#pragma once
#include "Sprite.hpp"

class ImageSprite : public Sprite
{
    uint8_t *pImage = NULL;
    int _imageWidth = 0;
    int _imageHeight = 0;

public:
    uint8_t *getImage() { return pImage; }
    void setImage(uint8_t *data, int width, int height)
    {
        pImage = data;
        _imageWidth = width;
        _imageHeight = height;
    }
    int getImageWidth() { return _imageWidth; }
    int getImageHeight() { return _imageHeight; }

    ImageSprite(M5Canvas *pDisplay) : Sprite(pDisplay)
    {
    }
    ImageSprite(M5Canvas *pDisplay, const uint8_t *data, int width, int height, int x = 0, int y = 0, bool enableTransparent = false, uint16_t transparentColor = TFT_BLACK) : Sprite(pDisplay, width, height, x, y)
    {
        this->pImage = (uint8_t *)data;
        this->_imageWidth = width;
        this->_imageHeight = height;
        this->enableTransparent = enableTransparent;
        this->_transparentColor = transparentColor;
        this->_width = width;
        this->_height = height;
        Sprite::add(this);
    }
    // 画像をpushするのでdrawは何もする必要なし
    void draw(void) override {}

    void push(void) override
    {
        pushImage(parentCanvas, this->pImage);
    }
};

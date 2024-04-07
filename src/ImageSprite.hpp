#pragma once
#include "Sprite.hpp"

class ImageSprite : public Sprite{
    uint8_t *pImage = NULL;
    int _imageWidth = 0;
    int _imageHeight = 0;
public:
    uint8_t* getImage(){return pImage;}
    void setImage(uint8_t* data,int width,int height){
        pImage = data;
        _imageWidth = width;
        _imageHeight = height;
    }
    int getImageWidth(){return _imageWidth;}    
    int getImageHeight(){return _imageHeight;}

    ImageSprite(M5Canvas* pDisplay) : Sprite(pDisplay) {
    }
    ImageSprite(M5Canvas* pDisplay,const uint8_t* data,int width,int height,int x=0,int y=0,bool enableTransparent = true,uint16_t transparentColor = TFT_BLACK) : Sprite(pDisplay,width,height,x,y) {
        this->pImage = (uint8_t*)data;
        this->_imageWidth = width;
        this->_imageHeight = height;
        this->enableTransparent = convertColor(enableTransparent);
        this->_transparentColor = transparentColor;
        Sprite::add(this);
    }
    // 画像をpushするのでdrawは何もする必要なし(Debug情報のみ)
    void draw(){
    }
    virtual bool push(void){
        return push(parentCanvas);
    }
    bool pushImage(int x,int y){
        parentCanvas->pushImage(x,y,_imageWidth,_imageHeight,pImage);
    }
    bool pushImage(M5Canvas *pCanvas,int x,int y){
        pCanvas->pushImage(x,y,_imageWidth,_imageHeight,pImage);
    }
    bool push(M5Canvas *pCanvas){
        if(pImage == NULL){
            return false;
        }
        LOG_V("ImageSprite::push() x:%d y:%d width:%d height:%d",_x,_y,_width,_height);
        if(enableAffine){

            if(enableAA){
                if(enableTransparent)
                    pCanvas->pushImageAffineWithAA(_matrix,_width,_height,pImage,_transparentColor);
                else
                    pCanvas->pushImageAffineWithAA(_matrix,_width,_height,pImage);
            }
            else{
                if(enableTransparent)
                    pCanvas->pushImageAffine(_matrix,_width,_height,pImage,_transparentColor);
                else
                    pCanvas->pushImageAffine(_matrix,_width,_height,pImage);
            }
            return true;
        }

        if(enableTransparent){
            pCanvas->pushImage(_x,_y,_width,_height,pImage,_transparentColor);
        }
        else{
            pCanvas->pushImage(_x,_y,_width,_height,pImage);
        }
        return true;

    }

};

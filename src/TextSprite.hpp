#pragma once
#include "Sprite.hpp"

class TextSprite : public Sprite{
public:
    String text;

    TextSprite(M5Canvas* pDisplay,int width=0,int height=0,int x=0,int y=0,int depth = M5UI_COLOR_DEPTH,bool psram = false) : Sprite(pDisplay,width,height,x,y,depth,psram) {
        if (create(width, height, x, y, depth, psram) == NULL)
        {
            LOG_E("Text Sprite create error");
        }
        Sprite::add(this);
    }


    void setText(const char* text){
        // テキストに変更があれば描画
        if(this->text == text){
            return;
        }

        this->text = text;
        this->redraw();

        // サイズが変わっていたら再作成
        int width = getTextWidth();
        int height = getTextHeight();
        if(_width == width && _height == height){
            return;
        }

        if (create(width, height, _x, _y, _depth, _psram) == NULL)
        {
            LOG_E("Text Sprite create error");
        }
    }
    void setText(String text){
        setText(text.c_str());
    }
    int getTextWidth(){
        return canvas.textWidth(text.c_str());
    }
    int getTextHeight(){
        return canvas.fontHeight(canvas.getFont());
    }


    virtual void onDraw(){
        canvas.clear();
        canvas.drawString(text.c_str(),0,0);
    }   
};

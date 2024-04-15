#pragma once
#include "Sprite.hpp"

class TextSprite : public Sprite{
public:
    String text;
    TextSprite(M5Canvas* pDisplay) : Sprite(pDisplay) {
    }
    TextSprite(M5Canvas* pDisplay,int width,int height,int x=0,int y=0,int depth = M5UI_COLOR_DEPTH,bool psram = false) : Sprite(pDisplay,width,height,x,y,depth,psram) {
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
    }
    void setText(String text){
        setText(text.c_str());
    }

    void draw(){
        canvas.clear();
        canvas.drawString(text.c_str(),0,0);
    }   
};

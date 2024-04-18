#pragma once
#include "Sprite.hpp"

class ConsoleSprite : public Sprite{
public:
    String text;

    ConsoleSprite(M5Canvas* pDisplay,int width=0,int height=0,int x=0,int y=0,int depth = 8,bool psram = false) : Sprite(pDisplay,width,height,x,y,depth,psram) {
        if(width == 0){
            width = pDisplay->width();
        }
        if(height == 0){
            height = pDisplay->height();
        }
        
        int size = max(width,height);

        if (create(size, size, x, y, depth, psram) == NULL)
        {
            LOG_E("Text Sprite create error");
        }
        Sprite::add(this);
    }


    virtual void onDraw(void){
    }   
};

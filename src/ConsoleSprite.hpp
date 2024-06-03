#pragma once
#include "Sprite.hpp"

class ConsoleSprite : public Sprite{
public:
    String text;
    ConsoleSprite(M5Canvas* pDisplay,LayoutType layout = LayoutType::ScreenBottomLeft,int width=0,int height=0,int x=0,int y=0,int depth = 8,bool psram = false) : Sprite(pDisplay,width,height,x,y,depth,psram) {
        Sprite::add(this);
    }
    bool setup() override
    {
        float size = max(M5.Display.width(),M5.Display.height()) / 2;
        if (create(size, size) == NULL)
        {
            LOG_E("Console Sprite Create Failed");
            return false;
        }
        setScale(2.0);
        setTextSize(1);
        enableTransparent = true;
        canvas.setCursor(0,0);
        LOG_I("Console Sprite Created %d %d",width(),height());
        return true;
    }
    
};

#ifndef _TextSprite_hpp_
#define _TextSprite_hpp_

#include "Sprite.hpp"

class TextSprite : public Sprite{
public:
    int textSize = 1;
    String text;
    TextSprite(M5Canvas* pDisplay) : Sprite(pDisplay) {

    }
    TextSprite(M5Canvas* pDisplay,String tag,int width,int height,int x=0,int y=0,int depth = 16,bool psram = false) : Sprite(pDisplay,tag,width,height,x,y,depth,psram) {
    }

    void setTextSize(int size){
        textSize = size;
    }
    void setText(const char* text){
        this->text = text;
    }
    void setText(String text){
        this->text = text;
    }
    void update(String text)
    {
        this->text = text;
        canvas.setTextSize(textSize);
        canvas.drawString(text.c_str(),0,0);
    }
};

#endif


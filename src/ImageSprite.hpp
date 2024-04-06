#ifndef _ImageSprite_hpp_
#define _ImageSprite_hpp_
#include "Sprite.hpp"

class ImageSprite : public Sprite{
public:

    ImageSprite(M5Canvas* pDisplay) : Sprite(pDisplay) {
    }
    ImageSprite(M5Canvas* pDisplay,String tag,const unsigned short *data,int width,int height,int x=0,int y=0,int depth = 16,bool psram = false) : Sprite(pDisplay,tag,width,height,x,y,depth,psram) {
       // this->data = data;
        if (create(width, height, x, y, depth, psram) == NULL)
        {
            LOG_E("Sprite create error");
        }
    }

    ImageSprite(M5Canvas* pDisplay,String tag,const unsigned char *data,int width,int height,int x=0,int y=0,int depth = 16,bool psram = false) : Sprite(pDisplay,tag,width,height,x,y,depth,psram) {
        if (create(width, height, x, y, depth, psram) == NULL)
        {
            LOG_E("Sprite create error");
        }
    }

};

#endif

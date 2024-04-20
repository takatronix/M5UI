#include "M5UI.h"

class StarFieldRenderer : public Renderer
{
public:
    int numStars = 100;
    struct Star
    {
        float angle;
        float radius;
        float speed;
        uint32_t color; // 星の色
    };
    Star* stars = NULL;
    StarFieldRenderer(int numStars = 100)
    {
        this->numStars = numStars;
        stars = new Star[numStars];
    }
    ~StarFieldRenderer()
    {
        delete[] stars;
    }
    /*
    struct Star
    {
        float angle;
        float radius;
        float speed;
        uint32_t color; // 星の色
    };

    int numStars = 100;
    Star stars[numStars];
    int centerX = 160;                                             // 画面中心のX座標
    int centerY = 120;                                             // 画面中心のY座標
    float maxRadius = sqrt(centerX * centerX + centerY * centerY); // 画面の対角線の長さの半分


    bool setup() override
    {
        for (int i = 0; i < numStars; ++i)
        {
            stars[i].angle = random(0, 360) * PI / 180.0;
            stars[i].radius = 0;
            // より大きな速度範囲を持たせる
            stars[i].speed = random(5, 50) / 10.0; // 0.5から5.0の間でランダムに設定
            stars[i].color = WHITE;                // 星の色を白に固定
        }
        return false;
    }
*/
    void draw(M5Canvas* canvas) override
    {
        float centerX = canvas->width() / 2;
        float centerY = canvas->height() / 2;
        float maxRadius = sqrt(centerX * centerX + centerY * centerY); // 画面の対角線の長さの半分
        
        for (int i = 0; i < numStars; ++i)
        {
            stars[i].radius += stars[i].speed;
            uint8_t brightness = map(stars[i].radius, 0, maxRadius, 0, 255);               // 輝度を調整
            uint32_t fadedColor = M5.Display.color888(brightness, brightness, brightness); // 輝度に基づいた色
            int x = centerX + stars[i].radius * cos(stars[i].angle);
            int y = centerY + stars[i].radius * sin(stars[i].angle);

            if (x < 0 || x >= M5.Display.width() || y < 0 || y >= M5.Display.height() || stars[i].radius > maxRadius)
            {
                stars[i].angle = random(0, 360) * PI / 180.0;
                stars[i].radius = 0;
                stars[i].speed = random(5, 50) / 10.0; // 速度を再設定
                stars[i].color = WHITE;
            }
            canvas->drawPixel(x, y, fadedColor);
        }
    }
};
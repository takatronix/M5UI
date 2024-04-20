#include "M5UI.h"
#include <vector>

struct Ripple {
    float x, y;
    float currentRadius = 0;
    float maxRadius;
    uint32_t color;
    float speed;

    Ripple(float x, float y, float maxRadius, uint32_t color, float speed)
        : x(x), y(y), maxRadius(maxRadius), color(color), speed(speed) {}
};

class RippleRenderer : public Renderer {
public:
    std::vector<Ripple> ripples;
    int maxRipples = 5; // 同時に表示する最大波紋数

    void addRipple(float x, float y, float maxRadius, uint32_t color, float speed) {
        if (ripples.size() < maxRipples) {
            ripples.emplace_back(x, y, maxRadius, color, speed);
        }
    }

    void draw(M5Canvas* canvas) override {
        for (auto it = ripples.begin(); it != ripples.end(); ) {
            it->currentRadius += it->speed;
            if (it->currentRadius > it->maxRadius) {
                it = ripples.erase(it); // 波紋が最大半径を超えたら削除
            } else {
                uint8_t alpha = 255 - (uint8_t)(255.0 * (it->currentRadius / it->maxRadius));
                uint32_t fadedColor = M5.Display.color565(alpha, alpha, alpha);
                canvas->drawCircle(it->x, it->y, (int)it->currentRadius, fadedColor);
                ++it;
            }
        }

        if (random(0, 100) > 95) { // 確率で新しい波紋を追加
            float newX = random(0, canvas->width());
            float newY = random(0, canvas->height());
            addRipple(newX, newY, random(50, 150), WHITE, random(1, 3));
        }
    }
};

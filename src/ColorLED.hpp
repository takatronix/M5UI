#pragma once
#include <M5Unified.h>

class ColorLED
{
public:
  struct color_t
  {
    uint8_t R;
    uint8_t G;
    uint8_t B;
  } __attribute__((packed));

  gpio_num_t _gpio;
  uint16_t _ledNum;
  uint8_t _maxBrightness;
  uint8_t _brightness;
  color_t *_led;

  ColorLED(gpio_num_t gpio, uint16_t ledNum, uint8_t maxBrightness = 40)
  {
    _gpio = gpio;
    _ledNum = ledNum;
    _maxBrightness = maxBrightness;
    _brightness = 100;

    uint32_t buffSize = _ledNum * 3;
    _led = (color_t *)calloc(1, buffSize);
    if (_led == NULL)
    {
      ESP_LOGE("ColorLED", "calloc failed");
      return;
    }
  };

  // 0-100
  void setBrightness(uint8_t brightness)
  {
    _brightness = brightness;
  }

  uint8_t getBrightness()
  {
    return _brightness;
  }

  uint16_t getLedNum()
  {
    return _ledNum;
  }

  void setColor(uint16_t num, uint8_t r, uint8_t g, uint8_t b)
  {
    _led[num].R = r;
    _led[num].G = g;
    _led[num].B = b;
  };

  void setColor(uint16_t num, color_t color)
  {
    _led[num] = color;
  };

  void show()
  {
    for (int i = 0; i < _ledNum; i++)
    {
      neopixelWrite(_gpio,
                    _led[i].R * _brightness * _maxBrightness / 100 / 255,
                    _led[i].G * _brightness * _maxBrightness / 100 / 255,
                    _led[i].B * _brightness * _maxBrightness / 100 / 255);
      delayMicroseconds(100);
    }
    delay(1);
  };

  void showColor(uint8_t r, uint8_t g, uint8_t b)
  {
    for (int i = 0; i < _ledNum; i++)
    {
      _led[i].R = r;
      _led[i].G = g;
      _led[i].B = b;
    }
    show();
  };

  void showColor(color_t color)
  {
    for (int i = 0; i < _ledNum; i++)
    {
      _led[i] = color;
    }
    show();
  };

  void clear()
  {
    for (int i = 0; i < _ledNum; i++)
    {
      _led[i] = {0, 0, 0};
    }
    show();
  };
};

namespace ColorLEDColor
{
  const ColorLED::color_t BLACK = {0, 0, 0};
  const ColorLED::color_t NAVY = {0, 0, 128};
  const ColorLED::color_t DARKGREEN = {0, 128, 0};
  const ColorLED::color_t DARKCYAN = {0, 128, 128};
  const ColorLED::color_t MAROON = {128, 0, 0};
  const ColorLED::color_t PURPLE = {128, 0, 128};
  const ColorLED::color_t OLIVE = {128, 128, 0};
  const ColorLED::color_t LIGHTGREY = {211, 211, 211};
  const ColorLED::color_t LIGHTGRAY = {211, 211, 211};
  const ColorLED::color_t DARKGREY = {128, 128, 128};
  const ColorLED::color_t DARKGRAY = {128, 128, 128};
  const ColorLED::color_t BLUE = {0, 0, 255};
  const ColorLED::color_t GREEN = {0, 255, 0};
  const ColorLED::color_t CYAN = {0, 255, 255};
  const ColorLED::color_t RED = {255, 0, 0};
  const ColorLED::color_t MAGENTA = {255, 0, 255};
  const ColorLED::color_t YELLOW = {255, 255, 0};
  const ColorLED::color_t WHITE = {255, 255, 255};
  const ColorLED::color_t ORANGE = {255, 180, 0};
  const ColorLED::color_t GREENYELLOW = {180, 255, 0};
  const ColorLED::color_t PINK = {255, 192, 203};
  const ColorLED::color_t BROWN = {150, 75, 0};
  const ColorLED::color_t GOLD = {255, 215, 0};
  const ColorLED::color_t SILVER = {192, 192, 192};
  const ColorLED::color_t SKYBLUE = {135, 206, 235};
  const ColorLED::color_t VIOLET = {180, 46, 226};
};

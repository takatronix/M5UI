#pragma once
#include "I2CWire.h"

// https://docs.m5stack.com/en/hat/MiniJoyC
#define UNIT_JOYC_ADDR 0x54
#define UNIT_JOYC_ADC_VALUE_REG 0x00
#define UNIT_JOYC_POS_VALUE_REG_10_BIT 0x10
#define UNIT_JOYC_POS_VALUE_REG_8_BIT 0x20
#define UNIT_JOYC_BUTTON_REG 0x30
#define UNIT_JOYC_RGB_LED_REG 0x40
#define UNIT_JOYC_CAL_REG 0x50
#define UNIT_JOYC_FIRMWARE_VERSION_REG 0xFE
#define UNIT_JOYC_2C_ADDRESS_REG 0xFF

#define UNIT_JOYC_CAL_MODE_STOP 0
#define UNIT_JOYC_CAL_MODE_AUTO 1
#define UNIT_JOYC_CAL_MODE_MANUAL 2

#define UNIT_JOYC_CALIBRATION_TIMES 100

class UnitMiniJoyC : public I2CWire
{
    uint8_t buttonStatus;
    float ax, ay;
    float x, y;
    bool buttonPressed;
    bool _wasPressed; // ボタンが押されたか


    float ad_min_x = 99999;
    float ad_min_y = 99999;
    float ad_max_x = 0;
    float ad_max_y = 0;
public:
    bool begin(uint8_t addr = UNIT_JOYC_ADDR)
    {
        return I2CWire::begin(addr);
    }

    bool wasPressed()
    {
        bool ret = _wasPressed;
        _wasPressed = false;
        return ret;
    }

    void update()
    {
        uint8_t stat = getButtonStatus();
        buttonPressed = !stat;
        if (stat != buttonStatus)
        {
            if (stat == 0)
            {
                _wasPressed = true;
                LOG_D("Button Pressed");
            }

            buttonStatus = stat;
        }
        // -1 - 1
        ax =  getADCValue(0);
        ay =  getADCValue(1);

        // minとmaxを取得
        if(ax < ad_min_x){
            ad_min_x = ax;
        }
        if(ax > ad_max_x){
            ad_max_x = ax;
        }
        if(ay < ad_min_y){
            ad_min_y = ay;
        }
        if(ay > ad_max_y){
            ad_max_y = ay;
        }
      //  LOG_D("ad_min_x:%f, ad_max_x:%f, ad_min_y:%f, ad_max_y:%f", ad_min_x, ad_max_x, ad_min_y, ad_max_y);

        // 最大値と最小値から中央値を算出
        float mid_x = (ad_max_x + ad_min_x) / 2;
        float mid_y = (ad_max_y + ad_min_y) / 2;
  //      LOG_D("mid_x:%f, mid_y:%f", mid_x, mid_y);
        // 仮想的な最大=1.0、最小=-1.0の範囲に変換
        x = (ax - mid_x) / (ad_max_x - mid_x);
        y = (ay - mid_y) / (ad_max_y - mid_y);

        //float fx =abs(x);
        //float fy =abs(y);
        //setLEDColor(fx * 100,fy * 100,0);
       // LOG_D("x:%f, y:%f", x, y);
    }

    uint16_t getADCValue(uint8_t index)
    {
        uint8_t data[4];
        if (index > 2)
            return 0;
        uint8_t reg = index * 2 + UNIT_JOYC_ADC_VALUE_REG;
        readBytes(reg, data, 2);
        uint32_t value = data[0] | (data[1] << 8);
        return value;
    }

    // -512 - 511
    uint16_t getPosValue10Bit(uint8_t index)
    {
        uint8_t data[4];
        if (index > 2)
            return 0;
        uint8_t reg = index * 2 + UNIT_JOYC_POS_VALUE_REG_10_BIT;
        readBytes(reg, data, 2);
        uint32_t value = data[0] | (data[1] << 8);
        return value;
    }
    // -128 - 127
    uint8_t getPosValue8Bit(uint8_t index)
    {
        uint8_t data;
        if (index > 2)
            return 0;
        uint8_t reg = index + UNIT_JOYC_POS_VALUE_REG_8_BIT;
        readBytes(reg, &data, 1);
        return data;
    }

    bool getButtonStatus()
    {
        uint8_t data;
        readBytes(UNIT_JOYC_BUTTON_REG, &data, 1);
        return data;
    }
    void setLEDColor(uint8_t r, uint8_t g, uint8_t b)
    {
        uint8_t data[4];
        data[2] = r;
        data[1] = g;
        data[0] = b;
        writeBytes(UNIT_JOYC_RGB_LED_REG, data, 3);
    }
    void setLEDColor(uint32_t color)
    {
        uint8_t data[4];
        data[2] = color & 0xff;
        data[1] = (color >> 8) & 0xff;
        data[0] = (color >> 16) & 0xff;
        writeBytes(UNIT_JOYC_RGB_LED_REG, data, 3);
    }

    void setOneCalValue(uint8_t index, uint16_t data)
    {
        uint8_t reg;
        uint8_t buf[4];

        reg = index * 2 + UNIT_JOYC_CAL_REG;
        buf[0] = data & 0xff;
        buf[1] = (data >> 8) & 0xff;
        writeBytes(reg, buf, 2);
        delay(1000);
    }

    void setAllCalValue(uint16_t *data)
    {
        writeBytes(UNIT_JOYC_CAL_REG, (uint8_t *)data, 12);
        delay(1000);
    }

    uint16_t getCalValue(uint8_t index)
    {
        if (index > 5)
            return 0;
        uint8_t data[4];
        uint8_t reg = index * 2 + UNIT_JOYC_CAL_REG;
        readBytes(reg, data, 2);
        uint16_t value = data[0] | (data[1] << 8);
        return value;
    }

    void setCalMode(uint8_t mode)
    {
        writeByte(UNIT_JOYC_CAL_REG, mode);
    }

    uint8_t getCalMode()
    {
        return readByte(UNIT_JOYC_CAL_REG);
    }

    uint8_t getFirmwareVersion()
    {
        return readByte(UNIT_JOYC_FIRMWARE_VERSION_REG);
    }
};
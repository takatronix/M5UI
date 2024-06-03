#pragma once
#include "I2CWire.h"

// I2C
#define UNIT_SCALES_ADDR 0x26

// SCALES REGISTER
#define UNIT_SCALES_RAW_ADC_REG 0x00
#define UNIT_SCALES_CAL_DATA_REG 0x10
#define UNIT_SCALES_BUTTON_REG 0x20
#define UNIT_SCALES_RGB_LED_REG 0x30
#define UNIT_SCALES_SET_GAP_REG 0x40
#define UNIT_SCALES_SET_OFFESET_REG 0x50
#define UNIT_SCALES_CAL_DATA_INT_REG 0x60
#define UNIT_SCALES_CAL_DATA_STRING_REG 0x70
#define UNIT_SCALES_FILTER_REG 0x80
#define UNIT_SCALES_JUMP_TO_BOOTLOADER_REG 0xFD
#define UNIT_SCALES_FIRMWARE_VERSION_REG 0xFE
#define UNIT_SCALES_I2C_ADDRESS_REG 0xFF

class UnitMiniScales : public I2CWire
{
    uint8_t buttonStatus;
    bool wasPressed;
public:
    bool begin(uint8_t addr = UNIT_SCALES_ADDR)
    {
        return I2CWire::begin(addr);
    }
    // ボタンが押されている
    bool isButtonPressed()
    {
        if(buttonStatus == 0)
            return true;
        return false;
    }
    // ボタンが押されたか
    bool wasButtonPressed()
    {
        if(wasPressed)
        {
            wasPressed = false;
            return true;
        }
        return false;
    }
    void update()
    {
        uint8_t stat = getBtnStatus();
        if(stat != buttonStatus){

            // ボタンが押された
            if(stat == 0){
                wasPressed = true;

                LOG_D("Button Pressed");
                this->setOffset();
            }
            // ボタンが離された
            else if(stat == 1){
                wasPressed = false;
            }

            buttonStatus = stat;
            LOG_I("Button Status: %d", buttonStatus);            
        }
    }
    uint8_t getBtnStatus()
    {
        uint8_t data = 0;
        readBytes(UNIT_SCALES_BUTTON_REG, &data, 1);
        return data;
    }

    bool setLEDColor(uint32_t colorHEX)
    {
        uint8_t color[4] = {0};
        color[0] = (colorHEX >> 16) & 0xff; // RED
        color[1] = (colorHEX >> 8) & 0xff;  // GREEN
        color[2] = colorHEX & 0xff;         // BLUE   
        return writeBytes(UNIT_SCALES_RGB_LED_REG, color, 3);
    }
    bool setLEDColor(uint8_t r, uint8_t g, uint8_t b)
    {
        uint8_t color[3] = {r, g, b};
        return writeBytes(UNIT_SCALES_RGB_LED_REG, color, 3);
    }

    bool setLPFilter(uint8_t en)
    {
        return writeBytes(UNIT_SCALES_FILTER_REG, &en,1);
    }

    uint8_t getLPFilter(void)
    {
        return readByte(UNIT_SCALES_FILTER_REG);
    }

    bool setAvgFilter(uint8_t avg)
    {
        return writeBytes(UNIT_SCALES_FILTER_REG + 1, &avg,1);
    }

    uint8_t getAvgFilter(void)
    {
        return readByte(UNIT_SCALES_FILTER_REG + 1);
    }

    bool setEmaFilter(uint8_t ema)
    {
        return writeBytes(UNIT_SCALES_FILTER_REG + 2, &ema,1);
    }

    uint8_t getEmaFilter(void)
    {
        return readByte(UNIT_SCALES_FILTER_REG + 2);
    }

    uint32_t getLEDColor()
    {
        uint8_t color[4] = {0};
        uint32_t colorHEX = 0;
        if (readBytes(UNIT_SCALES_RGB_LED_REG, color, 3))
        {
            colorHEX = color[0];
            colorHEX = (colorHEX << 8) | color[1];
            colorHEX = (colorHEX << 8) | color[2];
        };
        return colorHEX;
    }

    float getWeight()
    {
        uint8_t data[4];
        float c;
        uint8_t *p;

        if (readBytes(UNIT_SCALES_CAL_DATA_REG, data, 4))
        {
            p = (uint8_t *)&c;
            memcpy(p, data, 4);
        }
        return c;
    }

    int32_t getWeightInt()
    {
        uint8_t data[4];
        readBytes(UNIT_SCALES_CAL_DATA_INT_REG, data, 4);
        return (data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24));
    }

    String getWeightString()
    {
        char *p;
        uint8_t data[16];
        String res;

        readBytes(UNIT_SCALES_CAL_DATA_STRING_REG, data, 16);
        p = (char *)data;
        res = p;
        return res;
    }

    float getGapValue()
    {
        uint8_t data[4];
        float c;
        uint8_t *p;

        if (readBytes(UNIT_SCALES_SET_GAP_REG, data, 4))
        {
            p = (uint8_t *)&c;
            memcpy(p, data, 4);
        };
        return c;
    }

    void setGapValue(float offset)
    {
        uint8_t datatmp[4];
        uint8_t *p;
        p = (uint8_t *)&offset;

        memcpy(datatmp, p, 4);

        writeBytes(UNIT_SCALES_SET_GAP_REG, datatmp, 4);
        delay(100);
    }

    void setOffset(void)
    {
        uint8_t datatmp[4];
        datatmp[0] = 1;

        writeBytes(UNIT_SCALES_SET_OFFESET_REG, datatmp, 1);
    }

    int32_t getRawADC()
    {
        uint8_t data[4] = {0};
        int rawADC = 0;
        if (readBytes(UNIT_SCALES_RAW_ADC_REG, data, 4))
        {
            rawADC = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
        };

        return rawADC;
    }

    uint8_t setI2CAddress(uint8_t addr)
    {
        writeByte(UNIT_SCALES_I2C_ADDRESS_REG, addr);
        _addr = addr;
    }

    uint8_t getI2CAddress(void)
    {
        return readByte(UNIT_SCALES_I2C_ADDRESS_REG);
    }

    uint8_t getFirmwareVersion(void)
    {
        return readByte(UNIT_SCALES_FIRMWARE_VERSION_REG);
    }

    void jumpBootloader(void)
    {
        writeByte(UNIT_SCALES_JUMP_TO_BOOTLOADER_REG, 1);
    }
};

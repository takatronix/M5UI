#pragma once
#include "I2CWire.h"

#define ANGLE8_I2C_ADDR 0x43
#define ANGLE8_ANALOG_INPUT_12B_REG 0x00
#define ANGLE8_ANALOG_INPUT_8B_REG 0x10
#define ANGLE8_DIGITAL_INPUT_REG 0x20
#define ANGLE8_RGB_24B_REG 0x30

#define ANGLE8_FW_VERSION_REG 0xFE
#define ANGLE8_ADDRESS_REG 0xFF

#define ANGLE8_TOTAL_LED 9
#define ANGLE8_TOTAL_ADC 8

class UnitAngle8 : public I2CWire
{
public:
    float level[ANGLE8_TOTAL_ADC];
    uint8_t input[ANGLE8_TOTAL_ADC];
    bool digitalSwitch;

    bool begin(uint8_t addr = ANGLE8_I2C_ADDR)
    {
        bool ret = I2CWire::begin(addr);
        init();
        return ret;
    }
    void init()
    {
        //writeByte(ANGLE8_DIGITAL_INPUT_REG, 0);
        for (int i = 0; i < ANGLE8_TOTAL_LED; i++)
        {
            setLEDColor(i, 0, 0, 0, 0);
        }
    }

    void update()
    {
        if(!isConnected()){
            //LOG_E("Angle8 is not connected");
            return; 
        };

        digitalSwitch = getDigitalInput();
        setLEDColor(8, digitalSwitch ? 0x00ff00 : 0xff0000, 255);
        for (int i = 0; i < ANGLE8_TOTAL_ADC; i++)
        {
            float l = getLevel8(i);

            if(level[i] != l){
                this->level[i] = l;
                setLEDColorFloat(i, 1, 1, 1, l);
            }
        }
    }

    bool getDigitalInput()
    {
        return readByte(ANGLE8_DIGITAL_INPUT_REG);
    }
    uint8_t getAnalogInput8(uint8_t ch)
    {
        uint8_t reg = ch + ANGLE8_ANALOG_INPUT_8B_REG;
        uint8_t val = readByte(reg);
        input[ch] = val;
        return val;
    }
    uint16_t getAnalogInput12(uint8_t ch)
    {
        uint8_t reg = ch * 2 + ANGLE8_ANALOG_INPUT_12B_REG;
        return readWord(reg);
    }
    float getLevel12(uint8_t ch)
    {
        uint16_t val = getAnalogInput12(ch);
        return 1.0 - ((float)val / 4095.0);
    }
    float getLevel8(uint8_t ch)
    {
        uint8_t val = getAnalogInput8(ch);
        return 1.0 - ((float)val / 255.0);
    }

    void setLEDColor(uint8_t ch, uint32_t color, uint8_t bright)
    {
        if (ch > ANGLE8_TOTAL_LED)
            return;
        uint8_t data[4] = {0};
        bright = map(bright, 0, 255, 0, 100);
        data[0] = (color >> 16) & 0xff;
        data[1] = (color >> 8) & 0xff;
        data[2] = color & 0xff;
        data[3] = bright & 0xff;
        uint8_t reg = ch * 4 + ANGLE8_RGB_24B_REG;
        writeBytes(reg, data, 4);
    }
    void setLEDColor(uint8_t ch, uint8_t r, uint8_t g, uint8_t b, uint8_t bright)
    {
        if (ch > ANGLE8_TOTAL_LED)
            return;
        uint8_t data[4] = {0};
        bright = map(bright, 0, 255, 0, 100);
        data[0] = r;
        data[1] = g;
        data[2] = b;
        data[3] = bright & 0xff;
        uint8_t reg = ch * 4 + ANGLE8_RGB_24B_REG;
        writeBytes(reg, data, 4);
    }
    void setLEDColorFloat(uint8_t ch, float r, float g, float b, float bright)
    {
        setLEDColor(ch, (uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), (uint8_t)(bright * 255));
    }
    uint8_t getVersion()
    {
        uint8_t reg = ANGLE8_FW_VERSION_REG;
        return readByte(reg);
    }
    uint8_t getAddress()
    {
        uint8_t reg = ANGLE8_ADDRESS_REG;
        return readByte(reg);
    }
    void print()
    {
        //Serial.printf("Digital: %d\n", digitalSwitch);
        String s;
        for (int i = 0; i < ANGLE8_TOTAL_ADC; i++)
        {
            s += String(level[i], 2);
            s += " ";
        }
        Serial.printf("Analog: %s\n", s.c_str());
    }
};

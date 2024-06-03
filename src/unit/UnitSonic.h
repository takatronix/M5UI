#pragma once
#include "I2CWire.h"

#define ULTRASONIC_I2C_ADDR 0x57

class UnitSonic : public I2CWire
{
public:
    float distance = 0;
    bool begin(uint8_t addr = ULTRASONIC_I2C_ADDR)
    {
        // 400000ではエラーが発生する
        return I2CWire::begin(addr);
    }
    float getDistance()
    {
        uint32_t data;
        _wire->beginTransmission(_addr);
        _wire->write(0x01);
        _wire->endTransmission();

        delay(10);
        _wire->requestFrom(_addr, (uint8_t)3);
        data = _wire->read();
        data <<= 8;
        data |= _wire->read();
        data <<= 8;
        data |= _wire->read();
        if(data < 0){
            return distance;
        }

        float dist = float(data) / 1000;
        if(dist > 20 && dist < 4000){
            distance = dist;
        }
        LOG_D("distance:%f", distance);    
        return distance;
    }
};

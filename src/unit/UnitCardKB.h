#pragma once
#include "I2CWire.h"
#include <BleKeyboard.h>

#define CARDKB_ADDR 0x5F

class UnitCardKB : public I2CWire
{
public:
    bool begin(uint8_t addr = CARDKB_ADDR)
    {
        return I2CWire::begin(addr);
    }

    // キー入力を読みだす。取得できなければ-1を返す
    int getKey()
    {
        _wire->requestFrom(_addr, (uint8_t)1);
        if(_wire->available() == 0){
            return -1;
        }
        uint8_t data = _wire->read();
        if(data == 0){
            return -1;
        }
        LOG_D("key:%x", data);
        return convertKey(data);
    }

    char convertKey(char ch)
    {
        switch (ch)
        {
        case 0x0D:
            return KEY_RETURN;
        case 0x1B:
            return KEY_ESC;
        case 0x09:
            return KEY_TAB;
        case 0xB4:
            return KEY_LEFT_ARROW;
        case 0xB7:
            return KEY_RIGHT_ARROW;
        case 0xB5:
            return KEY_UP_ARROW;
        case 0xB6:
            return KEY_DOWN_ARROW;
        default:
            return ch;
        }
    }
};

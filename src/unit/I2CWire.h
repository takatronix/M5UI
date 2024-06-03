#pragma once
#include <Wire.h>

#define I2CGROVE_DEFAULT_FREQUENCY 400000
class I2CWire
{
public:
    uint8_t _addr;
    TwoWire *_wire;
    bool _isConnected = false;
    bool begin(TwoWire& wire, uint16_t addr,int sda=-1,int scl=-1)
    {
        _wire = &wire;
        _addr = addr;
        _wire->begin(sda,scl);
        delay(10);
        _wire->beginTransmission(_addr);
        uint8_t error = _wire->endTransmission();

        if (error == 0) {
            LOG_D("I2C connected: addr=%xh", addr);
            _isConnected = true;
            return true;
        } 
        _isConnected = false;
        return false; 
    }

    bool begin(uint16_t addr)
    {
        LOG_D("I2C begin: addr=%xh, frequency=%d", addr);
        _wire = &Wire;
        _addr = addr;
        _wire->begin();
        delay(10);
        _wire->beginTransmission(_addr);
        uint8_t error = _wire->endTransmission();

        if (error == 0) {
            LOG_D("I2C connected: addr=%xh", addr);
            _isConnected = true;
            return true;
        } 
        
        LOG_E("I2C not connected: addr=%xh", addr);
        _isConnected = false;
        return false; 
    }
    bool isConnected(){
        return _isConnected;
    }
    void end()
    {
        _wire->end();
    }
    
    void writeByte(uint8_t reg, uint8_t data)
    {
       // LOG_D("writeByte: address=%x, reg=%d, data=%d", _addr, reg, data);
        _wire->beginTransmission(_addr);
        _wire->write(reg);
        _wire->write(data);
        _wire->endTransmission();
    }

    uint8_t readByte(uint8_t reg)
    {
        uint8_t data;
        readBytes(reg, &data, 1);
        return data;
    }

    // big endian
    uint16_t readWord(uint8_t reg)
    {
        uint8_t data[2];
        readBytes(reg, data, 2);
        return (data[1] << 8) | data[0];
    }
    
    void writeWord(uint8_t reg, uint16_t data)
    {
        uint8_t buf[2] = {(uint8_t)(data >> 8), (uint8_t)(data & 0xFF)};
        writeBytes(reg, buf, 2);
    }

    int writeBytes(uint8_t reg, uint8_t *data, uint8_t length)
    {
        //LOG_D("writeBytes: address=%x, reg=%d, length=%d", _addr, reg, length);
        _wire->beginTransmission(_addr);
        _wire->write(reg);
        int result = 0;
        for (uint8_t i = 0; i < length; i++)
        {
            auto ret = _wire->write(data[i]);
            result += ret;
        }
        _wire->endTransmission();
        //LOG_D("writeBytes: result=%d", result);
        return result;
    }

    bool readBytes(uint8_t reg, uint8_t *buffer,uint8_t length) {
        //LOG_D("readBytes: address=%x, reg=%d, length=%d", _addr, reg, length);
        uint8_t index = 0;
        _wire->beginTransmission(_addr);
        _wire->write(reg);
        _wire->endTransmission();
        if (_wire->requestFrom(_addr, length)) {
            for (uint8_t i = 0; i < length; i++) {
                buffer[index++] = _wire->read();
            }
            return true;
        }
        LOG_E("readBytes failed: address=%x, reg=%d, length=%d", _addr, reg, length);
        return false;
    }

};

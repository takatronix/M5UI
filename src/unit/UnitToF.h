#pragma once
#include "I2CWire.h"

#define TOF_I2C_ADDR 0x29 // I2C address
#define TOF_REG_IDENTIFICATION_MODEL_ID 0xc0
#define TOF_REG_IDENTIFICATION_REVISION_ID 0xc2
#define TOF_REG_PRE_RANGE_CONFIG_VCSEL_PERIOD 0x50
#define TOF_REG_FINAL_RANGE_CONFIG_VCSEL_PERIOD 0x70
#define TOF_REG_SYSRANGE_START 0x00
#define TOF_REG_RESULT_INTERRUPT_STATUS 0x13
#define TOF_REG_RESULT_RANGE_STATUS 0x14

class UnitToF : public I2CWire
{
    uint8_t _buf[16];

public:
    uint16_t distance;
    uint16_t ambientCount;
    uint16_t signalCount;
    bool begin(uint8_t addr = TOF_I2C_ADDR)
    {
        bool ret = I2CWire::begin(addr);
        return ret;
    }

    void update()
    {
        if (!isConnected())
        {
            //LOG_E("UnitToF is not connected");
            return;
        };

        measureDistance();
    }

    void rangeStart()
    {
        writeByte(TOF_REG_SYSRANGE_START, 0x01);
    }

    // 1ならready
    uint8_t readRangeStatus()
    {
        return readByte(TOF_REG_RESULT_RANGE_STATUS);
    }

    uint16_t measureDistance()
    {
        rangeStart();

        if (!readBytes(TOF_REG_RESULT_RANGE_STATUS, _buf, 12))
        {
            LOG_E("UnitToF: Failed to read data");
            return 0;
        }

        uint16_t acnt = makeuint16(_buf[7], _buf[6]);
        uint16_t scnt = makeuint16(_buf[9], _buf[8]);
        uint16_t dist = makeuint16(_buf[11], _buf[10]);
        uint8_t deviceRangeStatusInternal = ((_buf[0] & 0x78) >> 3);

        distance = dist;
        ambientCount = acnt;
        signalCount = scnt;

        //LOG_D("UnitToF: dist=%d, acnt=%d, scnt=%d interval=%d", dist, acnt, scnt,deviceRangeStatusInternal);

        return dist;
    }

    uint16_t makeuint16(int lsb, int msb)
    {
        return ((msb & 0xFF) << 8) | (lsb & 0xFF);
    }
};

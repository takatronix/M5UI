#pragma once

#include <BluetoothSerial.h>
#include <functional>

BluetoothSerial SerialBT;

class BTSerial {
public:
    BTSerial(const char* deviceName, bool isMaster = false, const char* targetDeviceName = "") 
        : _deviceName(deviceName), _isMaster(isMaster), _targetDeviceName(targetDeviceName) {

            LOG_I("Device name: %s", deviceName);
            LOG_I("Device Name %s", _deviceName);
    }

    void begin() {
        LOG_I("Bluetooth start");
        if (_isMaster) {
            LOG_I("Master mode begin", _deviceName);
            if (!SerialBT.begin(_deviceName, true)) { // Master mode
                LOG_E("Bluetooth start failed");
                return;
            }
            LOG_I("Bluetooth started connecting to %s", _targetDeviceName);
            SerialBT.connect(_targetDeviceName);
            while (!SerialBT.connected(10000)) {
                LOG_I("Connecting to %s",_targetDeviceName);
                delay(1000);
            }
            LOG_I("Connected to target device");
        } else {
            LOG_I("Slave mode begin", _deviceName);
            if (!SerialBT.begin(_deviceName)) { // Slave mode
                LOG_E("Bluetooth start failed");
            } else {
                LOG_I("Bluetooth started");
            }
        }
        _buffer.reserve(256); // バッファの初期サイズを設定
        LOG_I("Bluetooth started");
    }

    void send(const String& message) {
        SerialBT.println(message);
    }

    void receive() {
        while (SerialBT.available()) {
            char c = SerialBT.read();
            LOG_D("Received: %c", c);
            if (c == '\n') {
                if (_buffer.length() > 0) {
                    if (_receiveCallback) {
                        _receiveCallback(_buffer);
                    }
                    _buffer = ""; // バッファをクリア
                }
            } else {
                _buffer += c;
            }
        }
    }

    void setReceiveCallback(std::function<void(const String&)> callback) {
        _receiveCallback = callback;
    }

    void update() {
        receive();
    }
    
    static std::vector<String> splitString(const String& str, char delimiter) {
        std::vector<String> tokens;
        String token;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == delimiter) {
                if (token.length() > 0) {
                    tokens.push_back(token);
                }
                token = "";
            } else {
                token += str[i];
            }
        }
        if (token.length() > 0) {
            tokens.push_back(token);
        }
        return tokens;
    }
    const char* getDeviceName(){
        return _deviceName;
    }
    const char* getTargetDeviceName(){
        return _targetDeviceName;
    }
private:
    const char* _deviceName;
    const char* _targetDeviceName;
    String _buffer;
    bool _isMaster;
    std::function<void(const String&)> _receiveCallback = nullptr;
};


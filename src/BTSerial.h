#pragma once

#include <BluetoothSerial.h>
#include <functional>


class BTSerial {
    BluetoothSerial SerialBT;
    // 終端文字
    static const char TERMINATOR = '\n';
    // バッファ最大サイズ
    static const int MAX_BUFFER_SIZE = 256;
public:
    BTSerial(const char* deviceName, bool isMaster = false, const char* targetDeviceName = "") 
        : _deviceName(deviceName), _isMaster(isMaster), _targetDeviceName(targetDeviceName) {
    }

    bool begin() {
        LOG_I("Bluetooth start");
        _buffer.reserve(MAX_BUFFER_SIZE); // バッファの初期サイズを設定
        if(SerialBT.begin(_deviceName,_isMaster)){
            LOG_I("Bluetooth started");
            if(_isMaster){
                return connect();
            }
            return true;
        }
        LOG_E("Bluetooth start failed");
        return false;
    }
    bool connect(){
        if(_targetDeviceName == ""){
            LOG_E("Target device name is not set");
            return false;
        }
        LOG_I("Bluetooth connecting to %s", _targetDeviceName);
        if(!SerialBT.connect(_targetDeviceName)){
            LOG_E("Failed to connect to target device %s", _targetDeviceName);
            return false;
        }
        LOG_I("Connected to target device %s", _targetDeviceName);
        return true;
    }
    bool isConnected(){
        return SerialBT.connected();
    }
    void send(const String& message) {
        SerialBT.println(message);
    }

    // \nを受信したらコールバックを呼ぶ
    void receive() {
        while (SerialBT.available()) {
            char c = SerialBT.read();
            LOG_D("Received: %c", c);
            if (c == TERMINATOR) {
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


#pragma once

#include <ArduinoJson.h>
#include <SPIFFS.h>

// SPIFFの設定ファイルを扱うクラス
// SIFFSの初期化はFileManagerで行うため、ここでは不要
class SettingsFile {
public:
    SettingsFile(const char* filename) : _filename(filename) {}

    void saveJson(const JsonObject& settings) {
        LOG_I("Opening file for writing...");
        File file = SPIFFS.open(_filename, FILE_WRITE);
        if (!file) {
            LOG_E("Failed to open file for writing");
            return;
        }

        LOG_V("Serializing JSON...");
        serializeJson(settings, file);
        file.close();
        LOG_I("Settings saved successfully");
    }

    bool loadJson(JsonDocument& doc) {
        LOG_I("Opening file for reading...");
        File file = SPIFFS.open(_filename, FILE_READ);
        if (!file) {
            LOG_E("Failed to open file for reading");
            return false;
        }

        LOG_I("Deserializing JSON...");
        DeserializationError error = deserializeJson(doc, file);
        file.close();
        if (error) {
            LOG_E("Failed to read file: %s",error.c_str());
            return false;
        }
        LOG_I("Settings loaded successfully");
        return true;
    }

    bool deleteSettings() {
        LOG_I("Attempting to delete file: %s", _filename);

        bool result = SPIFFS.remove(_filename);
        if (result) {
            LOG_I("File deleted successfully");
        } else {
            LOG_E("Failed to delete file");
        }
        return result;
    }

    void print() {
        LOG_I("Opening file for reading...");
        File file = SPIFFS.open(_filename, FILE_READ);
        if (!file) {
            LOG_E("Failed to open file for reading");
            return;
        }

       LOG_V("File content:");
        while (file.available()) {
            LOG_V((char*)file.read());
        }
        file.close();
    }

private:
    const char* _filename;
};

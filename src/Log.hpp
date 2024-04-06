#pragma once
#include <M5Unified.h>

// ログレベル（ログレベルによって出力するログを制御する）
#ifdef LOG_ENABLE_ERROR
#define LOG_E(format, ...) Log::error(format, ##__VA_ARGS__)
#else
#define LOG_E(format, ...)
#endif

#ifdef LOG_ENABLE_WARN
#define LOG_W(format, ...) Log::warn(format, ##__VA_ARGS__)
#else
#define LOG_W(format, ...)
#endif

#ifdef LOG_ENABLE_INFO
#define LOG_I(format, ...) Log::info(format, ##__VA_ARGS__)
#else
#define LOG_I(format, ...)
#endif

#ifdef LOG_ENABLE_DEBUG
#define LOG_D(format, ...) Log::debug(format, ##__VA_ARGS__)
#else
#define LOG_D(format, ...)
#endif

#ifdef LOG_ENABLE_VERBOSE
#define LOG_V(format, ...) Log::verbose(format, ##__VA_ARGS__)
#else
#define LOG_V(format, ...)
#endif




class Log
{
public:
    // 出力先のキャンバスのリスト
    static std::vector<M5Canvas *> _canvasList;
    // ログを配信するCanvas*を追加する
    static void addCanvas(M5Canvas *canvas)
    {
        _canvasList.push_back(canvas);
    }
    // ログを配信するCanvas*を削除する
    static void removeCanvas(M5Canvas *canvas)
    {
        _canvasList.erase(std::remove(_canvasList.begin(), _canvasList.end(), canvas), _canvasList.end());
    }
    // ログを配信するCanvas*を全て削除する
    static void removeAllCanvas()
    {
        _canvasList.clear();
    }
    // キャンバスにエラーを出力する
    static void errorToCanvas(M5Canvas *canvas, const char *message)
    {
        canvas->setTextSize(1);
        canvas->setTextColor(TFT_RED);
        canvas->println(message);
    }
    // キャンバスに警告を出力する
    static void warnToCanvas(M5Canvas *canvas, const char *message)
    {
        canvas->setTextSize(1);
        canvas->setTextColor(TFT_YELLOW);
        canvas->println(message);
    }
    // キャンバスに情報を出力する
    static void infoToCanvas(M5Canvas *canvas, const char *message)
    {
        canvas->setTextSize(1);
        canvas->setTextColor(TFT_WHITE);
        canvas->println(message);
    }
    // キャンバスにデバッグを出力する
    static void debugToCanvas(M5Canvas *canvas, const char *message)
    {
        canvas->setTextSize(1);
        canvas->setTextColor(TFT_GREEN);
        canvas->println(message);
    }
    // キャンバスに詳細を出力する
    static void verboseToCanvas(M5Canvas *canvas, const char *message)
    {
        canvas->setTextSize(1);
        canvas->setTextColor(TFT_CYAN);
        canvas->println(message);
    }
    // すべてのキャンバスにエラーを出力する
    static void errorToAllCanvas(const char *message)
    {
        for (auto canvas : _canvasList)
        {
            errorToCanvas(canvas, message);
        }
    }
    // すべてのキャンバスに警告を出力する
    static void warnToAllCanvas(const char *message)
    {
        for (auto canvas : _canvasList)
        {
            warnToCanvas(canvas, message);
        }
    }
    // すべてのキャンバスに情報を出力する
    static void infoToAllCanvas(const char *message)
    {
        for (auto canvas : _canvasList)
        {
            infoToCanvas(canvas, message);
        }
    }
    // すべてのキャンバスにデバッグを出力する
    static void debugToAllCanvas(const char *message)
    {
        for (auto canvas : _canvasList)
        {
            debugToCanvas(canvas, message);
        }
    }
    // すべてのキャンバスに詳細を出力する
    static void verboseToAllCanvas(const char *message)
    {
        for (auto canvas : _canvasList)
        {
            verboseToCanvas(canvas, message);
        }
    }
    
    static void info(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        Serial.print("[info]");
        Serial.println(buffer);
        infoToAllCanvas(buffer);
    }
    static void error(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        Serial.print("[error]");
        Serial.println(buffer);
        errorToAllCanvas(buffer);
    }

    static void debug(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        Serial.print("[debug]");
        Serial.println(buffer);
        debugToAllCanvas(buffer);
    }

    static void warn(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        Serial.print("[warn]");
        Serial.println(buffer);
        warnToAllCanvas(buffer);
    }

    static void verbose(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        Serial.print("[verbose]");
        Serial.println(buffer);
        verboseToAllCanvas(buffer);
    }
};
#pragma endregion

#endi
#pragma once
#include "Sprite.hpp"
#include "string.h"


class WiFiSprite : public Sprite
{
public:
    String ssid;
    String password;
    String ip;

    bool color = WHITE;
    bool isConnected = false;
    StopWatch sw;
    WiFiSprite(M5Canvas *pDisplay, int width = 16, int height = 16, int depth = M5UI_COLOR_DEPTH, bool psram = false) : Sprite(pDisplay, width, height, depth, psram)
    {
        if (create(width, height, 0, 0, depth, psram) == NULL)
        {
            LOG_E("Text Sprite create error");
        }
        Sprite::add(this);
        enableTransparent = true;

        // 未接続の場合にはSmartConfig待受
        if ( WiFi.status() != WL_CONNECTED ) {
            WiFi.mode(WIFI_STA);
            WiFi.beginSmartConfig();
            LOG_I("WiFi.beginSmartConfig()");
        }
    }
    
    virtual bool update(void)
    {
        // 1秒ごとに接続チェック
        if (sw.Elapsed() < 1000)
            return false;

        sw.reset();

        // 接続状態に変化があれば更新
        bool connected = WiFi.status() == WL_CONNECTED;
        if (connected != isConnected)
        {
            WiFi.stopSmartConfig();
            isConnected = connected;
            if(isConnected) {
                // ipアドレスを取得
                ip = WiFi.localIP().toString();
                ssid = WiFi.SSID();
                LOG_I("WiFi connected. IP address: %s", ip);
            }
            return true;
        }

        // falseを返すと描画処理がスキップされる
        return false;
    }

    virtual void onDraw(void)
    {
        canvas.clear();
        drawWifiIcon(&canvas, 0, 0, _width, _height, color,isConnected);
    }

    void drawWifiIcon(M5Canvas* canvas,int x=0,int y=0, int width=16, int height = 16, uint16_t color = WHITE,bool isConnected = true) {
        int centerX = x + width / 2;
        int centerY = y + height / 2;
        int radius = min(width, height) / 2;

        // WiFi のアークを描画
        if(isConnected){
            canvas->drawArc(centerX, centerY, radius, 0, 180, 2, color);
            canvas->drawArc(centerX, centerY, radius * 0.7, 0, 180, 2, color);
            canvas->drawArc(centerX, centerY, radius * 0.4, 0, 180, 2, color);
        }

        // WiFi シンボルの中心部分を描画
        canvas->fillCircle(centerX, centerY, radius * 0.1, color);
    }
};

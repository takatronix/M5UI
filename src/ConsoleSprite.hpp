#pragma once
#include "Sprite.hpp"

#include <efontEnableAll.h>
#include <efont.h>

class ConsoleSprite : public Sprite{
public:
    int textSize = 1;
    uint32_t textColor = 0xFFFFFF;
    uint32_t backgroundColor = 0x00000;
    bool drawTextBackground = false;

    ConsoleSprite(M5GFX* pDisplay) : Sprite(pDisplay) {
    canvas.setTextScroll(true);
    }

    ~ConsoleSprite(){
    }

    // スクロールの必要があればスクロール
    void scrollIfNeeded(int height){
        int posX = canvas.getCursorX();
        int posY = canvas.getCursorY();

        if(posY + height > canvas.height()){
            canvas.scroll(0, - height);
            posY -= height;
        }
        canvas.setCursor(posX, posY);
    }
    void print(const char *message)
    {
        puts(message,textSize,textColor,backgroundColor);
    }
    void printf(const char *format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        puts(buffer,textColor,backgroundColor);
    }
    void println(const char *format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        puts(buffer,textColor,backgroundColor);
        puts("\n",textSize,textColor,backgroundColor);
    }

    void puts(const char* str,int textSize = 1,uint32_t textColor = 0xFFFFFF,uint32_t backgroundColor = 0x00000){
        while (*str != 0x00) {
            put(*str,textSize,textColor,backgroundColor);
            str++;
            continue;
        }
    }

    /// @brief 一文字表示
    /// @param ch 表示する文字コード
    /// @param textSize テキストサイズ(1,2,4,8)
    /// @param textColor 描画色
    /// @param backColor 背景色
    void put(char ch,int textSize = 1,uint32_t textColor = 0xFFFFFF,uint32_t backgroundColor = 0x00000){
        // テキストサイズ
        int textWidth = textSize * 16;
        int textHeight = textSize * 16;

        // 半角
        if(ch < 0x80){
            textWidth = textSize * 8;
        }

        // 表示するフォントが下方向にはみ出す場合は上にスクロール
        scrollIfNeeded(textHeight);

        // 改行の場合は改行してカーソルを進める  
        int posX = canvas.getCursorX();
        int posY = canvas.getCursorY();

        if(ch == '\n'){
            scrollIfNeeded(textHeight);
            posX = 0;
            posY += textHeight;
            canvas.setCursor(posX, posY);
            return;
        }
        if(ch == '\r'){
            posX = 0;
            canvas.setCursor(posX, posY);
            return;
        }

        // 文字背景塗りつぶし
        canvas.fillRect(posX, posY, textWidth, textHeight, backgroundColor);

        // フォント取得
        byte font[32];
        uint16_t strUTF16;
        efontUFT8toUTF16( &strUTF16, &ch );
        getefontData( font, strUTF16 );

        // フォント描画
        for (uint8_t row = 0; row < 16; row++) {
            word fontdata = font[row*2] * 256 + font[row*2+1];
            for (uint8_t col = 0; col < 16; col++) {
            if( (0x8000 >> col) & fontdata ){
                int drawX = posX + col * textSize;
                int drawY = posY + row * textSize;
                if( textSize == 1 ){
                canvas.drawPixel(drawX, drawY, textColor);
                } else {
                canvas.fillRect(drawX, drawY, textSize, textSize, textColor);
                }
            }
            }
        }

        // カーソルを進める、画面外にカーソルがすすめば折り返し
        posX += textWidth;
        if(posX >= canvas.width()){
            posX = 0;
            posY += textHeight;
        }
        // カーソルを更新
        canvas.setCursor(posX, posY);
    }
};

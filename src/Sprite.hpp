#pragma once
#include <M5Unified.h>

#ifndef M5UI_COLOR_DEPTH
#define M5UI_COLOR_DEPTH 8
#endif

enum PositionType
{
    None,
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft,
    MiddleCenter,
    MiddleRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    Center,
};
class Sprite
{
protected:
    int _x = 0;
    int _y = 0;
    int _cx = 0;
    int _cy = 0;
    int _width = 0;
    int _height = 0;
    static int _spriteNo;
    float _angle = 0.0f;
    float _scale = 1.0f;

    bool _shouldBackup = false;
    bool _shouldRedraw = true;

    int _textSize;
    uint16_t _textColor;
    uint16_t _backgroundColor;
    uint16_t _transparentColor = 0;
    float _matrix[6] = {1, 0, 0, 0, 1, 0};

public:
    int _id;
    String tag;
    PositionType positionType = PositionType::None;
    bool enableTransparent = false;
    bool enableAffine = false;
    bool enableAA = false;

void calculateAffineTransformMatrix(float x, float y, float cx, float cy, float width, float height, float angle=0.0f, float scale=1.0f) {
    // 回転の角度をラジアンに変換
    float rad = angle * (M_PI / 180.0f);

    // 画像の中心座標を計算（グローバル座標系での位置）
    float centerX = x + cx + width / 2;
    float centerY = y + cy + height / 2;

    // ステップ1: 画像中心を原点に移動するための補正
    float correctedX = -width / 2;
    float correctedY = -height / 2;

    // アフィン変換行列の計算
    _matrix[0] = cos(rad) * scale; // 0行0列
    _matrix[1] = -sin(rad) * scale; // 0行1列
    _matrix[2] = centerX + correctedX * cos(rad) - correctedY * sin(rad); // 平行移動x
    _matrix[3] = sin(rad) * scale; // 1行0列
    _matrix[4] = cos(rad) * scale; // 1行1列
    _matrix[5] = centerY + correctedX * sin(rad) + correctedY * cos(rad); // 平行移動y
}
    Sprite &setX(int x)
    {
        _x = x;
        if (enableAffine)
            calculateAffineTransformMatrix(_x, _y, _cx, _cy,_width,_height, _angle, _scale);
        return *this;
    }
    Sprite &setY(int y)
    {
        _y = y;
        if (enableAffine)
            calculateAffineTransformMatrix(_x, _y, _cx, _cy,_width,_height, _angle, _scale);
        return *this;
    }
    Sprite &setCenterX(int x)
    {
        _cx = x;
        if (enableAffine)
            calculateAffineTransformMatrix(_x, _y, _cx, _cy,_width,_height, _angle, _scale);
        return *this;
    }
    Sprite &setCenterY(int y)
    {
        _cy = y;
        if (enableAffine)
            calculateAffineTransformMatrix(_x, _y, _cx, _cy,_width,_height, _angle, _scale);
        return *this;
    }
    Sprite &setWidth(int width)
    {
        _width = width;
        if (enableAffine)
            calculateAffineTransformMatrix(_x, _y, _cx, _cy,_width,_height, _angle, _scale);
        return *this;
    }
    Sprite &setHeight(int height)
    {
        _height = height;
        if (enableAffine)
            calculateAffineTransformMatrix(_x, _y, _cx, _cy,_width,_height, _angle, _scale);
        return *this;
    }
    Sprite &setAngle(float angle)
    {
        _angle = angle;
        if (enableAffine)
            calculateAffineTransformMatrix(_x, _y, _cx, _cy,_width,_height, _angle, _scale);
        return *this;
    }
    // duration秒かけてangleに変化させる
    Sprite &setAngle(float angle, int duration, TweenType type = TweenType::LINEAR, std::function<void()> callback = NULL)
    {
        if (_angle == angle)
            return *this;
        if (duration == 0)
        {
            _angle = angle;
            return *this;
        }
        Tween::create(_angle, angle, duration, type, loop)->start().onUpdate([this](float progress, float value)
                                                                             { _angle = value; })
            .onComplete([this, callback]()
                        {
            if(callback != NULL) callback(); });
        return *this;
    }

    Sprite &setScale(float scale)
    {
        _scale = scale;
        if (enableAffine)
            calculateAffineTransformMatrix(_x, _y, _cx, _cy,_width,_height, _angle, _scale);

        return *this;
    }


    Sprite &setTextColor(uint16_t color)
    {
        _textColor = color;
        canvas.setTextColor(color);
        return *this;
    }
    Sprite &setBackgroundColor(uint16_t color)
    {
        _backgroundColor = color;
        // canvas.setBackgroundColor(color);
        return *this;
    }
    Sprite &setTextSize(int size)
    {
        _textSize = size;
        canvas.setTextSize(size);
        return *this;
    }
    Sprite &setTransparentColor(uint16_t color)
    {
        _transparentColor = color;
        enableTransparent = true;
        // canvas.setTransparentColor(color);
        return *this;
    }

#pragma region SpriteManager

    static std::vector<Sprite *> _sprites;
    static bool updateAll()
    {
        bool shouldRefresh = false;
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i]->push())
            {
                shouldRefresh = true;
            }
        }
        return shouldRefresh;
    }
    static void setup()
    {
        _sprites.clear();
        _spriteNo = 0;
    }
    static void add(Sprite *sprite)
    {
        if (sprite->_id != 0 && Sprite::findById(sprite->_id) != NULL)
        {
            LOG_E("Sprite ID is already used:%d", sprite->_id);
            return;
        }

        if (sprite->_id == 0)
            sprite->_id = newId();
        _sprites.push_back(sprite);
    }
    static void remove(Sprite *sprite)
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i] == sprite)
            {
                _sprites.erase(_sprites.begin() + i);
                delete sprite;
                break;
            }
        }
    }
    // 最前面に移動
    static void bringToFront(Sprite *sprite)
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i] == sprite)
            {
                _sprites.erase(_sprites.begin() + i);
                _sprites.push_back(sprite);
                break;
            }
        }
    }
    // 最背面に移動
    static void sendToBack(Sprite *sprite)
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i] == sprite)
            {
                _sprites.erase(_sprites.begin() + i);
                _sprites.insert(_sprites.begin(), sprite);
                break;
            }
        }
    }
    // 削除
    static void remove(int id)
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i]->id() == id)
            {
                delete _sprites[i];
                _sprites.erase(_sprites.begin() + i);
                break;
            }
        }
    }
    // 削除
    static void remove(String tag)
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i]->tag == tag)
            {
                delete _sprites[i];
                _sprites.erase(_sprites.begin() + i);
                break;
            }
        }
    }
    // 全削除
    static void clear()
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            delete _sprites[i];
        }
        _sprites.clear();
    }
    static void pushAll()
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            _sprites[i]->push();
        }
    }
    // 新しいスプライトIDを取得する(0は未初期化に使用するため1から開始する)
    static int newId()
    {
        return ++_spriteNo;
    }
    // タグで検索
    static std::vector<Sprite *> findByTag(String tag)
    {
        std::vector<Sprite *> list;
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i]->tag == tag)
            {
                list.push_back(_sprites[i]);
            }
        }
        return list;
    }
    // IDで検索
    static Sprite *findById(int id)
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i]->_id == id)
            {
                return _sprites[i];
            }
        }
        return NULL;
    }
    static Sprite &get(int id)
    {
        return *findById(id);
    }
    static Sprite &get(String tag)
    {
        return *findByTag(tag)[0];
    }

#pragma endregion
    M5Canvas *parentCanvas;
    int _depth = M5UI_COLOR_DEPTH;
    bool _psram = false;

    M5Canvas canvas;           // スプライトのキャンバス
    M5Canvas backgroundCanvas; // 画面書き戻し用のキャンバス
    Sprite(M5Canvas *parent) : parentCanvas(parent)
    {
        parentCanvas = parent;
    }
    Sprite(M5Canvas *parent, int width, int height, int x = 0, int y = 0, int depth = M5UI_COLOR_DEPTH, bool psram = false) : parentCanvas(parent)
    {
        parentCanvas = parent;
        if (create(width, height, x, y, depth, psram) == NULL)
        {
            LOG_E("Sprite create error");
        }
        Sprite::add(this);
    }
    int id()
    {
        return _id;
    }
    ~Sprite()
    {
        deinit();
    };
    void deinit()
    {
        canvas.deleteSprite();
    }
    // 中心点を設定する
    void setPivotCenter(void)
    {
        canvas.setPivot(width() / 2.0f, (float)height() / 2.0f);
    }
    void pushBackground(void)
    {
        uint8_t *buffer = new uint8_t[width() * height() * _depth / 8];
        parentCanvas->readRect(_x, _y, width(), height(), buffer);
        backgroundCanvas.pushImage(0, 0, width(), height(), buffer);
        delete[] buffer;
        _shouldBackup = true;
    }
    void popBackground(void)
    {
        if (_shouldBackup)
        {
            uint8_t *buffer = new uint8_t[width() * height() * _depth / 8];
            backgroundCanvas.readRect(0, 0, width(), height(), buffer);
            parentCanvas->pushImage(_x, _y, width(), height(), buffer);
            delete[] buffer;
            _shouldBackup = false;
        }
    }

    uint32_t convertColor(uint32_t color)
    {
        if (_depth == 8)
            return M5.Lcd.color16to8(color);
        return color;
    }

    void clear(uint16_t color = TFT_BLACK)
    {
        canvas.setCursor(0, 0);
        if (_depth == 8)
        {
            canvas.fillSprite(M5.Lcd.color16to8(color));
            return;
        }
        canvas.fillSprite(color);
    }

    /// @brief          スプライトを作成する
    /// @param width    作成するスプライトの幅
    /// @param height   作成するスプライトの高さ
    /// @param x        作成するスプライトのX座標
    /// @param y        作成するスプライトのY座標
    /// @param depth    作成するスプライトの色深度(8bitでないとメモリ不足になる可能性がある)
    /// @param psram    PSRAMを使用するかどうか
    /// @return         true:成功 false:失敗
    bool create(int width, int height, int x = 0, int y = 0, int depth = 8, bool psram = false)
    {
        _x = x;
        _y = y;
        _depth = depth;
        _psram = psram;

        canvas.setPsram(psram);
        canvas.setColorDepth(depth);
        if (canvas.createSprite(width, height) == NULL)
        {
            LOG_E("Sprite create error");
            return false;
        }

        backgroundCanvas.setPsram(psram);
        backgroundCanvas.setColorDepth(depth);
        if (backgroundCanvas.createSprite(width, height) == NULL)
        {
            LOG_E("Sprite create error");
            canvas.deleteSprite();
            return false;
        }

        _width = width;
        _height = height;

        _cx = width / 2;
        _cy = height / 2;

        return true;
    }

#pragma region Property
    int x()
    {
        return _x;
    }
    void x(int value)
    {
        _x = value;
    }
    int y()
    {
        return _y;
    }
    void y(int value)
    {
        _y = value;
    }
    int width()
    {
        return canvas.width();
    }
    int height()
    {
        return canvas.height();
    }
#pragma endregion
    void redraw()
    {
        _shouldRedraw = true;
    }
    virtual void draw()
    {
        // debug
        canvas.clear(TFT_RED);
        canvas.setCursor(0, 0);
        canvas.printf("%s(%d)\n", tag.c_str(), _id);
        canvas.printf("%d,%d\n", _x, _y);
        canvas.printf("%d,%d\n", _width, _height);
    }

#pragma region Push
    virtual bool push(void)
    {
        bool shouldRefresh = false;
        // 背景バックアップONの場合は背景を復帰
        if (_shouldBackup)
        {
            popBackground();
        }
        if (_shouldRedraw)
        {
            draw();
            _shouldRedraw = false;
            shouldRefresh = true;
        }
        canvas.pushSprite(parentCanvas, _x, _y);
        return shouldRefresh;
    }

#pragma endregion

#pragma region Move
    void moveTo(int x, int y, unsigned long duration = 100, TweenType type = TweenType::LINEAR, bool loop = false)
    {
        if (_x == x && _y == y)
            return;
        if (duration == 0)
        {
            _x = x;
            _y = y;
            return;
        }

        Tween::create(_x, x, duration, type, loop)->start().onUpdate([this](float progress, float value)
                                                                     { _x = value; });

        Tween::create(_y, y, duration, type, loop)->start().onUpdate([this](float progress, float value)
                                                                     { _y = value; });
    }

    void moveToTopLeft(void)
    {
        _x = 0;
        _y = 0;
    }
    void moveToTopRight(void)
    {
        _x = M5.Display.width() - width();
        _y = 0;
    }

#pragma endregion
    bool updatePosition()
    {
        if (positionType == PositionType::None)
            return false;
        auto pos = getScreenPosition(positionType);
        _x = pos.first;
        _y = pos.second;
        return true;
    }
    bool setPositionType(PositionType pos)
    {
        this->positionType = pos;
        auto result = getScreenPosition(pos);
        _x = result.first;
        _y = result.second;
        return true;
    }
    bool setPosition(PositionType pos)
    {
        auto result = getScreenPosition(pos);
        _x = result.first;
        _y = result.second;
        return true;
    }

    std::pair<int, int> getScreenPosition(PositionType pos)
    {
        switch (pos)
        {
        case PositionType::TopLeft:
            return std::make_pair(0, 0);
            break;
        case PositionType::TopRight:
            return std::make_pair(M5.Display.width() - width(), 0);
            break;
        case PositionType::TopCenter:
            return std::make_pair((M5.Display.width() - width()) / 2, 0);
            break;
        case PositionType::MiddleLeft:
            return std::make_pair(0, (M5.Display.height() - height()) / 2);
            break;
        case PositionType::MiddleRight:
            return std::make_pair(M5.Display.width() - width(), (M5.Display.height() - height()) / 2);
            break;
        case PositionType::BottomLeft:
            return std::make_pair(0, M5.Display.height() - height());
            break;
        case PositionType::BottomRight:
            // 0割りエラー対策
            if (height() == 0)
                return std::make_pair(0, 0);
            return std::make_pair(M5.Display.width() - width(), M5.Display.height() - height());
            break;
        case PositionType::BottomCenter:
            // 0割りエラー対策
            if (height() == 0)
                return std::make_pair(0, 0);
            return std::make_pair((M5.Display.width() - width()) / 2, M5.Display.height() - height());
            break;
        case PositionType::Center:
            // 0割りエラー対策
            if (height() == 0)
                return std::make_pair(0, 0);
            return std::make_pair((M5.Display.width() - width()) / 2, (M5.Display.height() - height()) / 2);
            break;
        }
    }
#ifdef EFONT
#pragma region Text
    // スクロールの必要があればスクロール
    void scrollIfNeeded(int height)
    {
        int posX = canvas.getCursorX();
        int posY = canvas.getCursorY();

        if (posY + height > canvas.height())
        {
            canvas.scroll(0, -height);
            posY -= height;
        }
        canvas.setCursor(posX, posY);
    }
    void print(const char *message)
    {
        puts(message, _textSize, _textColor, _backgroundColor);
    }
    void printf(const char *format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        puts(buffer, _textColor, _backgroundColor);
    }
    void println(const char *format, ...)
    {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);

        puts(buffer, _textColor, _backgroundColor);
        puts("\n", _textSize, _textColor, _backgroundColor);
    }

    void puts(const char *str, int textSize = 1, uint16_t textColor = WHITE, uint16_t backgroundColor = BLACK)
    {
        while (*str != 0x00)
        {
            put(*str, textSize, textColor, backgroundColor);
            str++;
            continue;
        }
    }

    /// @brief 一文字表示
    /// @param ch 表示する文字コード
    /// @param textSize テキストサイズ(1,2,4,8)
    /// @param textColor 描画色
    /// @param backColor 背景色
    void put(char ch, int textSize = 1, uint32_t textColor = 0xFFFFFF, uint32_t backgroundColor = 0x00000)
    {
        // テキストサイズ
        int textWidth = textSize * 16;
        int textHeight = textSize * 16;

        // 半角
        if (ch < 0x80)
        {
            textWidth = textSize * 8;
        }

        // 表示するフォントが下方向にはみ出す場合は上にスクロール
        scrollIfNeeded(textHeight);

        // 改行の場合は改行してカーソルを進める
        int posX = canvas.getCursorX();
        int posY = canvas.getCursorY();

        if (ch == '\n')
        {
            scrollIfNeeded(textHeight);
            posX = 0;
            posY += textHeight;
            canvas.setCursor(posX, posY);
            return;
        }
        if (ch == '\r')
        {
            posX = 0;
            canvas.setCursor(posX, posY);
            return;
        }

        // 文字背景塗りつぶし
        canvas.fillRect(posX, posY, textWidth, textHeight, backgroundColor);

        // フォント取得
        byte font[32];
        uint16_t strUTF16;
        efontUFT8toUTF16(&strUTF16, &ch);
        getefontData(font, strUTF16);

        // フォント描画
        for (uint8_t row = 0; row < 16; row++)
        {
            word fontdata = font[row * 2] * 256 + font[row * 2 + 1];
            for (uint8_t col = 0; col < 16; col++)
            {
                if ((0x8000 >> col) & fontdata)
                {
                    int drawX = posX + col * textSize;
                    int drawY = posY + row * textSize;
                    if (textSize == 1)
                    {
                        canvas.drawPixel(drawX, drawY, textColor);
                    }
                    else
                    {
                        canvas.fillRect(drawX, drawY, textSize, textSize, textColor);
                    }
                }
            }
        }

        // カーソルを進める、画面外にカーソルがすすめば折り返し
        posX += textWidth;
        if (posX >= canvas.width())
        {
            posX = 0;
            posY += textHeight;
        }
        // カーソルを更新
        canvas.setCursor(posX, posY);
    }
#pragma endregion
#endif
};

#pragma once
#include <M5Unified.h>


#ifndef M5UI_COLOR_DEPTH
#define M5UI_COLOR_DEPTH 8
#endif

class Sprite
{
    int _x = 0;
    int _y = 0;
    int _width = 0;
    int _height = 0;
    bool _backup = false;
    static int _spriteNo;
public:
    int _id;
    String tag;
#pragma region SpriteManager
    static std::vector<Sprite *> _sprites;
    static void updateAll()
    {
        for (int i = 0; i < _sprites.size(); i++)
        {
            _sprites[i]->push();
        }
    }
    static void setup()
    {
        _sprites.clear();
        _spriteNo = 0;
    }
    static void add(Sprite *sprite)
    {
        if(sprite->_id == 0)
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
    static Sprite *findById(int id){
        for (int i = 0; i < _sprites.size(); i++)
        {
            if (_sprites[i]->_id == id)
            {
                return _sprites[i];
            }
        }
        return NULL;    
    }
    static Sprite& get(int id){
        return *findById(id);
    }
    static Sprite& get(String tag){
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
    Sprite(M5Canvas *parent,String tag, int width, int height, int x = 0, int y = 0, int depth = M5UI_COLOR_DEPTH, bool psram = false) : parentCanvas(parent)
    {
        this->tag = tag;
        parentCanvas = parent;
        if (create(width, height, x, y, depth, psram) == NULL)
        {
            LOG_E("Sprite create error");
        }
    }
    int id(){
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
    void drawDebug(){
        canvas.clear(TFT_RED);
        canvas.setCursor(0,0);
        canvas.printf("%d,%d",_x,_y);
    }
    void pushBackground(void)
    {
        uint8_t *buffer = new uint8_t[width() * height() * _depth / 8];
        parentCanvas->readRect(_x, _y, width(), height(), buffer);
        backgroundCanvas.pushImage(0, 0, width(), height(), buffer);
        delete[] buffer;
        _backup = true;
    }
    void popBackground(void)
    {
        if (_backup)
        {
            uint8_t *buffer = new uint8_t[width() * height() * _depth / 8];
            backgroundCanvas.readRect(0, 0, width(), height(), buffer);
            parentCanvas->pushImage(_x, _y, width(), height(), buffer);
            delete[] buffer;
            _backup = false;
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

        Sprite::add(this);

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

#pragma region Push
    void push(void)
    {
        // 背景バックアップONの場合は背景を復帰
        if (_backup)
        {
            popBackground();
        }
        canvas.pushSprite(parentCanvas, _x, _y);
    }
    void push(int x, int y)
    {
        _x = x;
        _y = y;
        canvas.pushSprite(parentCanvas, x, y);
    }
    /*
    void pushImage(int x,int y,int width,int height,const uint8_t* data){
        //canvas.pushImage(x,y,width,height,data);
        //canvas.pushImage(parentCanvas,_x,_y,width,height,data);
     //   this->parentCanvas->pushImage(x,y,width,height,(const uint8_t*)data);
    }
*/
#pragma endregion

#pragma region Move
    void moveTo(int x, int y, TweenType type = TweenType::LINEAR, unsigned long duration = 100, bool loop = false)
    {
        LOG_V("MoveTo:%d,%d", x, y);

        if(_x == x && _y == y) return;

        Tween *tweenX = Tween::create(_x, x, duration, type, loop);       
        tweenX->setProgressCallback([this](float progress,float value) {
            _x = (int)(value);
            LOG_V("moving :%d,%d",_x,_y);
        });
        tweenX->start();
       // tweenX->setCompletionCallback([this]() {
       //     _x = 0;
       // });

        Tween::create(_y, y, duration, type, loop)->start()
        .setProgressCallback([this](float progress,float value) {
            _y = value;
        });

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

#pragma region Animation Move
    void moveWithTween(int x, int y, int duration)
    {
        int dx = x - _x;
        int dy = y - _y;
        int step = 10;
        int count = duration / step;
        for (int i = 0; i < count; i++)
        {
            _x += dx / count;
            _y += dy / count;
            push();
            delay(step);
        }
    }

    void moveBy(int x, int y)
    {
        _x += x;
        _y += y;
    }
    void moveBy(int x, int y, int duration)
    {
        int dx = x - _x;
        int dy = y - _y;
        int step = 10;
        int count = duration / step;
        for (int i = 0; i < count; i++)
        {
            _x += dx / count;
            _y += dy / count;
            push();
            delay(step);
        }
    }
};

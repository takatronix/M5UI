#pragma once
#include <M5Unified.h>
#include <vector>
#include <algorithm>
#include <functional>
#include "Log.hpp"

enum class TweenType
{
    NONE,         // イージングなし (デフォルト)
    LINEAR,       // 等速で値を変化させる
    EASE_IN,      // ゆっくり始まり、徐々に加速
    EASE_OUT,     // 最初は速く、徐々に減速
    EASE_IN_OUT,  // ゆっくり始まり、中盤で加速、最後に再び減速
    BOUNCE,       // 落下して跳ね返る動き
    ELASTIC,      // 物体が跳ね返るような動き
    CUBIC,        // ３次方程式に基づく動き
    SINUSOIDAL    // シヌス波に基づく動き
};

class Tween
{
public:
#pragma region Static Functions
    static std::vector<Tween *> _tweens;
    static char* tweenName(TweenType type){
        switch (type)
        {
        case TweenType::LINEAR:
            return "LINEAR";
            break;
        case TweenType::EASE_IN:
            return "EASE_IN";
            break;
        case TweenType::EASE_OUT:
            return "EASE_OUT";
            break;
        case TweenType::EASE_IN_OUT:
            return "EASE_IN_OUT";
            break;
        case TweenType::BOUNCE:
            return "BOUNCE";
            break;
        case TweenType::ELASTIC:
            return "ELASTIC";
            break;
        case TweenType::CUBIC:
            return "CUBIC";
            break;
        case TweenType::SINUSOIDAL:
            return "SINUSOIDAL";
            break;
        default:
            return "NONE";
            break;
        }
    }
    static void addTween(Tween *tween)
    {
        _tweens.push_back(tween);
    }

    static void removeTween(Tween *tween)
    {
        _tweens.erase(std::remove(_tweens.begin(), _tweens.end(), tween), _tweens.end());
        delete tween;
    }
    static Tween *create(float start, float end, unsigned long duration, TweenType type = TweenType::LINEAR, bool loop = false)
    {
        Tween *tween = new Tween(start, end, duration, type, loop);
        addTween(tween);
        return tween;
    }

    static void updateAll()
    {
        for (auto it = _tweens.begin(); it != _tweens.end();)
        {
            Tween *tween = *it;
            if (tween->update())
            {
                ++it;
            }
            else
            {
                it = _tweens.erase(it);
                delete tween;
            }
        }
    }

#pragma endregion

    Tween(float start, float end, unsigned long duration, TweenType type = TweenType::LINEAR, bool loop = false)
        : _startValue(start), _endValue(end), _duration(duration), _type(type), _loop(loop), _isRunning(false), _isCompleted(true) {}

    // アニメーション完了時に実行する関数を設定
    Tween& onComplete(std::function<void()> callback)
    {
        _completionCallback = callback;
        return *this;
    }
    Tween& onUpdate(std::function<void(float,float)> callback)
    {
        _progressCallback = callback;
        return *this;
    }
    char* getName(){
        return tweenName(_type);
    }

    // アニメーションを開始する
    Tween& start()
    {
        _startTime = millis();
        _isCompleted = false;
        _isRunning = true;
        return *this;
    }

    // アニメーションをキャンセルする
    void cancel()
    {
        _isRunning = false;
        _isCompleted = true;
    }

    // アニメーションを更新し、必要に応じて現在の値を計算する
    bool update()
    {
        if (!_isRunning)
            return false;

        unsigned long currentTime = millis();
        float progress = static_cast<float>(currentTime - _startTime) / _duration;

        if (progress >= 1.0f)
        {
            progress = 1.0f;
            _isRunning = false;
            _isCompleted = true;
            if (_completionCallback){
                _completionCallback();
                _completionCallback = NULL;
            }
            if (_loop)
                start(); // ループが有効なら、再び開始する
        }

        // 選択されたイージング関数に基づいて値を計算
        switch (_type)
        {
        case TweenType::LINEAR:
            _currentValue = linear(progress);
            break;
        case TweenType::EASE_IN:
            _currentValue = easeIn(progress);
            break;
        case TweenType::EASE_OUT:
            _currentValue = easeOut(progress);
            break;
        case TweenType::EASE_IN_OUT:
            _currentValue = easeInOut(progress);
            break;
        case TweenType::BOUNCE:
            _currentValue = bounce(progress);
            break;
        case TweenType::ELASTIC:
            _currentValue = elastic(progress);
            break;
        case TweenType::CUBIC:
            _currentValue = cubic(progress);
            break;
        case TweenType::SINUSOIDAL:
            _currentValue = sinusoidal(progress);
            break;
        // なしの場合はアニメーションなし
        default:
            _currentValue = _endValue;
            break;
        }
        if(_progressCallback){
            _progressCallback(progress,_currentValue);
        }
        //LOG_D("[%s] progress:%f value:%f",tweenName(_type),progress,_currentValue);
        return true;
    }

    // アニメーションが実行中かどうかを返す
    bool isRunning() const
    {
        return _isRunning;
    }

    // 現在のアニメーション値を返す
    float currentValue() const
    {
        return _currentValue;
    }


private:
    float _startValue, _endValue, _currentValue;
    unsigned long _duration, _startTime;
    TweenType _type;
    bool _loop;
    bool _isRunning, _isCompleted;
    std::function<void()> _completionCallback;
    std::function<void(float,float)> _progressCallback;


    // 等速で値を変化させる
    float linear(float t) { return (_endValue - _startValue) * t + _startValue; }

    // ゆっくり始まり、徐々に加速する
    float easeIn(float t) { return (_endValue - _startValue) * t * t + _startValue; }

    // 最初は速く、徐々に減速する
    float easeOut(float t) { return -(_endValue - _startValue) * t * (t - 2) + _startValue; }

    // ゆっくり始まり、中盤で加速、最後に再び減速する
    float easeInOut(float t)
    {
        if ((t *= 2) < 1)
            return (_endValue - _startValue) / 2 * t * t + _startValue;
        return -(_endValue - _startValue) / 2 * ((--t) * (t - 2) - 1) + _startValue;
    }

    // 落下して跳ね返る動き
    float bounce(float t)
    {
        t = 1.0f - t;
        if (t < (1 / 2.75f))
        {
            return _endValue - (_endValue - _startValue) * (7.5625f * t * t);
        }
        else if (t < (2 / 2.75f))
        {
            float postFix = t -= (1.5f / 2.75f);
            return _endValue - (_endValue - _startValue) * (7.5625f * postFix * postFix + .75f);
        }
        else if (t < (2.5 / 2.75))
        {
            float postFix = t -= (2.25f / 2.75f);
            return _endValue - (_endValue - _startValue) * (7.5625f * postFix * postFix + .9375f);
        }
        else
        {
            float postFix = t -= (2.625f / 2.75f);
            return _endValue - (_endValue - _startValue) * (7.5625f * postFix * postFix + .984375f);
        }
    }


    // エラスティック関数の実装
    float elastic(float t)
    {
        if (t == 0)
            return _startValue;
        if (t == 1)
            return _endValue;
        float p = _duration * 0.3f;
        float a = _endValue - _startValue;
        float s = p / 4;
        t -= 1;
        return (_startValue - a * pow(2, 10 * t) * sin((t * _duration - s) * (2 * M_PI) / p));
    }

    // キュービック関数の実装
    float cubic(float t)
    {
        return (_endValue - _startValue) * t * t * t + _startValue;
    }

    // シヌソイダル関数の実装
    float sinusoidal(float t)
    {
        return -(_endValue - _startValue) * cos(t * (M_PI / 2)) + _endValue;
    }
};

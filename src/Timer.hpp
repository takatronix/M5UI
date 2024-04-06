#pragma once
#include <functional> // std::functionを使用するために必要

class Timer {
public:
  Timer() = delete; // デフォルトコンストラクタは禁止（時間を指定しないと使えないため
  // コンストラクタ。タイマーの時間をミリ秒で設定。
  Timer(unsigned long duration)
  : _duration(duration), _startTime(0), _isRunning(false) {}

  // タイマーを開始するメソッド。コールバック関数を引数に取る。
  Timer& start(std::function<void()> callback) {
    _startTime = millis();
    _isRunning = true;
    _callback = callback;
  }

  // タイマーが経過したかどうかをチェックし、必要に応じてコールバックを実行
  void update() {
    if (!_isRunning) return;
    
    if ((millis() - _startTime) >= _duration) {
      _isRunning = false; // タイマーを停止
      if (_callback) _callback(); // コールバック関数が設定されていれば実行
    }
  }

  // タイマーをリセットするメソッド
  Timer& reset() {
    _isRunning = false;
  }

private:
  unsigned long _duration; // タイマーの時間
  unsigned long _startTime; // タイマーが開始された時刻
  bool _isRunning; // タイマーが実行中かどうか
  std::function<void()> _callback; // タイマー経過時に実行されるコールバック関数
};

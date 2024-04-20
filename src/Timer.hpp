#pragma once
#include <M5Unified.h>
#include <vector>
#include <algorithm>
#include <functional>

/*

  5秒後に何かを実行する例
  Timer::create(5000, []() {

  });


*/

class Timer {

   static std::vector<Timer *> _timers;
   static std::vector<Timer *> _timersToAdd;
public:
   static Timer& create(unsigned long duration, std::function<void()> callback) {
     Timer* timer = new Timer();
     timer->start(duration, callback);
     _timersToAdd.push_back(timer); // 新しいタイマーを追加用リストに追加
     return *timer;
   }
   static void updateAll() {
     // 更新処理の前に新しいタイマーをメインリストに追加
     _timers.insert(_timers.end(), _timersToAdd.begin(), _timersToAdd.end());
     _timersToAdd.clear();

     for (auto it = _timers.begin(); it != _timers.end();) {
       Timer *timer = *it;
       timer->update();
       if (timer->isRunning()) {
         ++it;
       } else {
         it = _timers.erase(it);
         delete timer;
       }
     }
   }
  Timer(){
    _duration = 0;
    _startTime = 0;
    _isRunning = false;
  }
  // タイマーを開始する。時間とコールバック関数を引数に取る。
  void start(unsigned long duration, std::function<void()> callback) {
    _duration = duration;
    _callback = callback;
    _startTime = millis();
    _isRunning = true;
  }
  bool isRunning() {
    return _isRunning;
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

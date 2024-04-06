#ifndef _M5UI_h_
#define _M5UI_h_

#include <M5Unified.h>

#define LOG_ENABLE_ERROR
#define LOG_ENABLE_WARN
#define LOG_ENABLE_INFO
#define LOG_ENABLE_DEBUG
#define LOG_ENABLE_VERBOSE
#include "Log.hpp"

#include "Device.hpp"
#include "Timer.hpp"
#include "Sound.hpp"
#include "StopWatch.hpp"
#include "Tween.hpp"

#include "Sprite.hpp"
#include "TextSprite.hpp"
#include "ImageSprite.hpp"
#include "M5UICanvas.hpp"
#include "M5UIClass.hpp"


extern M5UIClass M5UI;

#endif
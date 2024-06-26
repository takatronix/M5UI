#pragma once

#include <M5Unified.h>
#include <WiFi.h>
#include <HTTPClient.h>

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
#include "BatterySprite.hpp"
#include "ConsoleSprite.hpp"
#include "WifiSprite.hpp"
#include "Renderer.hpp"
#include "M5UICanvas.hpp"
#include "unit/Unit.hpp"
#include "BTSerial.h"
#include "Task.h"
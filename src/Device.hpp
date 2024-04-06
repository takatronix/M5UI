// Created by: takatronix
#pragma once
#include <M5Unified.h>

/// @brief デバイスの向きを表す列挙型
enum DeviceOrientation
{
    Unknown,

    // M5Stack系
    Landscape,           // 通常
    LandscapeUpsideDown, // 上下逆
    PortraitLeft,        // ボタンが左
    PortraitRight,       // ボタンが右

    // Stick/Atom系
    Portrait,           // 通常
    PortraitUpsideDown, // 上下逆
    LandscapeRight,     // USBが右
    LandscapeLeft,      // USBが左

    FaceUp,  // 上向き
    FaceDown // 下向き
};

/// @brief デバイスの情報を取得するクラス
class Device
{
    static constexpr float threshold = 0.5f;

public:
    /// @brief デバイスの向きが変更されたかを取得
    /// @return true:変更された false:変更されていない
    static bool wasOrientationChanged()
    {
        static DeviceOrientation prevOrientation = Unknown;
        DeviceOrientation orientation = getOrientation();
        if (prevOrientation != orientation)
        {
            prevOrientation = orientation;
            return true;
        }
        return false;
    }

    /// @brief デバイスの向きが変更されたかを取得(faceup/facedownを除く)
    /// @return true:変更された false:変更されていない
    static bool wasGeneralOrientationChanged()
    {
        static DeviceOrientation prevOrientation = Unknown;
        DeviceOrientation orientation = getGeneralOrientation();
        if (orientation == Unknown)
            return false;
        if (prevOrientation != orientation)
        {
            prevOrientation = orientation;
            return true;
        }
        return false;
    }

    /// @brief デバイスの向きがPortraitかを取得
    static bool isPortrait()
    {
        return getOrientation() == Portrait || getOrientation() == PortraitUpsideDown || getOrientation() == PortraitLeft || getOrientation() == PortraitRight;
    }
    /// @brief デバイスの向きがLandscapeかを取得
    static bool isLandscape()
    {
        return getOrientation() == LandscapeLeft || getOrientation() == LandscapeRight || getOrientation() == Landscape || getOrientation() == LandscapeUpsideDown;
    }
    /// @brief デバイスの向きがFaceUpかを取得
    static bool isFaceUp()
    {
        return getOrientation() == FaceUp;
    }
    /// @brief デバイスの向きがFaceDownかを取得
    static bool isFaceDown()
    {
        return getOrientation() == FaceDown;
    }

    /// @brief デバイスの向きの名称を取得
    /// @param orientation デバイスの向き
    /// @return
    static const char *getOrientationName(DeviceOrientation orientation)
    {
        switch (orientation)
        {
        case Portrait:
            return "Portrait";
        case PortraitUpsideDown:
            return "PortraitUpsideDown";
        case PortraitLeft:
            return "PortraitLeft";
        case PortraitRight:
            return "PortraitRight";
        case LandscapeLeft:
            return "LandscapeLeft";
        case LandscapeRight:
            return "LandscapeRight";
        case Landscape:
            return "Landscape";
        case LandscapeUpsideDown:
            return "LandscapeUpsideDown";
        case FaceUp:
            return "FaceUp";
        case FaceDown:
            return "FaceDown";
        default:
            return "Unknown";
        }
    }

    /// @brief デバイスの向きの名称を取得
    static const char *getOrientationName()
    {
        return getOrientationName(getOrientation());
    }
    static DeviceOrientation getOrientation()
    {
        // 加速度センサーの値を取得
        float ax, ay, az;
        M5.Imu.getAccelData(&ax, &ay, &az);

        if (az > threshold)
            return FaceUp;
        if (az < -threshold)
            return FaceDown;

        // デバイスの向きを判断
        return detectOrientation(ax, ay, az);
    }

    // 加速度センサーの値からデバイスの向きを判断する関数
    static DeviceOrientation detectOrientation(float ax, float ay, float az)
    {
        // stick系
        if (Device::isStick() || Device::isAtom())
        {
            if (ax > threshold)
                return LandscapeRight;
            if (ax < -threshold)
                return LandscapeLeft;
            if (ay > threshold)
                return Portrait;
            if (ay < -threshold)
                return PortraitUpsideDown;
            return Unknown;
        }

        // M5Stack系
        if (ax > threshold)
            return PortraitRight;
        if (ax < -threshold)
            return PortraitLeft;
        if (ay > threshold)
            return Landscape;
        if (ay < -threshold)
            return LandscapeUpsideDown;

        return Unknown;
    }

    // faceup/facedownを返さないバージョン
    static DeviceOrientation getGeneralOrientation()
    {
        static DeviceOrientation prevOrientation = Unknown;

        // 加速度センサーの値を取得
        float ax, ay, az;
        M5.Imu.getAccelData(&ax, &ay, &az);

        DeviceOrientation orientation = detectOrientation(ax, ay, az);
        if (orientation != Unknown)
        {
            prevOrientation = orientation;
            return orientation;
        }
        return prevOrientation;
    }

    /// @brief ボード名を取得
    /// @return ボード名
    static const char *getBoardName()
    {
        const char *name;
        switch (M5.getBoard())
        {
        case m5::board_t::board_M5AtomS3Lite:
            name = "ATOMS3Lite";
            break;
        case m5::board_t::board_M5AtomS3:
            name = "ATOMS3";
            break;
        case m5::board_t::board_M5StampC3:
            name = "StampC3";
            break;
        case m5::board_t::board_M5StampC3U:
            name = "StampC3U";
            break;
        case m5::board_t::board_M5Stack:
            name = "Stack";
            break;
        case m5::board_t::board_M5StackCore2:
            name = "StackCore2";
            break;
        case m5::board_t::board_M5StackCoreS3:
            name = "StackCoreS3";
            break;
        case m5::board_t::board_M5StickC:
            name = "StickC";
            break;
        case m5::board_t::board_M5StickCPlus:
            name = "StickCPlus";
            break;
        case m5::board_t::board_M5StickCPlus2:
            name = "StickCPlus2";
            break;
        case m5::board_t::board_M5StackCoreInk:
            name = "CoreInk";
            break;
        case m5::board_t::board_M5Paper:
            name = "Paper";
            break;
        case m5::board_t::board_M5Tough:
            name = "Tough";
            break;
        case m5::board_t::board_M5Station:
            name = "Station";
            break;
        case m5::board_t::board_M5Atom:
            name = "ATOM";
            break;
        case m5::board_t::board_M5AtomPsram:
            name = "ATOM PSRAM";
            break;
        case m5::board_t::board_M5AtomU:
            name = "ATOM U";
            break;
        case m5::board_t::board_M5TimerCam:
            name = "TimerCamera";
            break;
        case m5::board_t::board_M5StampPico:
            name = "StampPico";
            break;
        default:
            name = "Unknown";
            break;
        }
        return name;
    }

    static bool isStick()
    {
        return M5.getBoard() == m5::board_t::board_M5StickC || M5.getBoard() == m5::board_t::board_M5StickCPlus || M5.getBoard() == m5::board_t::board_M5StickCPlus2;
    }
    static bool isAtom()
    {
        return M5.getBoard() == m5::board_t::board_M5Atom || M5.getBoard() == m5::board_t::board_M5AtomPsram || M5.getBoard() == m5::board_t::board_M5AtomU || M5.getBoard() == m5::board_t::board_M5AtomS3 || M5.getBoard() == m5::board_t::board_M5AtomS3Lite;
    }
    static bool isStamp()
    {
        return M5.getBoard() == m5::board_t::board_M5StampC3 || M5.getBoard() == m5::board_t::board_M5StampC3U || M5.getBoard() == m5::board_t::board_M5StampPico;
    }
    static bool isStack()
    {
        return M5.getBoard() == m5::board_t::board_M5Stack || M5.getBoard() == m5::board_t::board_M5StackCore2 || M5.getBoard() == m5::board_t::board_M5StackCoreS3 || M5.getBoard() == m5::board_t::board_M5StackCoreInk;
    }
    static bool isPaper()
    {
        return M5.getBoard() == m5::board_t::board_M5Paper;
    }
    static bool hasVibe()
    {
        if (M5.getBoard() == m5::board_t::board_M5StackCore2 || M5.getBoard() == m5::board_t::board_M5StackCoreS3 || M5.getBoard() == m5::board_t::board_M5StackCoreInk)
        {
            return true;
        }
        return false;
    }

    /// @brief 画面の向きを取得
    static int getRotation()
    {
        DeviceOrientation orientation = getOrientation();

        if (isStick() || isAtom())
        {
            switch (orientation)
            {
            case Portrait:
                return 0;
            case PortraitUpsideDown:
                return 2;
            case LandscapeRight:
                return 1;
            case LandscapeLeft:
                return 3;
            default:
                return -1;
            }
        }

        switch (orientation)
        {
        case PortraitLeft:
            return 0;
        case PortraitRight:
            return 2;
        case Landscape:
            return 1;
        case LandscapeUpsideDown:
            return 3;
        default:
            return -1;
        }

        return -1;
    }

    /// @brief デバイスの向きに合わせた画面の幅を取得
    /// @return 画面の幅(ピクセル)
    static int getVisibleDisplayWidth()
    {
        DeviceOrientation orientation = getGeneralOrientation();

        // stick系
        if (orientation == Portrait || orientation == PortraitUpsideDown)
        {
            return min(M5.Display.height(), M5.Display.width());
        }
        if (orientation == LandscapeRight || orientation == LandscapeLeft)
        {
            return max(M5.Display.height(), M5.Display.width());
        }

        // M5Stack系
        if (orientation == Landscape || orientation == LandscapeUpsideDown)
        {
            return max(M5.Display.height(), M5.Display.width());
        }

        return min(M5.Display.height(), M5.Display.width());
    }

    /// @brief デバイスの向きに合わせた画面の高さを取得
    /// @return 画面の高さ(ピクセル)
    static int getVisibleDisplayHeight()
    {
        DeviceOrientation orientation = getGeneralOrientation();

        // stick系
        if (orientation == Portrait || orientation == PortraitUpsideDown)
        {
            return max(M5.Display.height(), M5.Display.width());
        }
        if (orientation == LandscapeRight || orientation == LandscapeLeft)
        {
            return min(M5.Display.height(), M5.Display.width());
        }
        // M5Stack系
        if (orientation == Landscape || orientation == LandscapeUpsideDown)
        {
            return min(M5.Display.height(), M5.Display.width());
        }

        return max(M5.Display.height(), M5.Display.width());
    }

    /// @brief バッテリー残量を取得
    /// @return バッテリー残量(%)
    static int getBatteryLevel()
    {
        return M5.Power.getBatteryLevel();
    }
    // ヒープの空き容量を取得
    static int getFreeDmaSize()
    {
        return heap_caps_get_free_size(MALLOC_CAP_DMA);
    }
    // PSRAMの空き容量を取得
    static int getFreePsramSize()
    {
        return heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
    }
    // 1ブロックの最大空き容量を取得
    static int getLargestFreeBlock()
    {
        return heap_caps_get_largest_free_block(MALLOC_CAP_DMA);
    }
    //  PSRAM 1ブロックの最大空き容量を取得
    static int getLargestFreePsramBlock()
    {
        return heap_caps_get_largest_free_block(MALLOC_CAP_SPIRAM);
    }

    static void vibrate(bool on)
    {
        if (!Device::hasVibe)
            return;
        if (on)
        {
            //    M5.Power.Axp192.SetVibration(true);
        }
        else
        {
            //     M5.Power.Axp192.SetVibration(false);
        }
    }
    static void vibrate(int ms = 100)
    {
        if (!Device::hasVibe())
            return;
        vibrate(true);
        delay(ms);
        vibrate(false);
    }
};


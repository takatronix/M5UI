#pragma once

class UnitDualButton
{
public:
    int _red;
    int _blue;
    UnitDualButton(int red =22,int blue = 19)
    {
        _red = red;
        _blue = blue;
    }
    void begin()
    {
        pinMode(_red, INPUT); // Red Button Pin setting
        pinMode(_blue, INPUT); // Blue Button Pin setting
    }

    bool isRedButtonPressed()
    {
        if (digitalRead(_red) == 0)
            return true;
        return false;
    }
    bool isBlueButtonPressed()
    {
        if (digitalRead(_blue) == 0)
            return true;
        return false;
    }

    bool wasRedButtonPressed()
    {
        static bool wasPressed = false;
        static bool isPressed = false;
        isPressed = isRedButtonPressed();
        if (isPressed && !wasPressed)
        {
            wasPressed = true;
            return true;
        }
        else if (!isPressed)
        {
            wasPressed = false;
        }
        return false;
    }
    bool wasBlueButtonPressed()
    {
        static bool wasPressed = false;
        static bool isPressed = false;
        isPressed = isBlueButtonPressed();
        if (isPressed && !wasPressed)
        {
            wasPressed = true;
            return true;
        }
        else if (!isPressed)
        {
            wasPressed = false;
        }
        return false;
    }
};
